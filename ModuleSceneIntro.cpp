#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	/*SDL_Rect ground = App->physics->CreateGround(0, 300, 700, 100);
	App->renderer->DrawQuad(ground, 100, 100, 100, 100);*/

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//Create ground - ON UPDATE????
	SDL_Rect ground = App->physics->CreateGround(0, 500, 1200, 100);
	App->renderer->DrawQuad(ground, 116, 190, 227, 64);
	
	//SDL_Rect water = App->physics->CreateWater(50, 50, 1200, 100);

	//TESTING, DO NOT DELETE
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		PhysBody* bod = new PhysBody();

		bod->ax = 0;
		bod->ay = 0;

		bod->vx = 0;
		bod->vy = 0;

		bod->px = PIXELS_TO_METERS(500);
		bod->py = PIXELS_TO_METERS(100);

		bod->isAlive = true;
		bod->isStable = false;

		App->physics->listBodies.add(bod);
	}

	return UPDATE_CONTINUE;
}

// PostUpdate: draw fonts
update_status ModuleSceneIntro::PostUpdate()
{

	return UPDATE_CONTINUE;
}
