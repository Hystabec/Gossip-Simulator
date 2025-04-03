#pragma once

#include "npc/npc.h"
#include "gossip/gossip.h"
#include "gossip/gossipManager.h"

#include <memory>
#include <vector>

namespace GS {

	class simCore 
	{
	public:
		simCore();
		~simCore();

		/// @brief updates the core returns false if the core ended 
		bool update();

		const std::shared_ptr<npc::NPC>& findNPC(const std::string& name) const;
		inline const std::vector<std::shared_ptr<npc::NPC>>& getNPCVector() const { return m_npcVec; }

	private:
		std::vector<std::shared_ptr<npc::NPC>> m_npcVec;
		std::unique_ptr<gossip::GossipManager> m_gossipManager;
		uint32_t updateCount = 0;
	};

}