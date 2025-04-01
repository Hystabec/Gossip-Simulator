#include "simulationCore.h"

namespace GS {

	simCore::simCore()
	{
		//parseNodeFiles
		int npcCount = 3;
		for (int i = 0; i < npcCount; i++)
		{
			m_npcVec.push_back(std::make_shared<npc::NPC>(std::to_string(i)));
		}
	}

	bool simCore::update()
	{
		for (auto& npc : m_npcVec)
			npc->tick();

		return true;
	}

}
