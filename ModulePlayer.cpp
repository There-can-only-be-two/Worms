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

void ModulePlayer::Shoot() {

}

// Load assets
bool ModulePlayer::Start()
{
	isJumping = false;
	pBody.px = 100;
	pBody.py = 100;
	pBody.vx = 300;
	pBody.ay = GRAVITY*60;
	weaponType = 0;
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
		pBody.px -= pBody.vx*DELTATIME;
	}
	//Right
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		pBody.px += pBody.vx*DELTATIME;
	}
	//Jump
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		isJumping = true;
	}
	//Weapon 1
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		LOG("Selected GRENADE");
		weaponType = 0;
	}
	//Weapon 2
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		LOG("Selected ROCKET LAUNCHER");
		weaponType = 1;
	}
	//Shooting
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		switch (weaponType) {
		case 1:

		}

	}

	App->renderer->DrawQuad({ (int)pBody.px, (int)pBody.py, 20, 30 }, 200, 100, 130, 255, true);
	

	return UPDATE_CONTINUE;
}



