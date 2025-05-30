#include "npcManager.h"

#include <Daedalus.h>
#include "pugixml.hpp"

namespace GS::npc {

	NPCManager* NPCManager::s_instance = nullptr;

	static uint32_t npcPerRow = 4;

	NPCManager::NPCManager(const std::string& npcFile)
	{
		DD_ASSERT(!s_instance, "Duplicate NPCManager");
		s_instance = this;

		auto circleTexture = daedalusCore::graphics::Texture2D::create("resources/circle.png");
		daedalusCore::graphics::primatives2D::QuadProperties quadProps;
		quadProps.size = { 0.25f };
		quadProps.texture = circleTexture;

		pugi::xml_document npcDoc;
		pugi::xml_parse_result result = npcDoc.load_file(npcFile.c_str());

		if (!result)
			DD_ASSERT(false, DD_ASSERT_FORMAT_MESSAGE("Couldnt find {}", npcFile));

		m_npcVec.reserve(std::distance(npcDoc.child("listOfNPC").begin(), npcDoc.child("listOfNPC").end()));

		//parseNodeFiles - generate nodes
		int npcX = 0, npcY = 0;
		int npcCount = 0;
		for (pugi::xml_node node_NPC : npcDoc.child("listOfNPC"))
		{
			quadProps.position = { ((float)npcX * 0.8f) + -1.0f, (float)-npcY + 0.50f, (float)npcCount * 0.01f};

			m_npcVec.emplace_back(node_NPC.attribute("name").as_string(), quadProps);
			GS::npc::NPC& curNPC = m_npcVec.back();

			auto personalityNode = node_NPC.child("personality");
			if (personalityNode)
				curNPC.setPersonality(string_to_personality(personalityNode.attribute("type").as_string()));

			//parseNodeFiles - generate relationships
			for (pugi::xml_node node_relation : node_NPC.child("relationships"))
				curNPC.addRelation(node_relation.attribute("npc").as_string(), node_relation.attribute("value").as_int());

			npcX++;
			if (npcX >= npcPerRow)
			{
				npcX = 0;
				npcY++;
			}
			npcCount++;
		}
	}

	NPCManager::~NPCManager()
	{
		s_instance = nullptr;
	}

	const GS::npc::NPC& NPCManager::findNPC(const std::string& name, bool* found) const
	{
		for (auto& npc : m_npcVec)
		{
			if (npc.getName() == name)
			{
				if(found)
					*found = true;
				return npc;
			}
		}

		DD_LOG_WARN("NPC ({}) not found, NPC(NULL) returned", name);
		if (found)
			*found = false;
		return GS::npc::NPC();
	}

	void NPCManager::restart()
	{
		for (auto& npc : m_npcVec)
		{
			npc.reset();
		}
	}

}