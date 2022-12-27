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
	SDL_Rect ground = App->physics->CreateGround(0, 500, 1200, 100);
	App->renderer->DrawQuad(ground, 116, 190, 227);
	
	

	return UPDATE_CONTINUE;
}

// PostUpdate: draw fonts
update_status ModuleSceneIntro::PostUpdate()
{

	return UPDATE_CONTINUE;
}
