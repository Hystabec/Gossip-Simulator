#pragma once

#include "npc/npc.h"
#include "gossip/gossipManager.h"

#include <Daedalus.h>
#include <vector>

namespace GS {

	class simCore 
	{
	public:
		simCore();
		~simCore();

		/// @brief updates the core returns false if the core ended 
		bool update();

		void renderNPCs();

		const npc::NPC& findNPC(const std::string& name) const;
		//inline const std::vector<npc::NPC*>& getNPCVector() const { return m_npcVec; }

	private:
		daedalusCore::shr_ptr<daedalusCore::graphics::Texture2D> m_circleTexture;

		std::vector<npc::NPC> m_npcVec;
		gossip::GossipManager m_gossipManager;

		uint32_t updateCount = 0;
	};

}