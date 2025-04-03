#pragma once

#include <memory>
#include <vector>

#include "../npc/npc.h"
#include "gossip.h"

namespace GS { namespace gossip {

	class GossipManager
	{
	public:
		GossipManager(std::vector<std::shared_ptr<npc::NPC>>& npcVec) : m_NPCVecRef(npcVec) { };
		~GossipManager() { m_activeGossipVec.clear(); }

		const Gossip& createGossip(GossipType type, std::string& about, std::shared_ptr<npc::NPC> npcToStartFrom);

	private:
		std::vector<std::shared_ptr<npc::NPC>>& m_NPCVecRef;
		std::vector<Gossip> m_activeGossipVec;
	};

} }