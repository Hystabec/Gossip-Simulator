#include <Daedalus.h>
#include <entryPoint.h>

#include "src/appLayer.h"

class GossipSim_App : public daedalusCore::Application
{
public:
	GossipSim_App()
		: Application("NPC and Gossip Editor", 1280, 720, false)
	{
		this->pushLayer(new AppLayer());
	}

	~GossipSim_App()
	{
	}	
};

daedalusCore::Application* daedalusCore::createApplication()
{
	return new GossipSim_App();
}