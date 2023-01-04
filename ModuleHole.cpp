#include "Globals.h"
#include "Application.h"
#include "ModuleHole.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"


// Constructor
ModuleHole::ModuleHole(Application* app, bool start_enabled) : Module(app, start_enabled)
{}
// Destructor
ModuleHole::~ModuleHole()
{}

bool ModuleHole::Start()
{
	juan = App->textures->Load("/Game/Assets/Controls.png");
	LOG("Loading holes");
	
		
	return true;
}

update_status ModuleHole::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT)
	{
		App->renderer->Blit(juan,100,100);
	}
	
	return UPDATE_CONTINUE;
}

bool ModuleHole::CleanUp()
{
	LOG("Unloading player");
	return true;
}
