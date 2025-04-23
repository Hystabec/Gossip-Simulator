#include "simulationAppLayer.h"

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
	: m_camController(1280.0f / 720.0f), m_npcManager("NPC_Data.xml")
{
}

void SimLayer::attach()
{
	
}

void SimLayer::detach()
{
}

void SimLayer::update(const daedalusCore::application::DeltaTime& dt)
{
	m_mouseInBoundsThisFrame = false;
	m_camController.update(dt);

	for (auto& npc : m_npcManager.getNPCVec())
	{
		npc.tick();
		//check if mouse is hovering an NPCs

		if (m_mouseInBoundsThisFrame)
			continue;

		if (npc.inPointInBounds(m_camController.mouseToWorldPosition(daedalusCore::application::Input::getMousePosition())))
		{
			if (m_hoveredNPC && m_hoveredNPC != &npc)
				m_hoveredNPC->setRelationColours(true);

			m_mouseInBoundsThisFrame = true;
			m_hoveredNPC = &npc;
		}
	}

	//set colour for relations
#if 1

	if (m_mouseInBoundsThisFrame)
		m_hoveredNPC->setRelationColours();

	if (!m_mouseInBoundsThisFrame && m_hoveredNPC)
	{
		m_hoveredNPC->setRelationColours(true);
		m_hoveredNPC = nullptr;
	}

#endif

	if (daedalusCore::application::Input::getMouseButton(DD_INPUT_MOUSE_BUTTON_1) && m_mouseInBoundsThisFrame)
	{
		m_hoveredNPC->setPosition(m_camController.mouseToWorldPosition(daedalusCore::application::Input::getMousePosition()));
	}

	daedalusCore::graphics::RenderCommands::setClearColour({0.14f, 0.14f, 0.14f, 1.0f});
	daedalusCore::graphics::RenderCommands::clear();

	daedalusCore::graphics::Renderer2D::begin(m_camController.getCamera());

	for (auto& npc : m_npcManager.getNPCVec())
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