#pragma once
#include "gossip.h"

// This is a simple helper struct from the gossipManager so that it
// can store the gossip events once the file has been parsed

namespace GS::gossip {

	struct PreGossip : public Gossip
	{
		std::string startingFrom;
		uint32_t startTick;

		PreGossip(GossipType type, const std::string& about, const std::string& startingFrom, uint32_t startTick)
			: Gossip(type, about, 0), startingFrom(startingFrom), startTick(startTick)
		{
		}

		friend bool operator<(const PreGossip& left, const PreGossip& right)
		{
			return (left.startTick < right.startTick);
		}
	};

}