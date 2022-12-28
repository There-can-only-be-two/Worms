#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	//Integrate velocity and potential position
	//check for collisions in ppotential position
	//calculate normal collision and bounce coef

	p2List_item<PhysBody*>* item;
	PhysBody* pBody = NULL;

	for (item = App->physics->listBodies.getFirst(); item != NULL; item = item->next)
	{
		pBody = item->data;

		if (!pBody->isStable)
		{
			//0.016 time for 1frame asuming 60Hz
			pBody->ay = GRAVITY;

			pBody->vx += pBody->ax * DELTATIME;
			pBody->vy += pBody->ay * DELTATIME;

			double potentialX = pBody->px + pBody->vx * DELTATIME;
			double potentialY = pBody->py + pBody->vy * DELTATIME;

			//check for collisions on potential pos
			Circle* circle = new Circle();
			circle->px = pBody->px;
			circle->py = pBody->py;
			//is_colliding_with_ground(circle&, ground);

			pBody->px = potentialX;
			pBody->py = potentialY;

		}
	}
	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	return true;
}

void ModulePhysics::Drag(PhysBody* phbody)
{
	float rel_v_modul, drag_modul;

	phbody->v_rel_x = atm.windx - phbody->vx;
	phbody->v_rel_y = atm.windy - phbody->vy;

	rel_v_modul = sqrt((phbody->v_rel_x * phbody->v_rel_x) + (phbody->v_rel_y * phbody->v_rel_y));
	
	drag_modul = 0.5f * AIR_DENSITY * rel_v_modul * rel_v_modul * phbody->drag_surface *phbody->cd;

	phbody->drag_fx = drag_modul * phbody->v_rel_x;
	phbody->drag_fy = drag_modul * phbody->v_rel_y;
	
}

SDL_Rect ModulePhysics::CreateGround(float gx, float gy, float gw, float gh)
{
	//creating dynamically allocated object on function scope???
	//FIX
	Ground* ground = new Ground();

	ground->x = PIXELS_TO_METERS(gx); ground->y = PIXELS_TO_METERS(gy);

	ground->w = PIXELS_TO_METERS(gw); ground->h = PIXELS_TO_METERS(gh);

	SDL_Rect groundRect = {gx, gy, gw, gh};

	return groundRect;
}

SDL_Rect ModulePhysics::CreateWater(float wx, float wy, float ww, float wh)
{
	Water* water = new Water();

	water->x = PIXELS_TO_METERS(wx); water->y = PIXELS_TO_METERS(wy);

	water->w = PIXELS_TO_METERS(ww); water->h = PIXELS_TO_METERS(wh);

	SDL_Rect waterRect = { wx, wy, ww, wh };

	return waterRect;
}

// Compute modulus of a vector
float ModulePhysics::modulus(float vx, float vy)
{
	return std::sqrt(vx * vx + vy * vy);
}

// Compute Aerodynamic Drag force
void ModulePhysics::compute_aerodynamic_drag(float& fx, float& fy, const Circle& ball, const Atmosphere& atmosphere)
{
	float rel_vel[2] = { ball.vx - atmosphere.windx, ball.vy - atmosphere.windy }; // Relative velocity
	float speed = modulus(rel_vel[0], rel_vel[1]); // Modulus of the relative velocity
	float rel_vel_unitary[2] = { rel_vel[0] / speed, rel_vel[1] / speed }; // Unitary vector of relative velocity
	float fdrag_modulus = 0.5f * atmosphere.density * speed * speed * ball.surface * ball.cd; // Drag force (modulus)
	fx = -rel_vel_unitary[0] * fdrag_modulus; // Drag is antiparallel to relative velocity
	fy = -rel_vel_unitary[1] * fdrag_modulus; // Drag is antiparallel to relative velocity
}

