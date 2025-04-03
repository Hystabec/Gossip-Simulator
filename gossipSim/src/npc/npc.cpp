#include "npc.h"

#include <iostream>
#include <algorithm>

namespace GS { namespace npc {

	NPC::NPC(const std::string& name)
		: m_name(name)
	{
	}

	NPC::~NPC()
	{
		//Here to check object is being destroyed
		std::cout << "NPC: " << m_name << " destroyed\n";
	}

	void NPC::tick()
	{
	}

	static int clamp_relation_value(int val)
	{
		return std::clamp(val, -100, 100);
	}

	void NPC::addRelation(const std::string& npc, int relationValue)
	{
		if (!npcExsitsInMap(npc))
		{
			m_relationMap[npc] = clamp_relation_value(relationValue);
		}
	}

	void NPC::updateRelation(const std::string& npc, int relationValue)
	{
		if (npcExsitsInMap(npc))
		{
			m_relationMap[npc] = clamp_relation_value(relationValue);
		}
	}

	int NPC::checkRelation(const std::string& npc, bool* checkBool)
	{
		if (npcExsitsInMap(npc))
		{
			if (checkBool)
				*checkBool = true;

			return m_relationMap[npc];
		}

		if(checkBool)
			*checkBool = false;

		return 0;
	}

	void NPC::removeRelation(const std::string& npc)
	{
		if (npcExsitsInMap(npc))
		{
			m_relationMap.erase(npc);
		}
	}

	void NPC::clearRelations()
	{
		m_relationMap.clear();
	}

	bool NPC::npcExsitsInMap(const std::string& npc)
	{
		return m_relationMap.find(npc) != m_relationMap.end() ? true : false;
	}


} }