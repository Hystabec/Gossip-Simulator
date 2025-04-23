#pragma once
#include "npc.h"

namespace GS::npc {

	class NPCManager
	{
	public:
		NPCManager(const std::string& npcFile);
		~NPCManager();

		inline static NPCManager& get() { return *s_instance; }

		const GS::npc::NPC& findNPC(const std::string& name) const;
		std::vector<GS::npc::NPC>& getNPCVec() { return m_npcVec; };

	private:
		static NPCManager* s_instance;

		std::vector<GS::npc::NPC> m_npcVec;
	};

} 
