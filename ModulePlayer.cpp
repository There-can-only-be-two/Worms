#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	this->App = app;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	pos.x = 100;
	pos.y = 100;
	LOG("Loading player");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	//Left
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		pos.x -= 5;
	}
	//Right
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		pos.x += 5;
	}
	//Up
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {

	}
	//Down
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {

	}

	App->renderer->DrawQuad({ pos.x, pos.y, 20, 30 }, 200, 100, 130, 255, true);
	

	return UPDATE_CONTINUE;
}



