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

	ground = App->physics->CreateGround(400, 600, 600, 300);
	ground2 = App->physics->CreateGround(400, 0, 100, 600);
	water = App->physics->CreateWater(1000, 620, 1200, 200);
	atm = App->physics->CreateAtmosphere();

	
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

	return UPDATE_CONTINUE;
}

// PostUpdate: draw fonts
update_status ModuleSceneIntro::PostUpdate()
{

	SDL_Rect gRect = { METERS_TO_PIXELS(ground->x), METERS_TO_PIXELS(ground->y), METERS_TO_PIXELS(ground->w), METERS_TO_PIXELS(ground->h) };
	SDL_Rect g2Rect = { METERS_TO_PIXELS(ground2->x), METERS_TO_PIXELS(ground2->y), METERS_TO_PIXELS(ground2->w), METERS_TO_PIXELS(ground2->h) };
	SDL_Rect wRect = { METERS_TO_PIXELS(water->x), METERS_TO_PIXELS(water->y), METERS_TO_PIXELS(water->w), METERS_TO_PIXELS(water->h) };

	App->renderer->DrawQuad(gRect, 101, 67, 33, 128);
	App->renderer->DrawQuad(g2Rect, 101, 67, 33, 128);
	App->renderer->DrawQuad(wRect, 176, 224, 230, 128);

	return UPDATE_CONTINUE;
}
