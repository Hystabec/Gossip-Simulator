#include "simulationAppLayer.h"

#include "pugixml.hpp"
#include "imgui.h"
#include "mathsUtils/vec2Utils.h"

//GS::simCore gossipCore;

	////Example sudo API
	////gossipCore.getGossipManager().startGossip(/*GossipData*/);

	//bool loop = true;
	//while (loop)
	//{
	//	loop = gossipCore.update();
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//}

SimLayer::SimLayer()
	: m_camController(1280.0f / 720.0f)
{
}

void SimLayer::attach()
{
	auto circleTexture = daedalusCore::graphics::Texture2D::create("resources/circle.png");
	daedalusCore::graphics::primatives2D::QuadProperties quadProps;
	quadProps.size = { 0.25f };
	quadProps.texture = circleTexture;

	pugi::xml_document npcDoc;
	pugi::xml_parse_result result = npcDoc.load_file("NPC_Data.xml");

	if (!result)
		DD_ASSERT(false, "Couldnt find NPC_Data.xml");

	m_npcVec.reserve(std::distance(npcDoc.child("listOfNPC").begin(), npcDoc.child("listOfNPC").end()));

	//parseNodeFiles - generate nodes
	int npcX = 0, npcY = 0;
	int npcCount = 0;
	for (pugi::xml_node node_NPC : npcDoc.child("listOfNPC"))
	{
		DD_LOG_INFO("NPC ({}) constucted", node_NPC.attribute("name").as_string());

		quadProps.position = { (float)npcX + -1.0f, (float)-npcY + 0.75f, (float)npcCount * 0.01f };

		m_npcVec.emplace_back(node_NPC.attribute("name").as_string(), quadProps);
		GS::npc::NPC& curNPC = m_npcVec.back();

		//parseNodeFiles - generate relationships
		for (pugi::xml_node node_relation : node_NPC.child("relationships"))
			curNPC.addRelation(node_relation.attribute("npc").as_string(), node_relation.attribute("value").as_int());

		npcX++;
		if (npcX >= 3)
		{
			npcX = 0;
			npcY++;
		}
		npcCount++;
	}
}

void SimLayer::detach()
{
}

void SimLayer::update(const daedalusCore::application::DeltaTime& dt)
{
	m_mouseInBoundsThisFrame = false;
	m_camController.update(dt);

	for (auto& npc : m_npcVec)
	{
		npc.tick();
		//check if mouse is hovering an NPCs

		if (m_mouseInBoundsThisFrame)
			continue;
		

		if (npc.inPointInBounds(m_camController.mouseToWorldPosition(daedalusCore::application::Input::getMousePosition())))
		{
			m_mouseInBoundsThisFrame = true;
			m_hoveredNPC = &npc;
		}
	}

	if (daedalusCore::application::Input::getMouseButton(DD_INPUT_MOUSE_BUTTON_1) && m_mouseInBoundsThisFrame)
	{
		m_hoveredNPC->setPosition(m_camController.mouseToWorldPosition(daedalusCore::application::Input::getMousePosition()));
	}

#ifndef DD_DISTRO
	daedalusCore::graphics::Renderer2D::resetStats();
#endif
	daedalusCore::graphics::RenderCommands::setClearColour(daedalusCore::utils::colour_vec4_to_normalized_vec4({ 36,36,36,255 }));
	daedalusCore::graphics::RenderCommands::clear();

	daedalusCore::graphics::Renderer2D::begin(m_camController.getCamera());
	
	if (m_mouseInBoundsThisFrame)
	{
		for (const auto& npcRel : m_hoveredNPC->getRelationMap())
		{
			daedalusCore::maths::vec2 hovPos = m_hoveredNPC->getPosition();
			const GS::npc::NPC& otherNPC = findNPC(npcRel.first);

			if (otherNPC.getName() == "NULL")
				continue;

			daedalusCore::maths::vec2 otherPos = otherNPC.getPosition();
			daedalusCore::maths::vec2 diffrence = hovPos - otherPos;
			daedalusCore::maths::vec2 centrePoint = (hovPos + otherPos) / 2;

			daedalusCore::graphics::Renderer2D::drawRotatedQuad(
				{
					{ centrePoint.x, centrePoint.y, -0.1f },
					{ ((float)abs(diffrence.x) + (float)abs(diffrence.y)) , relationLineWidth},
					mathsUtils::angle_of_vec2(diffrence),
					npcRel.second >= 0 ? positiveRelationColour : negativeRelationColour
				});
		}
	}

	for (auto& npc : m_npcVec)
		npc.render();

	daedalusCore::graphics::Renderer2D::end();
}

void SimLayer::imGuiRender()
{
#ifndef DD_DISTRO
	ImGui::Begin("Settings");

	auto stats = daedalusCore::graphics::Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());

	ImGui::End();
#endif

	ImGui::Begin("NPC Details");
	if (m_mouseInBoundsThisFrame)
	{
		m_hoveredNPC->displayDataToImGui();
	}
	else
		ImGui::Text("No NPC Hovered");

	ImGui::End();
}

void SimLayer::onEvent(daedalusCore::event::Event& e)
{
	m_camController.onEvent(e);
}

const GS::npc::NPC& SimLayer::findNPC(const std::string& name) const
{
	for (auto& npc : m_npcVec)
	{
		if (npc.getName() == name)
			return npc;
	}

	DD_LOG_WARN("NPC ({}) not found, NPC(NULL) returned", name);
	return GS::npc::NPC();
}
