#pragma once

#include <memory>
#include <vector>

#include "../npc/npc.h"
#include "gossip.h"

namespace GS { namespace gossip {

	class GossipManager
	{
	public:
		GossipManager() { };
		~GossipManager() { m_activeGossipVec.clear(); }

		const Gossip& createGossip(GossipType type, std::string& about, npc::NPC* npcToStartFrom);

	private:
		std::vector<Gossip> m_activeGossipVec;
	};

} }