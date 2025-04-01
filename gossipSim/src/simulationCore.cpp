#include "simulationCore.h"

namespace GS {

	int count = 0;

	simCore::simCore()
	{
		//parseNodeFiles
		int npcCount = 3;
		for (int i = 0; i < npcCount; i++)
		{
			m_npcVec.push_back(std::make_shared<npc::NPC>(std::to_string(i)));
		}

		for (auto& npc : m_npcVec)
		{
			for (auto& npc2 : m_npcVec)
			{
				if (npc == npc2)
					continue;

				npc->addRelation(npc2, 0);
			}
		};
	}

	simCore::~simCore()
	{
		//remove internal shared ptr refs in npc relation map
		for (auto& npc : m_npcVec)
			npc->clearRelations();

		m_npcVec.clear();
	}

	bool simCore::update()
	{
		if (count == 5)
			return false;

		for (auto& npc : m_npcVec)
			npc->tick();

		count++;
		return true;
	}

}
