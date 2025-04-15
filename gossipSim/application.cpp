#include <Daedalus.h>
#include <entryPoint.h>

#include "src/simulationAppLayer.h"

class GossipSim_App : public daedalusCore::Application
{
public:
	GossipSim_App()
		: Application("Gossip Sim", 1280, 720, false)
	{
		this->pushLayer(new SimLayer());
	}

	~GossipSim_App()
	{
	}	
};

daedalusCore::Application* daedalusCore::createApplication()
{
	return new GossipSim_App();
}