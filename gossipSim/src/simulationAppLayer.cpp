#include "simulationAppLayer.h"

#include "simulationCore.h"

//GS::simCore gossipCore;

	////Example sudo API
	////gossipCore.getGossipManager().startGossip(/*GossipData*/);

	//bool loop = true;
	//while (loop)
	//{
	//	loop = gossipCore.update();
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//}

SimLayer::SimLayer()
{
}

void SimLayer::attach()
{
}

void SimLayer::detach()
{
}

void SimLayer::update(const daedalusCore::application::DeltaTime& dt)
{
	DD_LOG_INFO("Hello world");
}

void SimLayer::imGuiRender()
{
}

void SimLayer::onEvent(daedalusCore::event::Event& e)
{
}
