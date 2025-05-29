#pragma once

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
		std::string fileID;
		uint32_t trackingID;

		Gossip()
			: type(GossipType::neutral), aboutNPC("NULL"), fileID(""), trackingID(0)
		{
		}

		Gossip(GossipType type, const std::string& about, std::string fileID, uint32_t trackingID)
			: type(type), aboutNPC(about), fileID(fileID), trackingID(trackingID)
		{
		}

		bool operator==(const Gossip& other)
		{
			return (this->type == other.type && this->aboutNPC == other.aboutNPC);
		}
	};

} }