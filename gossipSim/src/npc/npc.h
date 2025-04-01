#pragma once

#include <string>
#include <memory>
#include <unordered_map>

namespace GS { namespace npc {

	class NPC
	{
	public:
		NPC(const std::string& name);
		~NPC();

		void tick();

		inline std::string getName() const { return m_name; }

	public:
		/// @brief This will add new relation to an npc with a relation value between 100 & -100
		void addRelation(std::shared_ptr<NPC> npc, int relationValue);

		/// @brief If a relation to the given npc is found it will update the value
		void updateRelation(std::shared_ptr<NPC> npc, int relationValue);

		/// @brief Returns the value of the relation to an NPC, a bool* can be passed to check if a relation is found 
		int checkRelation(std::shared_ptr<NPC> npc, bool* checkBool = nullptr);

		void removeRelation(std::shared_ptr<NPC> npc);

		/// @brief clears all realtions for this NPC
		void clearRelations();

	private:
		bool npcExsitsInMap(std::shared_ptr<NPC> npc);

	private:
		std::string m_name;
		std::unordered_map<std::shared_ptr<NPC>, int> m_relationMap;
	};

} }