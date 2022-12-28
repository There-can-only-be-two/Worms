#include "Application.h"
#include "ModuleDebug.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "ModuleFadeToBlack.h"

#include <string>
#include <chrono>
using namespace std::chrono;


ModuleDebug::ModuleDebug(Application* app, bool start_enabled) : Module(app, start_enabled) { drawDebug = false; }
ModuleDebug::~ModuleDebug() {}

bool ModuleDebug::Start()
{
	drawDebug = true;
	drawPhysics = true;
	variables = true;

	return true;
}

update_status ModuleDebug::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
		drawDebug = !drawDebug;


	if (drawDebug)
	{
		// F1: View colliders / logic
		if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
			drawPhysics = !drawPhysics;

		//F2: Lights ON/OFF
		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
			lightsON = !lightsON;

		//FPS
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && desiredFPS < 120)
			desiredFPS += 5;
		else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && desiredFPS > 10)
			desiredFPS -= 5;

		//F4: Sfx ON/OFF
		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
			sfxON = !sfxON;

		if (sfxON)
			Mix_Volume(-1, 32);
		else
			Mix_Volume(-1, 0);

		//U: Chnage UI
		if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
			changeUI = !changeUI;

		if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
			variables = !variables;
	}


	return UPDATE_CONTINUE;
}

update_status ModuleDebug::PostUpdate()
{
	if (drawDebug)
		DrawDebug();

	if (drawPhysics)
		DrawPhysics();

	return UPDATE_CONTINUE;
}

void ModuleDebug::DrawDebug()
{
	int fontId = 0;
	int debugX = 50;
	int debugY = 50;


	App->fonts->BlitText(debugX, debugY + 0, fontId, "#DEBUG MODE (TAB)  ON/OFF");

	//Colliders
	if (drawPhysics)
		App->fonts->BlitText(debugX, debugY + 20, fontId, "#COLLIDERS  (F1)   ON");
	else
		App->fonts->BlitText(debugX, debugY + 20, fontId, "#COLLIDERS  (F1)   OFF");

	//Lights
	if (lightsON)
		App->fonts->BlitText(debugX, debugY + 40, fontId, "#LIGHTS     (F2)   ON");
	else
		App->fonts->BlitText(debugX, debugY + 40, fontId, "#LIGHTS     (F2)   OFF");


	//Frames
	std::string string = std::string("#FRAMESPS   (Q-/E+)  ") + std::to_string(desiredFPS);
	App->fonts->BlitText(debugX, debugY + 80, fontId, string.c_str());

	//Gravity
	//string = std::string("#GRAVITY.Y  (S-/W+)  ") + std::to_string((int)-App->physics->gravity);
	//App->fonts->BlitText(debugX, debugY + 100, fontId, string.c_str());

	////Bounce coef
	//string = std::string("#BOUNCE COEF(A-/D+)  ") + std::to_string(App->scene_intro->ball->intensity);
	//App->fonts->BlitText(debugX, debugY + 120, fontId, string.c_str());



	//Change UI
	if (changeUI)
		App->fonts->BlitText(debugX, debugY + 160, fontId, "#CHANGE UI  (U)    ON");
	else
		App->fonts->BlitText(debugX, debugY + 160, fontId, "#CHANGE UI  (U)    OFF");

	//Mute SFX
	if (sfxON)
		App->fonts->BlitText(debugX, debugY + 180, fontId, "#SFX        (M)    ON");
	else
		App->fonts->BlitText(debugX, debugY + 180, fontId, "#SFX        (M)    OFF");

	//Variables
	if (variables)
		App->fonts->BlitText(debugX, debugY + 200, fontId, "#VARIABLES  (V)    ON");
	else
		App->fonts->BlitText(debugX, debugY + 200, fontId, "#VARIABLES  (V)    OFF");


	if (variables)
	{
		//Last pBody on List
		p2List_item<PhysBody*>* item;
		PhysBody* pBody = NULL;

		item = App->physics->listBodies.getLast();

		if (item != NULL)
		{
			pBody = item->data;

			std::string string = std::string("PBODY.AX = ") + std::to_string(pBody->ax);
			App->fonts->BlitText(debugX + 16, debugY + 220, fontId, string.c_str());

			string = std::string("PBODY.AY = ") + std::to_string(pBody->ay);
			App->fonts->BlitText(debugX + 16, debugY + 240, fontId, string.c_str());

			string = std::string("PBODY.VX = ") + std::to_string(pBody->vx);
			App->fonts->BlitText(debugX + 16, debugY + 260, fontId, string.c_str());

			string = std::string("PBODY.VY = ") + std::to_string(pBody->vy);
			App->fonts->BlitText(debugX + 16, debugY + 280, fontId, string.c_str());

			string = std::string("PBODY.PX = ") + std::to_string(pBody->px);
			App->fonts->BlitText(debugX + 16, debugY + 300, fontId, string.c_str());

			string = std::string("PBODY.PY = ") + std::to_string(pBody->py);
			App->fonts->BlitText(debugX + 16, debugY + 320, fontId, string.c_str());


			string = std::string("PLAYER.PX = ") + std::to_string(App->player->pBody.px);
			App->fonts->BlitText(debugX + 16, debugY + 360, fontId, string.c_str());

			string = std::string("PLAYER.PY = ") + std::to_string(App->player->pBody.py);
			App->fonts->BlitText(debugX + 16, debugY + 380, fontId, string.c_str());
		}


		//Spring force
		/*if (App->scene_intro->springForce == 420)
			string = std::string("SPRING.F = ") + std::to_string(App->scene_intro->springForce) + "  X)";
		else
			string = std::string("SPRING.F = ") + std::to_string(App->scene_intro->springForce);
		App->fonts->BlitText(debugX + 16, debugY + 260, fontId, string.c_str());*/
	}
}

void ModuleDebug::DrawPhysics()
{
	//Shooting angle
	double cosinus = METERS_TO_PIXELS(100 * cos(App->player->shootAngle * DEGTORAD));
	double sinus = METERS_TO_PIXELS(100 * sin(App->player->shootAngle * DEGTORAD));
	double playerX = METERS_TO_PIXELS(App->player->pBody.px);
	double playerY = METERS_TO_PIXELS(App->player->pBody.py);
	App->renderer->DrawLine(playerX, playerY, playerX + cosinus, playerY - sinus, 255, 165, 0, 255, false);

	//Physics bodies
	p2List_item<PhysBody*>* item;
	PhysBody* pBody = NULL;

	for (item = App->physics->listBodies.getFirst(); item != NULL; item = item->next)
	{
		pBody = item->data;

		App->renderer->DrawCircle(METERS_TO_PIXELS(pBody->px), METERS_TO_PIXELS(pBody->py), METERS_TO_PIXELS(pBody->radius), 255, 255, 255);
	}
}