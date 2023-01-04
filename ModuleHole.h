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

public:
	SDL_Texture* juan;
	int posiciones[3];
};


#endif 