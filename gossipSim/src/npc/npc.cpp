#include "npc.h"
#include "imgui.h"

#include "../simulationAppLayer.h"
#include "../mathsUtils/vec2Utils.h"

namespace GS { namespace npc {

	NPC::NPC()
		: m_name("NULL")
	{
	}

	NPC::NPC(const std::string& name, const daedalusCore::graphics::primatives2D::QuadProperties& renderProp)
		: m_name(name), m_renderProperties(renderProp)
	{
	}

	NPC::~NPC()
	{
		//Here to check object is being destroyed
		DD_LOG_INFO("NPC: {} destroyed", m_name);
	}

	void NPC::tick()
	{
	}

	void NPC::render()
	{
		daedalusCore::graphics::Renderer2D::drawQuad(m_renderProperties);
	}

	void NPC::renderRelations(void* sl)
	{
		DD_LOG_TRACE("Rendering {} relations", m_name);

		for (const auto& npcRel : m_relationMap)
		{
			daedalusCore::maths::vec2 asVec2Pos = { m_renderProperties.position.x, m_renderProperties.position.y };
			const GS::npc::NPC& otherNPC = ((SimLayer*)sl)->findNPC(npcRel.first);

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

} }