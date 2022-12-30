#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "math.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"

struct Object
{
	SDL_Texture* graphic;
	uint fx;

	Object() : graphic(NULL)
	{}
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void Shoot(Circle* player);

public:
	
	p2List<Circle*> listPlayers;
	Circle* pBody;
	uint weaponType;

	uint explosionTimer;
	bool isShootingGrenade;
	bool isShootingMissile;

	int speed;

	float coef_rest_player, coef_rest_grenade;
	float cd_player, cd_grenade;
	float b_player, b_grenade;
};