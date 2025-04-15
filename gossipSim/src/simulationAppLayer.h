#pragma once
#include <Daedalus.h>
#include <vector>
#include "npc/npc.h"
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
	const GS::npc::NPC& findNPC(const std::string& name) const;

private:
	daedalusCore::graphics::OrthographicCameraController m_camController;

	std::vector<GS::npc::NPC> m_npcVec;
	GS::gossip::GossipManager m_gossipManager;

	bool m_mouseInBoundsThisFrame = false;
	GS::npc::NPC* m_hoveredNPC = nullptr;
};