#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include "ModuleFonts.h"

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
	
	gravity = GRAVITY;

	return true;
}

void ModulePhysics::CheckIfHit(PhysBody& body, Explosion& ex) {
	double x = body.px - ex.x;
	double y = body.py - ex.y;
	double h = sqrt((y * y) + (x * x));
	double sn = y / h;
	double cs = x / h;
	if (h <= ex.radius) {
		body.fx = (ex.radius*cs) - x;
		body.fy = (ex.radius*sn) - y;
	}
}

// 
update_status ModulePhysics::PreUpdate()
{
	//Integrate velocity and potential position
	//check for collisions in potential position
	//calculate normal collision and bounce coef

	dt = 1 / App->debug->desiredFPS;

	LOG("DELTA TIME = %f", dt);

	p2List_item<Circle*>* item;
	Circle* pBody = NULL;

	if (App->player->explosionTimer > 0) {
		App->player->explosionTimer--;
	}

	if (App->player->shootingIFrames > 0) {
		App->player->shootingIFrames--;
	}

	for (item = App->physics->listBodies.getFirst(); item != NULL; item = item->next)
	{
		pBody = item->data;

		if (!pBody->isStable)
		{		
			pBody->fx = pBody->fy = 0.0f;
			pBody->ax = 0.0f;
			pBody->ay = 0.0f;

			if (pBody->label == GRENADE)
			{
				// Aerodynamic Drag force (only when not in water)
				if (!is_colliding_with_water(*pBody, *App->scene_intro->water))
				{
					float fdx = 0.0f; float fdy = 0.0f;
					compute_aerodynamic_drag(fdx, fdy, *pBody, *App->scene_intro->atm);
					pBody->fx += fdx; pBody->fy += fdy; // Add this force to ball's total force
				}

			}

			if (App->player->explosionTimer <= 0 && (pBody->label == GRENADE || pBody->label == MISSILE)) {
				App->scene_intro->explosion->x = pBody->px;
				App->scene_intro->explosion->y = pBody->py;
				listBodies.del(item);
			}

			if (App->player->explosionTimer <= 0 
				&& (App->player->isShootingGrenade || App->player->isShootingMissile) 
				&& (pBody->label == PLAYER_1 || pBody->label == PLAYER_2)) {
				double x = pBody->px - App->scene_intro->explosion->x;
				double y = pBody->py - App->scene_intro->explosion->y;
				double h = sqrt((y * y) + (x * x));
				double sn = y / h;
				double cs = x / h;
				if (h <= App->scene_intro->explosion->radius) {
					pBody->fx += 1000*(App->scene_intro->explosion->radius * cs) - x;
					pBody->fy += 1000*(App->scene_intro->explosion->radius * sn) - y;
					if (pBody->isHit) {
						pBody->life -= 20;
						pBody->isHit = false;
					}
				}
			}

			
			// Gravity force
			float fgx = 0.0f;
			float fgy = pBody->mass * -gravity; // Let's assume gravity is constant and downwards, like in real situations
			pBody->fx += fgx; pBody->fy += fgy; // Add this force to ball's total force

			if ((pBody->label == PLAYER_1 || pBody->label == PLAYER_2) && pBody->isJumping > 0)
			{
				pBody->isGrounded = false;
				pBody->fy -= 1000;
				pBody->isJumping--;
			}

			// Hydrodynamic forces (only when in water)
			if (is_colliding_with_water(*pBody, *App->scene_intro->water))
			{
				if (pBody->label == PLAYER_1 || pBody->label == PLAYER_2) {
					pBody->mass = 200.0f;
					pBody->life = 0;
				}
				if (pBody->label == MISSILE) {
					App->player->explosionTimer = 0;
				}
				else {
					// Hydrodynamic Drag force
					float fhdx = 0.0f; float fhdy = 0.0f;
					compute_hydrodynamic_drag(fhdx, fhdy, *pBody, *App->scene_intro->water);
					pBody->fx += fhdx; pBody->fy += fhdy; // Add this force to ball's total force

					// Hydrodynamic Buoyancy force
					float fhbx = 0.0f; float fhby = 0.0f;
					compute_hydrodynamic_buoyancy(fhbx, fhby, *pBody, *App->scene_intro->water);
					pBody->fx += fhbx; pBody->fy -= fhby; // Add this force to ball's total force
				}	
			}

			// TESTING WITH CODE, DO NOT ERASE

			pBody->ax = pBody->fx / pBody->mass;
			pBody->ay = pBody->fy / pBody->mass;

			// Solve collision between ball and ground(list)
			p2List_item<Ground*>* gItem;
			Ground* ground = NULL;
			for (gItem = App->scene_intro->listGrounds.getFirst(); gItem != NULL; gItem = gItem->next)
			{
				ground = gItem->data;

				if (is_colliding_with_ground(*pBody, *ground))
				{					
					detect_direction_ground(*pBody, *ground);

					if (pBody->label == PLAYER_1 || pBody->label ==  PLAYER_2) {
						pBody->isGrounded = true;
					}
					if (pBody->label == MISSILE) {
						App->player->explosionTimer = 0;
					}
				}
			}

			if (pBody->label != PLAYER_1 && pBody->label != PLAYER_2) {
				if (is_colliding_with_player(*pBody, *App->player->listPlayers.getFirst()->data) 
					|| is_colliding_with_player(*pBody, *App->player->listPlayers.getLast()->data)) {
					if (App->player->shootingIFrames == 0) {
						App->player->explosionTimer = 0;
					}
				}
			}

			
			if (is_colliding_with_enemy(*pBody, *App->scene_intro->enemy)){
				detect_direction_enemy(*pBody, *App->scene_intro->enemy);
			}

			integrator_velocity_verlet(*pBody, dt);

			
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

Ground* ModulePhysics::CreateGround(float gx, float gy, float gw, float gh)
{
	//creating dynamically allocated object on function scope???
	//FIX
	Ground* ground = new Ground();

	ground->x = PIXELS_TO_METERS(gx); ground->y = PIXELS_TO_METERS(gy);

	ground->w = PIXELS_TO_METERS(gw); ground->h = PIXELS_TO_METERS(gh);

	return ground;
}

Water* ModulePhysics::CreateWater(float wx, float wy, float ww, float wh)
{
	Water* water = new Water();

	water->x = PIXELS_TO_METERS(wx); water->y = PIXELS_TO_METERS(wy);

	water->w = PIXELS_TO_METERS(ww); water->h = PIXELS_TO_METERS(wh);
	
	water->density = 50.0f; // [kg/m^3]
	water->vx = -1.0f; // [m/s]
	water->vy = 0.0f; // [m/s]

	return water;
}

Atmosphere* ModulePhysics::CreateAtmosphere()
{
	Atmosphere* atm = new Atmosphere();
	
	atm->windx = 10.0f; // [m/s]
	atm->windy = 5.0f; // [m/s]
	atm->density = 1.0f; // [kg/m^3]

	return atm;
}

Enemy* ModulePhysics::CreateEnemy(float ex, float ey, float ew, float eh)
{
	Enemy* enemy = new Enemy();

	enemy->x = PIXELS_TO_METERS(ex); enemy->y = PIXELS_TO_METERS(ey);

	enemy->w = PIXELS_TO_METERS(ew); enemy->h = PIXELS_TO_METERS(eh);

	enemy->life = 100;

	return enemy;
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

//Integration scheme: Velocity Verlet
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

// Detect collision with enemy
bool ModulePhysics::is_colliding_with_enemy(const Circle& ball, const Enemy& enemy)
{
	float rect_x = (enemy.x + enemy.w / 2.0f); // Center of rectangle
	float rect_y = (enemy.y + enemy.h / 2.0f); // Center of rectangle
	return check_collision_circle_rectangle(ball.px, ball.py, ball.radius, rect_x, rect_y, enemy.w, enemy.h);
}

bool ModulePhysics::is_colliding_with_player(const Circle& ball, const Circle& player) {
	return check_collision_circles(ball.px, ball.py, ball.radius, player.px, player.py, player.radius);
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

void ModulePhysics::collision_direction(Circle& pBody, const Ground& ground)
{
	double vectorX = 0;
	double vectorY = 0;
	int count = 1;

	for (int i = 0; i < 32; i++)
	{
		//resert point list
		pointList[i][0] = -69420;
		pointList[i][1] = -69420;

		double arc = 360 / 32;
		double x = pBody.px + (pBody.radius * cos(DEG_TO_RAD(i*arc)));
		double y = pBody.py + (pBody.radius * sin(DEG_TO_RAD(i*arc)));

		if ((x >= ground.x) && (x <= ground.x + ground.w) && (y >= ground.y) && (y <= ground.y + ground.h))
		{
			count++;

			pointList[i][0] = x;
			pointList[i][1] = y;

			vectorX += (pBody.px - x);
			vectorY += (pBody.py - y);
		}
	}

	App->physics->vectorX = vectorX;
	App->physics->vectorY = vectorY;
}

void ModulePhysics::detect_direction_ground(Circle& pBody, const Ground& ground)
{	
	//calculate collision vector
	collision_direction(pBody, ground);

	//vector to angle
	angle = RAD_TO_DEG(atan2(vectorY, vectorX));
	//angle = RAD_TO_DEG(atan(vectorX/vectorY));

	//right
	if ((angle >= 0 && angle <= 45) || (angle <= 0 && angle >= -45))
	{
		pBody.px = ground.x + ground.w + pBody.radius;

		// Elastic bounce with ground
		pBody.vx = -pBody.vx;

		// FUYM non-elasticity
		pBody.vy *= pBody.coef_friction;
		pBody.vx *= pBody.coef_restitution;
	}
	//up
	else if (angle >= -135 && angle <= -45)
	{
		pBody.py = ground.y - pBody.radius;

		// Elastic bounce with ground
		pBody.vy = -pBody.vy;

		// FUYM non-elasticity
		pBody.vx *= pBody.coef_friction;
		pBody.vy *= pBody.coef_restitution;
	}
	 
	//left
	else if ((angle <= -135 && angle >= -180) || (angle <= 180 && angle >= 135))
	{
		pBody.px = ground.x - pBody.radius;

		// Elastic bounce with ground
		pBody.vx = -pBody.vx;

		// FUYM non-elasticity
		pBody.vy *= pBody.coef_friction;
		pBody.vx *= pBody.coef_restitution;
	}
	//down
	else if (angle >= 45 && angle <= 135)
	{
		pBody.py = ground.y + ground.h + pBody.radius;

		// Elastic bounce with ground
		pBody.vy = -pBody.vy;

		// FUYM non-elasticity
		pBody.vx *= pBody.coef_friction;
		pBody.vy *= pBody.coef_restitution;
	}

}

void ModulePhysics::detect_direction_enemy(Circle& pBody, const Enemy& enemy) {
	if (std::abs(pBody.vy) > std::abs(pBody.vx)) {
		// TP ball to ground surface
		if (pBody.vy < 0) {
			pBody.py = enemy.y + enemy.h + pBody.radius;
		}
		else if (pBody.vy > 0) {
			pBody.py = enemy.y - pBody.radius;
		}
		// Elastic bounce with ground
		pBody.vy = -pBody.vy;

		// FUYM non-elasticity
		pBody.vx *= pBody.coef_friction;
		pBody.vy *= pBody.coef_restitution;
	}
	else if (std::abs(pBody.vx) > std::abs(pBody.vy)) {
		// TP ball to ground surface
		if (pBody.vx < 0) {
			pBody.px = enemy.x + enemy.w + pBody.radius;
		}
		else if (pBody.vx > 0) {
			pBody.px = enemy.x - pBody.radius;
		}
		// Elastic bounce with ground
		pBody.vx = -pBody.vx;

		// FUYM non-elasticity
		pBody.vy *= pBody.coef_friction;
		pBody.vx *= pBody.coef_restitution;
	}
}

bool ModulePhysics::check_collision_circles(float cx1, float cy1, float cr1, float cx2, float cy2, float cr2) {

	float dist_x = std::abs(cx1 - cx2);
	float dist_y = std::abs(cy1 - cy2);
	float r = cr1 + cr2;
	float h = dist_x * dist_x + dist_y * dist_y;

	return (h <= r * r);
}

void ModulePhysics::SetGravity(float g)
{
	gravity = g;
}

float ModulePhysics::GetGravity()
{
	return gravity;
}