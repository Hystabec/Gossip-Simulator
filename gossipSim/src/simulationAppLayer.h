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

public:
	const daedalusCore::maths::vec4 negativeRelationColour = { 0.8f, 0.2f, 0.2f, 1.0f };
	const daedalusCore::maths::vec4 positiveRelationColour = { 0.2f, 0.8f, 0.2f, 1.0f };
	const float relationLineWidth = 0.05f;

private:
	daedalusCore::graphics::OrthographicCameraController m_camController;

	std::vector<GS::npc::NPC> m_npcVec;
	GS::gossip::GossipManager m_gossipManager;

	bool m_mouseInBoundsThisFrame = false;
	GS::npc::NPC* m_hoveredNPC = nullptr;
};