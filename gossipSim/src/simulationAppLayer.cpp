#include "simulationAppLayer.h"

#include "imgui.h"
#include "mathsUtils/vec2Utils.h"
#include "graphics/arrow2D.h"

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

#define SHOW_GLITCH_ARROWS 0
#define MOVEABLE_NPC 0

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
	if(!m_userUpdatePaused)
		m_npcUpdateTime += dt.getSeconds();

	m_mouseInBoundsThisFrame = false;
	m_camController.update(dt);

	m_gossipManager.tick(m_numNPCTicks);

	for (auto& npc : m_npcManager.getNPCVec())
	{
#if TICK_ONCE_SECOND
		if (!m_userUpdatePaused)
		{
			if (m_npcUpdateTime >= secondsBetweenNPCUpdates)
			{
				npc.tick();
			}
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
	if (!m_userUpdatePaused)
	{
		if (m_npcUpdateTime >= secondsBetweenNPCUpdates)
		{
			m_numNPCTicks++;
			m_npcUpdateTime = 0.0f;
		}
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


	/*if (m_selectedGossip != 0)
	{
		auto gossipNPCVec = m_gossipManager.getNPCsHeardGossip(m_selectedGossip);

		for (auto npc : gossipNPCVec)
			const_cast<GS::npc::NPC*>(npc)->setColour({ 0.6f, 0.0f, 0.7f, 1.0f });
	}*/


#if MOVEABLE_NPC
	if (daedalusCore::application::Input::getMouseButton(DD_INPUT_MOUSE_BUTTON_1) && m_mouseInBoundsThisFrame)
		m_hoveredNPC->setPosition(m_camController.mouseToWorldPosition(daedalusCore::application::Input::getMousePosition()));
#endif

	daedalusCore::graphics::RenderCommands::setClearColour({0.14f, 0.14f, 0.14f, 1.0f});
	daedalusCore::graphics::RenderCommands::clear();

	daedalusCore::graphics::Renderer2D::begin(m_camController.getCamera());

	if (m_selectedGossip != 0)
	{
		auto gossipNPCVec = m_gossipManager.getNPCsHeardGossip(m_selectedGossip);

		for (auto npc : gossipNPCVec)
		{
			const_cast<GS::npc::NPC*>(npc)->setColour({ 0.6f, 0.0f, 0.7f, 1.0f });

#if SHOW_GLITCH_ARROWS
			const auto& dGossip = m_gossipManager.getDetailedGossipFromID(m_selectedGossip);
			for (const auto& event : dGossip.eventMap)
			{
				for (const auto& gossipEvent : event.second)
				{
					auto spreadVec = m_npcManager.findNPC(gossipEvent.spreader).getPosition();
					auto listenVec = m_npcManager.findNPC(gossipEvent.listener).getPosition();

					renderArrow(spreadVec,
						listenVec,
						gossipEvent.outcome == true ? daedalusCore::maths::vec4(0.0f, 1.0f, 0.0f, 1.0f) : daedalusCore::maths::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				}
			}
#endif
		}
	}

	//renderArrow(m_npcManager.findNPC("Alpha").getPosition(), m_camController.mouseToWorldPosition(daedalusCore::application::Input::getMousePosition()), {0.0f, 1.0f, 0.0f, 1.0f});

	for (auto& npc : m_npcManager.getNPCVec())
		npc.render();

	daedalusCore::graphics::Renderer2D::end();
}

void SimLayer::imGuiRender()
{
	ImGui::DockSpaceOverViewport(0, 0, ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::Begin("Simulator Controls");

	char playButtonbuff[50];
	sprintf_s(playButtonbuff, "%s", m_userUpdatePaused ? "Play" : "Pause");

	if (ImGui::Button(playButtonbuff))
	{
		m_userUpdatePaused = !m_userUpdatePaused;
		m_npcUpdateTime = 0.0f;
	}

	ImGui::SameLine();

	if (ImGui::Button("Next Tick"))
	{
		for (auto& npc : m_npcManager.getNPCVec())
			npc.tick();
		
		m_numNPCTicks++;
	}

	ImGui::SameLine();

	ImGui::Text("[Current Tick: %i]", m_numNPCTicks);

	ImGui::SameLine();

	ImGui::Text("[Time until next tick: %.1f]", secondsBetweenNPCUpdates - m_npcUpdateTime);

	ImGui::SameLine();

	if (ImGui::Button("Restart"))
	{
		//restart logic
		DD_LOG_WARN("Simulation restarted");

		m_userUpdatePaused = true;
		m_npcUpdateTime = 0.0f;
		m_numNPCTicks = 0;
		m_selectedGossip = 0;

		m_npcManager.restart();
		m_gossipManager.restart();
	}

	ImGui::End();

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

	ImGui::Begin("Gossip Details");

	if (m_selectedGossip == 0)
	{
		ImGui::Text("No Gossip Selected");
		ImGui::TextWrapped("Please select a gossip instance from the 'Gossip Selector' to view it's details");
	}
	else
	{
		const auto& dGossip = m_gossipManager.getDetailedGossipFromID(m_selectedGossip);

		ImGui::SeparatorText("Details");
		ImGui::Text("Gossip ID is %s", dGossip.fileID.c_str());
		ImGui::Text("Gossip is about: %s", dGossip.aboutNPC.c_str());
		ImGui::Text("Gossip type is %s", gossip_to_string(dGossip.type).c_str());

		ImGui::SeparatorText("Events List");
		ImGui::Text("Started on Tick %i from %s", dGossip.startTick, dGossip.startingNPC.c_str());

		if (ImGui::BeginTable("EventTable", 4, ImGuiTableFlags_Borders))
		{
			ImGui::TableSetupColumn("Occurence Tick");
			ImGui::TableSetupColumn("Spreader NPC");
			ImGui::TableSetupColumn("Listener NPC");
			ImGui::TableSetupColumn("Outcome");
			//ImGui::TableSetupColumn("Reason");
			ImGui::TableHeadersRow();

			for (const auto& event : dGossip.eventMap)
			{
				for (const auto& gossipEvent : event.second)
				{
					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("%i", event.first);

					ImGui::TableSetColumnIndex(1);
					ImGui::Text("%s", gossipEvent.spreader.c_str());

					ImGui::TableSetColumnIndex(2);
					ImGui::Text("%s", gossipEvent.listener.c_str());

					ImGui::TableSetColumnIndex(3);
					ImGui::Text("%s", gossipEvent.outcome ? "Remembered" : "Ignored");
					ImGui::SameLine();

					ImGui::TextDisabled("(?)");
					if (ImGui::BeginItemTooltip())
					{
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);

						char reasonBuffer[50];
						sprintf_s(reasonBuffer, "Reason: \n%s", gossipEvent.reason.c_str());

						ImGui::TextUnformatted(reasonBuffer);
						ImGui::PopTextWrapPos();
						ImGui::EndTooltip();
					}

					//ImGui::TableSetColumnIndex(4);
					//ImGui::Text("%s", gossipEvent.reason.c_str());
				}
			}
			ImGui::EndTable();
		}

		
	}

	ImGui::End();
}

void SimLayer::onEvent(daedalusCore::event::Event& e)
{
	//ignore mouse scrolled events 
	if (e.getType() == daedalusCore::event::EventType::MouseScrolled)
		return;

	m_camController.onEvent(e);
}