// Compute Hydrodynamic Drag force
void ModulePhysics::compute_hydrodynamic_drag(float& fx, float& fy, const Circle& ball, const Water& water)
{
	float rel_vel[2] = { ball.vx - water.vx, ball.vy - water.vy }; // Relative velocity
	float speed = modulus(rel_vel[0], rel_vel[1]); // Modulus of the relative velocity
	float rel_vel_unitary[2] = { rel_vel[0] / speed, rel_vel[1] / speed }; // Unitary vector of relative velocity
	float fdrag_modulus = ball.b * speed; // Drag force (modulus)
	fx = -rel_vel_unitary[0] * fdrag_modulus; // Drag is antiparallel to relative velocity
	fy = -rel_vel_unitary[1] * fdrag_modulus; // Drag is antiparallel to relative velocity
}

// Compute Hydrodynamic Buoyancy force
void ModulePhysics::compute_hydrodynamic_buoyancy(float& fx, float& fy, const Circle& ball, const Water& water)
{
	// Compute submerged area (assume ball is a rectangle, for simplicity)
	float water_top_level = water.y + water.h; // Water top level y
	float h = 2.0f * ball.radius; // Ball "hitbox" height
	float surf = h * (water_top_level - ball.py); // Submerged surface
	if ((ball.py + ball.radius) < water_top_level) surf = h * h; // If ball completely submerged, use just all ball area
	surf *= 0.4; // FUYM to adjust values (should compute the area of circle segment correctly instead; I'm too lazy for that)

	// Compute Buoyancy force
	double fbuoyancy_modulus = water.density * 10.0 * surf; // Buoyancy force (modulus)
	fx = 0.0; // Buoyancy is parallel to pressure gradient
	fy = fbuoyancy_modulus; // Buoyancy is parallel to pressure gradient
}

// Integration scheme: Velocity Verlet
void ModulePhysics::integrator_velocity_verlet(Circle& ball, float dt)
{
	ball.px += ball.vx * dt + 0.5f * ball.ax * dt * dt;
	ball.py += ball.vy * dt + 0.5f * ball.ay * dt * dt;
	ball.vx += ball.ax * dt;
	ball.vy += ball.ay * dt;
}

// Detect collision with ground
bool ModulePhysics::is_colliding_with_ground(const Circle& ball, const Ground& ground)
{
	float rect_x = (ground.x + ground.w / 2.0f); // Center of rectangle
	float rect_y = (ground.y + ground.h / 2.0f); // Center of rectangle
	return check_collision_circle_rectangle(ball.px, ball.py, ball.radius, rect_x, rect_y, ground.w, ground.h);
}

// Detect collision with water
bool ModulePhysics::is_colliding_with_water(const Circle& ball, const Water& water)
{
	float rect_x = (water.x + water.w / 2.0f); // Center of rectangle
	float rect_y = (water.y + water.h / 2.0f); // Center of rectangle
	return check_collision_circle_rectangle(ball.px, ball.py, ball.radius, rect_x, rect_y, water.w, water.h);
}

// Detect collision between circle and rectange
bool ModulePhysics::check_collision_circle_rectangle(float cx, float cy, float cr, float rx, float ry, float rw, float rh)
{
	// Algorithm taken from https://stackoverflow.com/a/402010

	// Distance from center of circle to center of rectangle
	float dist_x = std::abs(cx - rx);
	float dist_y = std::abs(cy - ry);

	// If circle is further than half-rectangle, not intersecting
	if (dist_x > (rw / 2.0f + cr)) { return false; }
	if (dist_y > (rh / 2.0f + cr)) { return false; }

	// If circle is closer than half-rectangle, is intersecting
	if (dist_x <= (rw / 2.0f)) { return true; }
	if (dist_y <= (rh / 2.0f)) { return true; }

	// If all of above fails, check corners
	float a = dist_x - rw / 2.0f;
	float b = dist_y - rh / 2.0f;
	float cornerDistance_sq = a * a + b * b;
	return (cornerDistance_sq <= (cr * cr));
}

// Convert from meters to pixels (for SDL drawing)
//SDL_Rect Ground::pixels()
//{
//	SDL_Rect pos_px{};
//	pos_px.x = METERS_TO_PIXELS(x);
//	pos_px.y = SCREEN_HEIGHT - METERS_TO_PIXELS(y);
//	pos_px.w = METERS_TO_PIXELS(w);
//	pos_px.h = METERS_TO_PIXELS(-h); // Can I do this? LOL
//	return pos_px;
//}