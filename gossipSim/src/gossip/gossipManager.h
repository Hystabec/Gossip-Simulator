#pragma once

#include <memory>
#include <unordered_map>
#include <set>
#include <vector>

#include "../npc/npc.h"
#include "gossip.h"
#include "preGossip.h"

namespace GS { namespace gossip {

	class GossipManager
	{
	public:
		GossipManager();
		~GossipManager();

		inline static GossipManager& get() { return *s_instance; }

		void tick(uint32_t currentTick);

		void readGossipDataFile(const std::string& fileLoc);

		uint32_t createGossip(std::string id, GossipType type, const std::string& about, const npc::NPC& npcToStartFrom);
		void registerGossipListener(uint32_t gossipID, const npc::NPC* listener);

		Gossip getGossipFromID(uint32_t gossipID) { return m_activeGossips[gossipID - 1]; }
		const std::vector<const npc::NPC*>& getNPCsHeardGossip(uint32_t gossipID) { return m_activeGossipMap[gossipID]; }
		uint32_t getNextGossipID() const { return m_activeGossips.size() + 1; }

	private:
		static GossipManager* s_instance;

		std::unordered_map<uint32_t, std::vector<const npc::NPC*>> m_activeGossipMap;
		std::vector<Gossip> m_activeGossips;
		std::set<PreGossip> m_gossipEvents;
	};

} }