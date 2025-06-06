#pragma once
#include <Daedalus.h>

#include "npc/npcManager.h"
#include "gossip/gossipManager.h"

class SimLayer : public daedalusCore::application::Layer
{
public:
	SimLayer();

	void attach() override;
	void detach() override;
	void update(const daedalusCore::application::DeltaTime& dt) override;
	void imGuiRender() override;
	void onEvent(daedalusCore::event::Event& e) override;

private:
	daedalusCore::graphics::OrthographicCameraController m_camController;


	float secondsBetweenNPCUpdates = 2.0f;
	float m_npcUpdateTime = 0.0f;
	GS::npc::NPCManager m_npcManager;
	GS::gossip::GossipManager m_gossipManager;
	uint32_t m_selectedGossip = 0;
	uint32_t m_numNPCTicks = 0;

	bool m_mouseInBoundsThisFrame = false;
	GS::npc::NPC* m_hoveredNPC = nullptr;

	bool m_userUpdatePaused = true;
};