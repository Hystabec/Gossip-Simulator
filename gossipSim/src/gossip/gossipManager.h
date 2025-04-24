#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "../npc/npc.h"
#include "gossip.h"

namespace GS { namespace gossip {

	class GossipManager
	{
	public:
		GossipManager() {}
		~GossipManager() {}

		const Gossip& createGossip(GossipType type, const std::string& about, const npc::NPC& npcToStartFrom);
		const std::vector<const npc::NPC*>& getNPCsHeardGossip(uint32_t gossipID) { return m_activeGossipMap[gossipID]; }
		uint32_t getNextGossipID() const { return m_activeGossips.size() + 1; }

	private:
		std::unordered_map<uint32_t, std::vector<const npc::NPC*>> m_activeGossipMap;
		std::vector<Gossip> m_activeGossips;
	};

} }