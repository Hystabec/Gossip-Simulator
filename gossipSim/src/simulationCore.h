#pragma once
#include "npc/npc.h"
#include "gossip/gossip.h"

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

	private:
		std::vector<std::shared_ptr<npc::NPC>> m_npcVec;
	};

}