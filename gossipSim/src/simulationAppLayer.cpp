#include "simulationAppLayer.h"

#include "pugixml.hpp"
#include "imgui.h"

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
	int npcX = 0;
	int npcY = 0;
	for (pugi::xml_node node_NPC : npcDoc.child("listOfNPC"))
	{
		DD_LOG_INFO("NPC ({}) constucted", node_NPC.attribute("name").as_string());

		quadProps.position = { (float)npcX + -1.0f, (float)-npcY + 0.75f, 0.0f };

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

	daedalusCore::graphics::RenderCommands::setClearColour(daedalusCore::utils::colour_vec4_to_normalized_vec4({ 36,36,36,255 }));
	daedalusCore::graphics::RenderCommands::clear();

	daedalusCore::graphics::Renderer2D::begin(m_camController.getCamera());
	
	for (auto& npc : m_npcVec)
		npc.render();
	
	daedalusCore::graphics::Renderer2D::end();
}

void SimLayer::imGuiRender()
{
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
