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

		//Temp relation testing
		for (auto& npc : m_npcVec)
		{
			for (auto& npc2 : m_npcVec)
			{
				if (npc == npc2)
					continue;

				npc->addRelation(npc2, 0);
			}
		};

		m_gossipManager = std::make_unique<gossip::GossipManager>(m_npcVec);
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
		for (auto& npc : m_npcVec)
			npc->tick();

		updateCount++;
		return true;
	}

	const std::shared_ptr<npc::NPC>& simCore::findNPC(const std::string& name) const
	{
		for (auto& npc : m_npcVec)
		{
			if (npc->getName() == name)
				return npc;
		}

		return nullptr;
	}

}
