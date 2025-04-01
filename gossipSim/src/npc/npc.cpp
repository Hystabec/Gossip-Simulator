#include "npc.h"
#include <iostream>

namespace GS { namespace npc {

	NPC::NPC(const std::string& name)
		: m_name(name)
	{
	}

	void NPC::tick()
	{
		std::cout << "NPC: " << m_name << " tick()\n";
	}

} }