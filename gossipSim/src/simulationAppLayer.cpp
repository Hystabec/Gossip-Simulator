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

#define COLOUR_RELATIONS 1
#define USE_TEST_GOSSIP 1
#define TICK_ONCE_SECOND 1

SimLayer::SimLayer()
	: m_camController(1280.0f / 720.0f), m_npcManager("NPC_Data.xml")
{
	m_gossipManager.readGossipDataFile("Gossip_Data.xml");
	m_camController.setPosition({ 1.1f, -0.3f, 0.0f });
	m_camController.setZoomLevel(1.5f);
}

void SimLayer::attach()
{
	//const GS::npc::NPC& npcFound = m_npcManager.findNPC("Echo");
	//const_cast<GS::npc::NPC&>(npcFound).storeGossip(m_gossipManager.createGossip(GS::gossip::GossipType::negative, "Bravo", npcFound));
}

void SimLayer::detach()
{
}

void SimLayer::update(const daedalusCore::application::DeltaTime& dt)
{
	static float updateTime = 0.0f;
	updateTime += dt.getSeconds();

	m_mouseInBoundsThisFrame = false;
	m_camController.update(dt);

	m_gossipManager.tick(m_numNPCTicks);

	for (auto& npc : m_npcManager.getNPCVec())
	{
#if TICK_ONCE_SECOND
		if (updateTime >= secondsBetweenNPCUpdates)
		{
			npc.tick();
		}
#else
		npc.tick():
#endif
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

#if TICK_ONCE_SECOND
	if (updateTime >= secondsBetweenNPCUpdates)
	{
		m_numNPCTicks++;
		updateTime = 0.0f;
	}
#else
	m_numNPCTicks++;
#endif

	//set colour for relations
#if COLOUR_RELATIONS

	if (m_mouseInBoundsThisFrame)
		m_hoveredNPC->setRelationColours();

	if (!m_mouseInBoundsThisFrame && m_hoveredNPC)
	{
		m_hoveredNPC->setRelationColours(true);
		m_hoveredNPC = nullptr;
	}

#endif
#if USE_TEST_GOSSIP

	if (m_selectedGossip != 0)
	{
		auto gossipNPCVec = m_gossipManager.getNPCsHeardGossip(m_selectedGossip);

		for (auto npc : gossipNPCVec)
			const_cast<GS::npc::NPC*>(npc)->setColour({ 0.6f, 0.0f, 0.7f, 1.0f });
	}

#endif

	if (daedalusCore::application::Input::getMouseButton(DD_INPUT_MOUSE_BUTTON_1) && m_mouseInBoundsThisFrame)
		m_hoveredNPC->setPosition(m_camController.mouseToWorldPosition(daedalusCore::application::Input::getMousePosition()));

	daedalusCore::graphics::RenderCommands::setClearColour({0.14f, 0.14f, 0.14f, 1.0f});
	daedalusCore::graphics::RenderCommands::clear();

	daedalusCore::graphics::Renderer2D::begin(m_camController.getCamera());

	for (auto& npc : m_npcManager.getNPCVec())
		npc.render();

	daedalusCore::graphics::Renderer2D::end();
}

void SimLayer::imGuiRender()
{
	ImGui::DockSpaceOverViewport(0, 0, ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::Begin("NPC Details");
	char tickBuff[50];
	sprintf_s(tickBuff, "NPCs ticked %i time(s)", m_numNPCTicks);
	ImGui::Text(tickBuff);
	ImGui::Separator();
	if (m_mouseInBoundsThisFrame)
	{
		m_hoveredNPC->displayDataToImGui();
	}
	else
		ImGui::Text("No NPC Hovered");

	ImGui::End();


	ImGui::Begin("Gossip Selector");

	for (int i = 0; i < m_gossipManager.getNextGossipID(); i++)
	{
		char buff[50];
		if (i == 0)
			sprintf_s(buff, "None");
		else
			sprintf_s(buff, "Gossip: %i", i);

		if (ImGui::Selectable(buff, m_selectedGossip == i) && i != m_selectedGossip)
		{
			// reset colours back to default
			auto gossipNPCVec = m_gossipManager.getNPCsHeardGossip(m_selectedGossip);
			for (auto npc : gossipNPCVec)
				const_cast<GS::npc::NPC*>(npc)->setColour({ 1.0f, 1.0f, 1.0f, 1.0f });

			m_selectedGossip = i;
		}
	}

	ImGui::End();
}

void SimLayer::onEvent(daedalusCore::event::Event& e)
{
	m_camController.onEvent(e);
}