#include "simulationCore.h"

#include "pugixml.hpp"
#include "iostream"

namespace GS {

	simCore::simCore()
	{
		pugi::xml_document npcDoc;
		pugi::xml_parse_result result = npcDoc.load_file("NPC_Data.xml");
		if (!result)
			std::cout << "Couldnt find NPC_Data.xml\n";

		//parseNodeFiles - generate nodes
		for (pugi::xml_node node_NPC : npcDoc.child("listOfNPC"))
		{
			npc::NPC* curNPC = new npc::NPC(node_NPC.attribute("name").as_string());

			std::cout << node_NPC.attribute("name").as_string() << "\n";

			//parseNodeFiles - generate relationships
			for (pugi::xml_node node_relation : node_NPC.child("relationships"))
			{
				curNPC->addRelation(node_relation.attribute("npc").as_string(), node_relation.attribute("value").as_int());
				std::cout << node_relation.attribute("npc").as_string() << " " << node_relation.attribute("value").as_int() << "\n";
			}

			m_npcVec.push_back(curNPC);
		}

		//m_gossipManager = std::make_unique<gossip::GossipManager>(m_npcVec);
	}

	simCore::~simCore()
	{
		for (auto npc : m_npcVec)
		{
			delete npc;
			npc = nullptr;
		}

		m_npcVec.clear();
	}

	bool simCore::update()
	{
		for (auto& npc : m_npcVec)
			npc->tick();

		updateCount++;
		return true;
	}

	const npc::NPC* simCore::findNPC(const std::string& name) const
	{
		for (auto& npc : m_npcVec)
		{			
			if (npc->getName() == name)
				return npc;
		}

		return nullptr;
	}

}
