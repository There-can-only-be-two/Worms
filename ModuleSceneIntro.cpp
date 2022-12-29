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

	ground = App->physics->CreateGround(0, 500, 1200, 100);
	water = App->physics->CreateWater(0, 400, 1200, 100);
	atm = App->physics->CreateAtmosphere();
	
	//water->density = 50.0f; // [kg/m^3]
	//water->vx = -1.0f; // [m/s]
	//water->vy = 0.0f; // [m/s]

	
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
	//SDL_Rect ground = App->physics->CreateGround(0, 500, 1200, 100);
	//App->renderer->DrawQuad(ground, 116, 190, 227, 64);
	
	//SDL_Rect water = App->physics->CreateWater(50, 50, 1200, 100);

	//TESTING, DO NOT DELETE

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		Circle* bod = new Circle();

	//if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	//{
	//	PhysBody* bod = new PhysBody();

	//	bod->ax = 0;
	//	bod->ay = 0;

	//	bod->vx = 0;
	//	bod->vy = 0;

	//	bod->px = PIXELS_TO_METERS(500);
	//	bod->py = PIXELS_TO_METERS(100);

	//	bod->isAlive = true;
	//	bod->isStable = false;


		bod->mass = 100.0f; // [kg]
		bod->surface = 1.0f; // [m^2]
		bod->radius = 0.5f; // [m]
		bod->cd = 0.4f; // [-]
		bod->cl = 1.2f; // [-]
		bod->b = 10.0f; // [...]
		bod->coef_friction = 0.9f; // [-]
		bod->coef_restitution = 0.8f; // [-]

		bod->label = GRENADE;

		App->physics->listBodies.add(bod);
	}

	//	bod->label = GRENADE;
	//	App->physics->listBodies.add(bod);
	//}

	return UPDATE_CONTINUE;
}

// PostUpdate: draw fonts
update_status ModuleSceneIntro::PostUpdate()
{

	SDL_Rect gRect = { METERS_TO_PIXELS(ground->x), METERS_TO_PIXELS(ground->y), METERS_TO_PIXELS(ground->w), METERS_TO_PIXELS(ground->h) };
	SDL_Rect wRect = { METERS_TO_PIXELS(water->x), METERS_TO_PIXELS(water->y), METERS_TO_PIXELS(water->w), METERS_TO_PIXELS(water->h) };

	App->renderer->DrawQuad(gRect, 101, 67, 33, 128);
	App->renderer->DrawQuad(wRect, 176, 224, 230, 128);

	return UPDATE_CONTINUE;
}
