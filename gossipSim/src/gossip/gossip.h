#pragma once

#include <string>
#include <string>

namespace GS { namespace gossip {

	enum class GossipType
	{
		positive,
		negative,
		neutral
	};

	GossipType string_to_gossip_type(const std::string& str);
	std::string gossip_to_string(const GossipType& gossipType);

	struct Gossip
	{
		GossipType type;
		std::string aboutNPC;
		uint32_t id;

		Gossip()
			: type(GossipType::neutral), aboutNPC("NULL"), id(0)
		{
		}

		Gossip(GossipType type, const std::string& about, uint32_t id)
			: type(type), aboutNPC(about), id(id)
		{
		}

		bool operator==(const Gossip& other)
		{
			return (this->type == other.type && this->aboutNPC == other.aboutNPC);
		}
	};

} }