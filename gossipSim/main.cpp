#include "src/simulationCore.h"

#include <thread>

int main()
{
	GS::simCore gossipCore;

	//Example sudo API
	//gossipCore.getGossipManager().startGossip(/*GossipData*/);

	bool loop = true;
	while (loop)
	{
		loop = gossipCore.update();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}