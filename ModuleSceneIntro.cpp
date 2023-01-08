#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	startSprite = App->textures->Load("Assets/Start.png");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	start = true;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	App->textures->Unload(startSprite);

	return true;
}
// Update: draw background
update_status ModuleSceneIntro::Update()
{

	if (start) {

		App->renderer->Blit(startSprite, 0, 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN ) {

		//Borrar el PRESS ENTER TO START
		start = false;
	

	}

	


	
	

	return UPDATE_CONTINUE;
}
