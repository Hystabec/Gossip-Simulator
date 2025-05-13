#include "gossip.h"

GS::gossip::GossipType GS::gossip::string_to_gossip_type(const std::string& str)
{
	// This doesnt catch any spelling mistakes and will default to neutral 
	// - could add an error type if becomes an issue

	if (str == "positive" || str == "Positive")
		return GossipType::positive;
	else if (str == "negative" || str == "Negative")
		return GossipType::negative;
	else
		return GossipType::neutral;
}

std::string GS::gossip::gossip_to_string(const GossipType& gossipType)
{
	switch (gossipType)
	{
	case GossipType::positive:
		return "positive";
	case GossipType::negative:
		return "negative";
	case GossipType::neutral:
		return "neutral";
	default:
		return "error couldn't find gossip type";
	}
}
