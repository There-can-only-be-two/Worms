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
	void Shoot();

public:
	
	Circle* pBody;
	bool isJumping;
	uint weaponType;
	int shootAngle;
	int shootForce;

};