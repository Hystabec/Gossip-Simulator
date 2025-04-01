#pragma once

#include <string>

namespace GS { namespace gossip {

	enum class GossipType
	{
		//positive,
		negative,
		neutral
	};

	struct Gossip
	{
		GossipType type;
		std::string aboutNPC;

		Gossip(GossipType type, const std::string& about)
			: type(type), aboutNPC(about)
		{
		}

		bool operator==(const Gossip& other)
		{
			return (this->type == other.type && this->aboutNPC == other.aboutNPC);
		}
	};

} }