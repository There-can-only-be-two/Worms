#include "Application.h"

Application::Application()
{
	window		=	new ModuleWindow(this);
	textures	=	new ModuleTextures(this);
	input		=	new ModuleInput(this);
	audio		=	new ModuleAudio(this, true);	
	title		=   new ModuleTitle(this);
	player		=	new ModulePlayer(this, false);
	scene_intro =	new ModuleSceneIntro(this, false);
	physics		=	new ModulePhysics(this);
	fade		=	new ModuleFadeToBlack(this);
	fonts		=	new ModuleFonts(this);
	debug		=	new ModuleDebug(this, false);
	renderer	=	new ModuleRender(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order


	// Main Modules
	AddModule(window);
	AddModule(textures);
	AddModule(input);
	AddModule(audio);
	AddModule(fonts);
	AddModule(physics);

	// Scenes
	AddModule(title);
	AddModule(scene_intro);
	
	// Player
	AddModule(player);
	AddModule(fade);
	AddModule(debug);
	
	//render last!
	AddModule(renderer);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		if(item->data->IsEnabled())
			ret = item->data->Start();
		item = item->next;
	}
	
	return ret;
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if(item->data->IsEnabled())
			ret = item->data->PreUpdate();
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if(item->data->IsEnabled())
  			ret = item->data->Update();
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		if(item->data->IsEnabled())
			ret = item->data->PostUpdate();
		item = item->next;
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}

