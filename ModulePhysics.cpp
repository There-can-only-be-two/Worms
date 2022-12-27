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
	//Integrate position and velocity
	
	//check for collisions in new position
	//calculate normal collision and bounce coef

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

SDL_Rect ModulePhysics::CreateGround(float gx, float gy, float gw, float gh)
{
	Ground* ground = new Ground();

	ground->x = PIXEL_TO_METERS(gx); ground->y = PIXEL_TO_METERS(gy);

	ground->w = PIXEL_TO_METERS(gw); ground->h = PIXEL_TO_METERS(gh);

	SDL_Rect groundRect = {gx, gy, gw, gh};

	return groundRect;
}
