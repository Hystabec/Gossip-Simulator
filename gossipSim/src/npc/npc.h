#pragma once

#include <string>
#include <unordered_map>

namespace GS { namespace npc {

	class NPC
	{
	public:
		NPC(const std::string& name);
		~NPC();

		void tick();

		inline std::string getName() const { return m_name; }

		operator std::string() { return m_name; }

		/// @brief This will add new relation to an npc with a relation value between 100 & -100
		void addRelation(const std::string& npc, int relationValue);

		/// @brief If a relation to the given npc is found it will update the value
		void updateRelation(const std::string& npc, int relationValue);

		/// @brief Returns the value of the relation to an NPC, a bool* can be passed to check if a relation is found 
		int checkRelation(const std::string& npc, bool* checkBool = nullptr);

		void removeRelation(const std::string& npc);

		/// @brief clears all realtions for this NPC
		void clearRelations();

	private:
		bool npcExsitsInMap(const std::string& npc);

	private:
		std::string m_name;
		std::unordered_map<std::string, int> m_relationMap;
	};

} }