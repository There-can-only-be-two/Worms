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

		PhysBody* bod = new PhysBody();

		bod->ax = 0;
		bod->ay = 0;

		bod->vx = 0;
		bod->vy = 0;

		bod->px = pBody.px;
		bod->py = pBody.py;

		bod->isAlive = true;
		bod->isStable = false;

	switch (weaponType) {
	case 0:
		bod->label = GRENADE;
		break;
	case 1:
		bod->label = MISSILE;
		break;
	default:
		break;
	}

	App->physics->listBodies.add(bod);
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
	shootAngle = 180;
	shootForce = 50;
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
	//Weapons
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		LOG("Selected GRENADE");
		weaponType = 0;
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		LOG("Selected ROCKET LAUNCHER");
		weaponType = 1;
	}
	//Angle
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
		if (shootAngle < 180) { shootAngle += 1; }
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
		if (shootAngle > 0) { shootAngle -= 1; }
	}
	//Force
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		if (shootForce < 100) { shootForce += 1; }
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		if (shootForce > 0) { shootForce -= 1; }
	}
	//Shooting
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		Shoot();
	}

	App->renderer->DrawQuad({ (int)pBody.px, (int)pBody.py, 20, 30 }, 200, 100, 130, 255, true);
	

	return UPDATE_CONTINUE;
}



