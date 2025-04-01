#pragma once
#include <string>

namespace GS { namespace npc {

	class NPC
	{
	public:
		NPC(const std::string& name);

		void tick();

	private:
		std::string m_name;
	};

} }