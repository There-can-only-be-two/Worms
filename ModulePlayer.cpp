#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include <string>

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	this->App = app;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	coef_rest_grenade = 0.7f;
	cd_grenade = 0.4f;
	b_grenade = 10.0f;

	for (int i = 0; i < 2; i++)
	{
		pBody = new Circle();

		if (i == 0)
		{
			pBody->px = PIXELS_TO_METERS(600);
			pBody->py = PIXELS_TO_METERS(400);
			pBody->label = PLAYER_1;
		}
		else
		{
			pBody->px = PIXELS_TO_METERS(1400);
			pBody->py = PIXELS_TO_METERS(500);
			pBody->label = PLAYER_2;
		}

		pBody->vx = 0;
		pBody->vy = 0;
		pBody->ax = 0;
		pBody->ay = 0;
		speed = 6;

		pBody->isAlive = true;
		pBody->isStable = false;
		pBody->isGrounded = false;
		isShootingGrenade = false;
		isShootingMissile = false;

		pBody->mass = 30.0f; // [kg]
		pBody->surface = 1.0f; // [m^2]
		pBody->radius = 0.8f; // [m]
		pBody->cd = 0.4f; // [-]
		pBody->cl = 1.2f; // [-]
		pBody->b = 10.0f; // [...]
		pBody->coef_friction = 0.0f; // [-]
		pBody->coef_restitution = 0.0f; // [-]
		pBody->life = 100;

		weaponType = 0;
		pBody->shootAngle = 90;
		pBody->shootForce = 10;
		pBody->isJumping = 0;
		pBody->isGrounded = true;

		explosionTimer = 0;

		App->physics->listBodies.add(pBody);
		listPlayers.add(pBody);
	}
	
	LOG("Loading player");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	p2List_item<Circle*>* pItem;
	Circle* player = NULL;
	for (pItem = listPlayers.getFirst(); pItem != NULL; pItem = pItem->next)
	{
		player = pItem->data;
		player->px = 1000;
		player->py = 1000;
		player->label = DEAD;
	
	}
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	p2List_item<Circle*>* pItem;
	Circle* player = NULL;

	for (pItem = listPlayers.getFirst(); pItem != NULL; pItem = pItem->next)
	{
		player = pItem->data;

		player->coef_restitution = coef_rest_player;
		player->cd = cd_player;
		player->b = b_player;
		std::string string;

		if (player->label == App->scene_intro->turn)
		{
			if (player->label == PLAYER_1) {
				App->fonts->BlitText(METERS_TO_PIXELS(player->px - 30), METERS_TO_PIXELS(player->py - 80), App->fonts->selected, "PLAYER 1");
				string = std::to_string(player->life);
				App->fonts->BlitText(METERS_TO_PIXELS(player->px - 10), METERS_TO_PIXELS(player->py - 50), App->fonts->selected, string.c_str());

			}else{
				App->fonts->BlitText(METERS_TO_PIXELS(player->px - 30), METERS_TO_PIXELS(player->py - 80), App->fonts->selected, "PLAYER 2");
				string = std::to_string(player->life);
				App->fonts->BlitText(METERS_TO_PIXELS(player->px - 10), METERS_TO_PIXELS(player->py - 50), App->fonts->selected, string.c_str());
			}
			
			//Left
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
				player->px -= speed * DELTATIME;
			}
			//Right
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
				player->px += speed * DELTATIME;
			}
			//Jump
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
				
				if (player->isGrounded) {
					player->isJumping = 8;
					player->isGrounded = false;
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
				if (player->shootAngle < 180) { player->shootAngle += 1; }
			}
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
				if (player->shootAngle > 0) { player->shootAngle -= 1; }
			}
			//Force
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
				if (player->shootForce < 100) { player->shootForce += 1; }
			}
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
				if (player->shootForce > 0) { player->shootForce -= 1; }
			}
			//Shooting
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
				if (!isShootingGrenade && !isShootingMissile) {
					Shoot(player);
					if (App->scene_intro->turn == PLAYER_1) {
						App->scene_intro->turn = PLAYER_2;
					}
					else {
						App->scene_intro->turn = PLAYER_1;
					}
				}
				if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {
					player->px = PIXELS_TO_METERS(600);
					player->py = PIXELS_TO_METERS(400);
				}
				
			}
			if (player->life == 0) {
				//if (pBody->label == PLAYER_1)

				App->fonts->BlitText(700, 400, App->fonts->selected, "GAME OVER, CLICK ENTER TO RESTART");
				if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
					App->fade->FadeBlack((Module*)App->scene_intro, (Module*)App->scene_intro, 90);
					player->life = 100;
					player->px = 1000;
					player->py = 1000;
					player->label = DEAD;
					//listPlayers.del(*player);

				}
			}
		}
		else {
			if (player->label == PLAYER_1) {
				App->fonts->BlitText(METERS_TO_PIXELS(player->px - 30), METERS_TO_PIXELS(player->py - 80), App->fonts->font, "PLAYER 1");
				string = std::to_string(player->life);
				App->fonts->BlitText(METERS_TO_PIXELS(player->px - 10), METERS_TO_PIXELS(player->py - 50), App->fonts->font, string.c_str());
			}
			else {
				App->fonts->BlitText(METERS_TO_PIXELS(player->px - 30), METERS_TO_PIXELS(player->py - 80), App->fonts->font, "PLAYER 2");
				string = std::to_string(player->life);
				App->fonts->BlitText(METERS_TO_PIXELS(player->px - 10), METERS_TO_PIXELS(player->py - 50), App->fonts->font, string.c_str());
			}
		}

		
	}
		

	return UPDATE_CONTINUE;
}

void ModulePlayer::Shoot(Circle* player)
{

	Circle* bod = new Circle();

	bod->ax = 0;
	bod->ay = 0;

	bod->vx = player->shootForce * cos(player->shootAngle * DEGTORAD);
	bod->vy = -player->shootForce * sin(player->shootAngle * DEGTORAD);

	bod->px = player->px;
	bod->py = player->py;

	bod->isAlive = true;
	bod->isStable = false;

	bod->mass = 10.0f; // [kg]
	bod->surface = 1.0f; // [m^2]
	bod->radius = 0.4f; // [m]
	bod->cd = cd_grenade; // [-]
	bod->cl = 1.2f; // [-]
	bod->b = b_grenade; // [...]
	bod->coef_friction = 0.9f; // [-]
	bod->coef_restitution = coef_rest_grenade; // [-]

	App->scene_intro->explosion->steps = App->scene_intro->explosion->stepIterator;

	bod->label = GRENADE;

	shootingIFrames = 10;

	switch (weaponType) {
	case 0:
		bod->label = GRENADE;
		isShootingGrenade = true;
		explosionTimer = 5 / DELTATIME;
		break;
	case 1:
		bod->label = MISSILE;
		isShootingMissile = true;
		explosionTimer = 5 / DELTATIME;
		break;
	default:
		break;
	}

	App->physics->listBodies.add(bod);
}