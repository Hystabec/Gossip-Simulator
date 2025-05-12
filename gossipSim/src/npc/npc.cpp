#include "npc.h"
#include "imgui.h"

#include "../simulationAppLayer.h"
#include "../mathsUtils/vec2Utils.h"
#include "npcManager.h"
#include "../gossip/gossipManager.h"

namespace GS::npc {

	NPC::NPC()
		: m_name("NULL")
	{
		//DD_LOG_INFO("NPC ({}) constucted", m_name);
	}

	NPC::NPC(const std::string& name, const daedalusCore::graphics::primatives2D::QuadProperties& renderProp)
		: m_name(name), m_renderProperties(renderProp)
	{
		//DD_LOG_INFO("NPC ({}) constucted", m_name);
	}

	NPC::~NPC()
	{
		//Here to check object is being destroyed
		//DD_LOG_INFO("NPC: ({}) destroyed", m_name);
	}

	void NPC::tick()
	{
		if (m_storedGossip != 0 && m_relationMap.size() > 0)
		{
			auto& firstRelation = NPCManager::get().findNPC(m_relationMap.begin()->first);
			const_cast<NPC&>(firstRelation).listenToGossip(m_storedGossip);
			DD_LOG_INFO("{} told {} about [gossipID]{}", m_name, firstRelation.getName(), m_storedGossip);
		}
	}

	void NPC::setColour(const daedalusCore::maths::vec4& colour)
	{
		m_renderProperties.colour = colour;
	}

	void NPC::render()
	{
		daedalusCore::graphics::Renderer2D::drawQuad(m_renderProperties);
	}

	void NPC::setRelationColours(bool revertToDefault)
	{
		if (revertToDefault)
			this->setColour({ 1,1,1,1 });
		else
			this->setColour({ 0.8f, 0.8f, 0.8f, 0.8f });

		for (const auto& npcRel : m_relationMap)
		{
			//This const_cast is a little goffy but its how i can get around findNPC returning a const ref

			if (revertToDefault)
				const_cast<NPC&>(NPCManager::get().findNPC(npcRel.first)).setColour({1, 1, 1, 1});
			else
				const_cast<NPC&>(NPCManager::get().findNPC(npcRel.first)).setColour(npcRel.second >= 0 ? positiveRelationColour : negativeRelationColour);
		}
	}

	void NPC::renderRelations()
	{
		DD_LOG_TRACE("Rendering {} relations", m_name);

		for (const auto& npcRel : m_relationMap)
		{
			daedalusCore::maths::vec2 asVec2Pos = { m_renderProperties.position.x, m_renderProperties.position.y };
			const GS::npc::NPC& otherNPC = NPCManager::get().findNPC(npcRel.first);

			if (otherNPC.getName() == "NULL")
				continue;

			daedalusCore::maths::vec2 otherPos = otherNPC.getPosition();
			daedalusCore::maths::vec2 diffrence = asVec2Pos - otherPos;
			daedalusCore::maths::vec2 centrePoint = (asVec2Pos + otherPos) / 2;

			daedalusCore::graphics::Renderer2D::drawRotatedQuad(
				{
					{ centrePoint.x, centrePoint.y, -0.1f },
					{ ((float)abs(diffrence.x) + (float)abs(diffrence.y)) , relationLineWidth},
					mathsUtils::angle_of_vec2(diffrence),
					npcRel.second >= 0 ? positiveRelationColour : negativeRelationColour
				});
		}
	}	

	void NPC::listenToGossip(uint32_t gossipID)
	{
		gossip::GossipManager::get().registerGossipListener(gossipID, this);

		gossip::Gossip gossipInstance = gossip::GossipManager::get().getGossipFromID(gossipID);

		// "The gossip is about me, so ill ignore it"
		if (gossipInstance.aboutNPC == m_name)
			return;

		auto asRelation = m_relationMap.find(gossipInstance.aboutNPC);
		if (asRelation != m_relationMap.end())
		{
			if (asRelation->second > 0) // "The gossip is about someone i like, so ill ignore it"
				return;
			else if (asRelation->second == 0) // "The gossip is about someone i dont have an opinion, so ill ignore it"
				return;
			else // "The gossip is about someone i dont like so ill remeber it"
			{
				m_storedGossip = gossipID;
			}
		}

		// "The gossip is about someone i dont know, so ill ignore it"
		return;
	}

	static int clamp_relation_value(int val)
	{
		return std::clamp(val, -100, 100);
	}

	void NPC::addRelation(const std::string& npc, int relationValue)
	{
		if (!npcExsitsInMap(npc))
		{
			m_relationMap[npc] = clamp_relation_value(relationValue);
		}
	}

	void NPC::updateRelation(const std::string& npc, int relationValue)
	{
		if (npcExsitsInMap(npc))
		{
			m_relationMap[npc] = clamp_relation_value(relationValue);
		}
	}

	int NPC::checkRelation(const std::string& npc, bool* checkBool)
	{
		if (npcExsitsInMap(npc))
		{
			if (checkBool)
				*checkBool = true;

			return m_relationMap[npc];
		}

		if(checkBool)
			*checkBool = false;

		return 0;
	}

	void NPC::removeRelation(const std::string& npc)
	{
		if (npcExsitsInMap(npc))
		{
			m_relationMap.erase(npc);
		}
	}

	void NPC::clearRelations()
	{
		m_relationMap.clear();
	}

	bool NPC::inPointInBounds(const daedalusCore::maths::vec2& pointPos)
	{
		daedalusCore::maths::vec2 dif = daedalusCore::maths::vec2(m_renderProperties.position.x, m_renderProperties.position.y) - pointPos;

		if ((float)(abs(dif.x) + (float)abs(dif.y)) <= m_renderProperties.size.x)
			return true;

		return false;
	}

	void NPC::displayDataToImGui()
	{
		ImGui::Text("NPC Name: %s", m_name.c_str());
		ImGui::SeparatorText("Relations");
		for (auto relation : m_relationMap)
			ImGui::Text("NPC: %s, Value: %i", relation.first.c_str(), relation.second);
	}

	bool NPC::npcExsitsInMap(const std::string& npc)
	{
		return m_relationMap.find(npc) != m_relationMap.end() ? true : false;
	}

}