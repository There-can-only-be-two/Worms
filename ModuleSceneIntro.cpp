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
	App->player->Enable();
	App->scene_intro->Enable();
	App->debug->Enable();

	App->renderer->camera.x = App->renderer->camera.y = 0;

	ground = App->physics->CreateGround(400, 600, 300, 300);
	listGrounds.add(ground);

	ground2 = App->physics->CreateGround(400, 0, 100, 600);
	listGrounds.add(ground2);

	ground3 = App->physics->CreateGround(1200, 500, 400, 400);
	listGrounds.add(ground3);

	water = App->physics->CreateWater(700, 620, 500, 200);
	atm = App->physics->CreateAtmosphere();
	//enemy = App->physics->CreateEnemy(1000, 200, 100, 100);

	explosion = new Explosion();

	explosion->radius = 2.5;
	explosion->stepIterator = 10;
	
	turn = PLAYER_1;

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

	if (explosion->steps > 0 && App->player->explosionTimer <= 0 && (App->player->isShootingGrenade == true || App->player->isShootingMissile == true)) {
		App->renderer->DrawCircle(METERS_TO_PIXELS(explosion->x), METERS_TO_PIXELS(explosion->y), METERS_TO_PIXELS(explosion->radius) - METERS_TO_PIXELS(explosion->radius*(explosion->steps/explosion->stepIterator)), 255, 128, 0, 255);
		explosion->steps--;
		if (explosion->steps == 0) {
			App->player->isShootingGrenade = false;
			App->player->isShootingMissile = false;
		}
	}

	return UPDATE_CONTINUE;
}

// PostUpdate: draw fonts
update_status ModuleSceneIntro::PostUpdate()
{

	SDL_Rect gRect = { METERS_TO_PIXELS(ground->x), METERS_TO_PIXELS(ground->y), METERS_TO_PIXELS(ground->w), METERS_TO_PIXELS(ground->h) };
	SDL_Rect g2Rect = { METERS_TO_PIXELS(ground2->x), METERS_TO_PIXELS(ground2->y), METERS_TO_PIXELS(ground2->w), METERS_TO_PIXELS(ground2->h) };
	SDL_Rect g3Rect = { METERS_TO_PIXELS(ground3->x), METERS_TO_PIXELS(ground3->y), METERS_TO_PIXELS(ground3->w), METERS_TO_PIXELS(ground3->h) };
	SDL_Rect wRect = { METERS_TO_PIXELS(water->x), METERS_TO_PIXELS(water->y), METERS_TO_PIXELS(water->w), METERS_TO_PIXELS(water->h) };
	//SDL_Rect eRect = { METERS_TO_PIXELS(enemy->x), METERS_TO_PIXELS(enemy->y), METERS_TO_PIXELS(enemy->w), METERS_TO_PIXELS(enemy->h) };

	App->renderer->DrawQuad(gRect, 101, 67, 33, 128);
	App->renderer->DrawQuad(g2Rect, 101, 67, 33, 128);
	App->renderer->DrawQuad(g3Rect, 101, 67, 33, 128);
	App->renderer->DrawQuad(wRect, 135, 205, 235, 128);
	//App->renderer->DrawQuad(eRect, 223, 255, 0, 128);

	return UPDATE_CONTINUE;
}
