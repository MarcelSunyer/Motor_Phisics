#ifndef __ModuleHole_H__
#define __ModuleHole_H__

#include "Module.h"
#include "Globals.h"
#include "p2List.h"
#include "p2Point.h"

class ModuleHole : public Module
{
public:

	ModuleHole(Application* app, bool start_enabled = true);
	
	~ModuleHole();
	
	bool Start();
	update_status Update();
	bool CleanUp();
	void Randomizer(int x);

public:
	SDL_Texture* juans;
	SDL_Texture* victoria;
	SDL_Rect juan[3];
	int random = 1;
	bool tocado = false;
};


#endif 