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
		// this is here so all gossip ticks dont mostly happen on one tick
		if (m_toldGossipThisTick)
		{
			m_toldGossipThisTick = false;
			return;
		}

		if (m_personality == personality::gossipSink) // gossip sinks - like hearing gossip but dont tell it
			return;

		if (!m_toldRecentGossip && m_relationMap.size() > 0)
		{
			DD_LOG_TRACE("{} started telling gossip", m_name);

			gossip::Gossip storedGossip = gossip::GossipManager::get().getGossipFromID(m_storedGossips.front());

			for (auto& npcRel : m_relationMap)
			{
				if (npcRel.second < 1 && m_personality != personality::gossipSpreader) // gossip spreaders will tell everyone they know not just people they like
					continue;

				bool NPCFoundBool;
				auto& asNPC = NPCManager::get().findNPC(npcRel.first, &NPCFoundBool);
				if (NPCFoundBool)
				{
					DD_LOG_INFO("{} told {} gossip | gossipID = [{}]", m_name, asNPC.getName(), storedGossip.fileID);
					const_cast<NPC&>(asNPC).listenToGossip(m_storedGossips.front(), this);
				}
				else
					DD_LOG_ERROR("{} couldn't tell gossip | NPC {} not found", m_name, npcRel.first);
			}

			m_toldRecentGossip = true;
		}
	}

	void NPC::reset()
	{
		m_storedGossips.clear();
		m_toldRecentGossip = true;
		m_toldGossipThisTick = false;
		this->setColour({ 1,1,1,1 });
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
				const_cast<NPC&>(NPCManager::get().findNPC(npcRel.first)).setColour({ 1, 1, 1, 1 });
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
					{ ((float)abs(mathsUtils::vec2_magnitude(diffrence))) , relationLineWidth},
					mathsUtils::angle_of_vec2(diffrence),
					npcRel.second >= 0 ? positiveRelationColour : negativeRelationColour
				});
		}
	}	

	bool foward_list_contains(const std::forward_list<uint32_t>& fl, uint32_t toSearchFor)
	{
		// TO DO: this is O(n) time could probably be optimized using set/u_set
		// if preformance becomes an issue check this

		for (auto i : fl)
		{
			if (i == toSearchFor)
				return true;
		}

		return false;
	}

	void NPC::listenToGossip(uint32_t gossipID, const NPC* teller)
	{
		gossip::GossipManager::get().registerGossipListener(gossipID, this);

		gossip::Gossip gossipInstance = gossip::GossipManager::get().getGossipFromID(gossipID);

		//already heard and process gossips (stops looping)
		if (foward_list_contains(m_storedGossips, gossipID))
		{
			DD_LOG_INFO("{} ignored gossip about {} | reason = 'already heard it' | gossipID = [{}]", m_name, gossipInstance.aboutNPC, gossipInstance.fileID);
			gossip::GossipManager::get().registerGossipEvent(gossipID, this, teller, false, "Already heard");
			return;
		}

		// "The gossip is about me, so ill ignore it"
		if (gossipInstance.aboutNPC == m_name)
		{
			DD_LOG_INFO("{} ignored gossip about {} | reason = 'about me' | gossipID = [{}]", m_name, gossipInstance.aboutNPC, gossipInstance.fileID);
			gossip::GossipManager::get().registerGossipEvent(gossipID, this, teller, false, "About me");
			return;
		}

		if (m_personality == personality::gossipSink)
		{
			m_storedGossips.push_front(gossipID);
			m_toldRecentGossip = false;
			m_toldGossipThisTick = true;
			DD_LOG_INFO("{} remembered {} gossip about {} | reason = 'im a gossip sink' | gossipID = [{}]", m_name, gossip::gossip_to_string(gossipInstance.type), gossipInstance.aboutNPC, gossipInstance.fileID);
			gossip::GossipManager::get().registerGossipEvent(gossipID, this, teller, true, "I'm a gossip sink");
			return;
		}
		else if (m_personality == personality::gossipSpreader)
		{
			m_storedGossips.push_front(gossipID);
			m_toldRecentGossip = false;
			m_toldGossipThisTick = true;
			DD_LOG_INFO("{} remembered {} gossip about {} | reason = 'im a gossip spreader' | gossipID = [{}]", m_name, gossip::gossip_to_string(gossipInstance.type), gossipInstance.aboutNPC, gossipInstance.fileID);
			gossip::GossipManager::get().registerGossipEvent(gossipID, this, teller, true, "I'm a gossip spreader");
			return;
		}


		auto asRelation = m_relationMap.find(gossipInstance.aboutNPC);
		if (asRelation != m_relationMap.end())
		{
			if (asRelation->second > 0) // "The gossip is about someone i like"
			{
				if (gossipInstance.type != gossip::GossipType::negative)
				{
					// "positive or neutral gossip about someone i like - ill remeber it"

					m_storedGossips.push_front(gossipID);
					m_toldRecentGossip = false;
					m_toldGossipThisTick = true;
					DD_LOG_INFO("{} remembered {} gossip about {} | reason = 'about someone i like' | gossipID = [{}]", m_name, gossip::gossip_to_string(gossipInstance.type), gossipInstance.aboutNPC, gossipInstance.fileID);
					gossip::GossipManager::get().registerGossipEvent(gossipID, this, teller, true, "About someone I like");
					return;
				}
				else
				{
					// "negative gossip about someone i like - ill ignore it"

					DD_LOG_INFO("{} ignored negative gossip about {} | reason = 'about someone i like' | gossipID = [{}]", m_name, gossipInstance.aboutNPC, gossipInstance.fileID);
					gossip::GossipManager::get().registerGossipEvent(gossipID, this, teller, false, "About someone I like");
					return;
				}
			}
			else if (asRelation->second < 0) // "The gossip is about someone i dont like so ill remeber it"
			{
				if (gossipInstance.type == gossip::GossipType::negative)
				{
					// "negative gossip about someone i dont like - ill remeber it"

					m_storedGossips.push_front(gossipID);
					m_toldRecentGossip = false;
					m_toldGossipThisTick = true;
					DD_LOG_INFO("{} remembered negative gossip about {} | reason = 'about someone i dont like' | gossipID = [{}]", m_name, gossipInstance.aboutNPC, gossipInstance.fileID);
					gossip::GossipManager::get().registerGossipEvent(gossipID, this, teller, true, "About someone I don't like");
					return;
				}
				else
				{
					// "positive or neutral gossip about someone i dont like - ill ignore it"

					DD_LOG_INFO("{} ignored {} gossip about {} | reason = 'about someone i dont like' | gossipID = [{}]", m_name, gossip::gossip_to_string(gossipInstance.type), gossipInstance.aboutNPC, gossipInstance.fileID);
					gossip::GossipManager::get().registerGossipEvent(gossipID, this, teller, false, "About someone I don't like");
					return;
				}
			}
			else // "The gossip is about someone i dont have an opinion, so ill ignore it"
			{
				DD_LOG_INFO("{} ignored {} gossip about {} | reason = 'about someone i dont have an opinion of' | gossipID = [{}]", m_name, gossip::gossip_to_string(gossipInstance.type), gossipInstance.aboutNPC, gossipInstance.fileID);
				gossip::GossipManager::get().registerGossipEvent(gossipID, this, teller, false, "About someone I don't have an opinion of");
				return;
			}
		}

		// "The gossip is about someone i dont know, so ill ignore it"
		DD_LOG_INFO("{} ignored {} gossip about {} | reason = 'about someone i dont know' | gossipID = [{}]", m_name, gossip::gossip_to_string(gossipInstance.type), gossipInstance.aboutNPC, gossipInstance.fileID);
		gossip::GossipManager::get().registerGossipEvent(gossipID, this, teller, false, "About someone I don't know");
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
		ImGui::SeparatorText("Hovered NPC Details");
		ImGui::Text("NPC Name: %s", m_name.c_str());
		ImGui::Text("Personality: %s", personality_to_string(m_personality).c_str());
		ImGui::Separator();
		if (ImGui::BeginTable("relationshioTable", 2, ImGuiTableFlags_Borders))
		{
			ImGui::TableSetupColumn("NPC");
			ImGui::TableSetupColumn("Relationship");
			ImGui::TableHeadersRow();

			for (auto relation : m_relationMap)
			{
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text(relation.first.c_str());

				ImGui::TableSetColumnIndex(1);
				ImGui::Text(relation.second > 0 ? "positve" : "negative");
			}

			ImGui::EndTable();
		}
	}

	bool NPC::npcExsitsInMap(const std::string& npc)
	{
		return m_relationMap.find(npc) != m_relationMap.end() ? true : false;
	}

}