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
	pBody = new Circle();
	pBody->label = PLAYER;
	isJumping = false;
	pBody->px = PIXELS_TO_METERS(600);
	pBody->py = PIXELS_TO_METERS(400);
	pBody->vx = 0;
	pBody->vy = 0;
	pBody->ax = 0;
	pBody->ay = 0;
	speed = 6;

	pBody->isAlive = true;
	pBody->isStable = false;
	grounded = false;

	pBody->mass = 10.0f; // [kg]
	pBody->surface = 1.0f; // [m^2]
	pBody->radius = 0.5f; // [m]
	pBody->cd = 0.4f; // [-]
	pBody->cl = 1.2f; // [-]
	pBody->b = 10.0f; // [...]
	pBody->coef_friction = 0.0f; // [-]
	pBody->coef_restitution = 0.0f; // [-]

	weaponType = 0;
	shootAngle = 90;
	shootForce = 10;
	isJumping = 0;

	App->physics->listBodies.add(pBody);

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
		pBody->px -= speed * DELTATIME;
	}
	//Right
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		pBody->px += speed * DELTATIME;
	}
	//Jump
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		if (grounded) {
			isJumping = 8;
			grounded = false;
		}
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
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		if (shootAngle < 180) { shootAngle += 1; }
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
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
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		Shoot();
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {
		pBody->px = PIXELS_TO_METERS(600);
		pBody->py = PIXELS_TO_METERS(400);
	}

	App->renderer->DrawQuad({ METERS_TO_PIXELS(pBody->px), METERS_TO_PIXELS(pBody->py), 20, 30 }, 200, 100, 130, 255, true);
	

	return UPDATE_CONTINUE;
}

void ModulePlayer::Shoot()
{
	Circle* bod = new Circle();

	bod->ax = 0;
	bod->ay = 0;

	bod->vx = shootForce * cos(shootAngle * DEGTORAD);
	bod->vy = -shootForce * sin(shootAngle * DEGTORAD);

	bod->px = pBody->px;
	bod->py = pBody->py;

	bod->isAlive = true;
	bod->isStable = false;

	bod->mass = 10.0f; // [kg]
	bod->surface = 1.0f; // [m^2]
	bod->radius = 0.5f; // [m]
	bod->cd = 0.4f; // [-]
	bod->cl = 1.2f; // [-]
	bod->b = 10.0f; // [...]
	bod->coef_friction = 0.9f; // [-]
	bod->coef_restitution = 0.7f; // [-]

	bod->label = GRENADE;

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