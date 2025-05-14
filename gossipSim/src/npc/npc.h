#pragma once

#include <string>
#include <unordered_map>
#include <Daedalus.h>
#include <forward_list>

#include "../gossip/gossip.h"

//class SimLayer;

namespace GS::npc {

	enum class personality
	{
		gossipSpreader,
		gossipSink,
		standard
	};

	inline personality string_to_personality(const std::string& str)
	{
		if (str == "spreader")
			return personality::gossipSpreader;
		else if (str == "sink")
			return personality::gossipSink;
		else
			return personality::standard;
	}

	class NPC
	{
	public:
		NPC();
		NPC(const std::string& name, const daedalusCore::graphics::primatives2D::QuadProperties& renderProps);
		~NPC();

		void tick();

		inline void setPersonality(personality newPersonality) { m_personality = newPersonality; }

		void setColour(const daedalusCore::maths::vec4& colour);

		void render();

		void setRelationColours(bool revertToDefault = false);

		/* I have passed as a void* even though it will be cast to a SimLayer*
		 this is to avoid having to include simulationAppLayer.h in this file
		 i could pre declare the class, which should be changed if this works 

		   this function is really buggy - if hovering over NPC 1 or NPC 2
		   then if you move NPC 4 it will be renderer 2 times.
		   The angles between the nodes is also being calculated incorrecly
		   so the links between nodes arent alreays correctly represented*/
		void renderRelations();

		inline std::string getName() const { return m_name; }

		void listenToGossip(uint32_t gossipID);

		inline void storeGossip(uint32_t gossipID) { m_storedGossips.push_front(gossipID); m_toldRecentGossip = false; }

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
		inline const std::unordered_map<std::string, int>& getRelationMap() const { return m_relationMap; }

		inline const daedalusCore::maths::vec2& getPosition() const { return { m_renderProperties.position.x, m_renderProperties.position.y }; };
		inline void setPosition(const daedalusCore::maths::vec2& newPos) { m_renderProperties.position = { newPos.x, newPos.y, m_renderProperties.position.z }; }
		
		bool inPointInBounds(const daedalusCore::maths::vec2& pointPos);
		void displayDataToImGui();

	private:
		bool npcExsitsInMap(const std::string& npc);

	private:
		std::string m_name;
		personality m_personality = personality::standard;
		std::unordered_map<std::string, int> m_relationMap;
		daedalusCore::graphics::primatives2D::QuadProperties m_renderProperties;

		std::forward_list<uint32_t> m_storedGossips;
		bool m_toldRecentGossip = true;
	};

	const daedalusCore::maths::vec4 negativeRelationColour = { 0.8f, 0.2f, 0.2f, 1.0f };
	const daedalusCore::maths::vec4 positiveRelationColour = { 0.2f, 0.8f, 0.2f, 1.0f };
	const float relationLineWidth = 0.05f;

}