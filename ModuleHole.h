#ifndef __ModuleHole_H__
#define __ModuleHole_H__

#include "Module.h"
#include "Globals.h"

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
};


#endif 