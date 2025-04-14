#include "simulationCore.h"

#include "pugixml.hpp"

namespace GS {

	simCore::simCore()
	{
		pugi::xml_document npcDoc;
		pugi::xml_parse_result result = npcDoc.load_file("NPC_Data.xml");

		if (!result)
			DD_ASSERT(false, "Couldnt find NPC_Data.xml");

		m_npcVec.reserve(std::distance(npcDoc.child("listOfNPC").begin(), npcDoc.child("listOfNPC").end()));

		//parseNodeFiles - generate nodes
		for (pugi::xml_node node_NPC : npcDoc.child("listOfNPC"))
		{
			DD_LOG_INFO("NPC ({}) constucted", node_NPC.attribute("name").as_string());
			m_npcVec.emplace_back(node_NPC.attribute("name").as_string());
			npc::NPC& curNPC = m_npcVec.back();

			//parseNodeFiles - generate relationships
			for (pugi::xml_node node_relation : node_NPC.child("relationships"))
			{
				curNPC.addRelation(node_relation.attribute("npc").as_string(), node_relation.attribute("value").as_int());
			}
		}

		//m_gossipManager = std::make_unique<gossip::GossipManager>(m_npcVec);

		m_circleTexture = daedalusCore::graphics::Texture2D::create("resources/circle.png");
	}

	simCore::~simCore()
	{
		/*for (auto npc : m_npcVec)
		{
			delete npc;
			npc = nullptr;
		}*/

		m_npcVec.clear();
	}

	bool simCore::update()
	{
		for (auto& npc : m_npcVec)
			npc.tick();

		updateCount++;
		return true;
	}

	void simCore::renderNPCs()
	{
		for (int i = 0; i < m_npcVec.size(); i++)
		{
			daedalusCore::graphics::Renderer2D::drawQuad({ { (float)i, 0 }, { 0.5f, 0.5f }, m_circleTexture, { 1.0f, 1.0f, 1.0f, 1.0f } });
		}
	}

	const npc::NPC& simCore::findNPC(const std::string& name) const
	{
		for (auto& npc : m_npcVec)
		{			
			if (npc.getName() == name)
				return npc;
		}

		DD_LOG_WARN("NPC ({}) not found, NPC(NULL) returned", name);
		return npc::NPC("NULL");
	}

}
