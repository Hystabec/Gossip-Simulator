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
		std::string startedFromNPC;

		Gossip(GossipType type, const std::string& about, const std::string& startedFrom)
			: type(type), aboutNPC(about), startedFromNPC(startedFrom)
		{
		}

		bool operator==(const Gossip& other)
		{
			return (this->type == other.type && this->aboutNPC == other.aboutNPC && this->startedFromNPC == other.startedFromNPC);
		}
	};

} }