#include "Globals.h"
#include "Application.h"
#include "ModuleHole.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "p2List.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include <time.h>


// Constructor
ModuleHole::ModuleHole(Application* app, bool start_enabled) : Module(app, start_enabled)
{}
// Destructor
ModuleHole::~ModuleHole()
{}

bool ModuleHole::Start()
{
	juans = App->textures->Load("Assets/Palo_golf.png");
	LOG("Loading holes");
	//Posicion_1 || Hacer una variable x y y para cada espacio de la array y que estos sean randoms.

	
	juan[0].x = 100, juan[0].y = 400, juan[0].w = 30, juan[0].h = 20;

	juan[1].x = 780, juan[1].y = 262, juan[1].w = 30, juan[1].h = 20;
	juan[2].x = 115, juan[2].y = 142, juan[2].w = 30, juan[2].h = 20;
	
	
	return true;
}

update_status ModuleHole::Update()
{
	if (tocado == true)
	{
		Randomizer(random);
		tocado = false;
	}
	

	App->renderer->DrawQuad(juan[random], 255, 255, 255);
	App->renderer->Blit(juans, juan[random].x - 17, juan[random].y - 70);
	
	return UPDATE_CONTINUE;
}

bool ModuleHole::CleanUp()
{
	LOG("Unloading player");
	return true;
}

void ModuleHole::Randomizer(int x)
{
	time_t times = 100;
	srand(time(&times));
	random = rand() % 3 + 0;
}

