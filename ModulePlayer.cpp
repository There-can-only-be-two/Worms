#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
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
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {

	}
	//Right
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {

	}
	//Up
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {

	}
	//Down
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {

	}

	return UPDATE_CONTINUE;
}



