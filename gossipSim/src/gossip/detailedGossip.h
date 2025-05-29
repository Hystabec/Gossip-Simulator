#pragma once
#include "gossip.h"
#include "gossipEvent.h"

#include "map"
#include "vector"
#include "string"

namespace GS::gossip {

	struct DetailedGossip
	{
		std::string fileID;
		GossipType type;
		std::string aboutNPC;
		std::string startingNPC;
		uint32_t startTick;
		std::map<uint32_t, std::vector<GossipEvent>> eventMap;

		DetailedGossip()
			: fileID("NULL"), type(GossipType::neutral), aboutNPC("NULL"), startingNPC("NULL"), startTick(0)
		{
		}

		DetailedGossip(const Gossip& baseGossip, const std::string& startNPC, uint32_t startTick)
			: fileID(baseGossip.fileID), type(baseGossip.type), aboutNPC(baseGossip.aboutNPC),
			startingNPC(startNPC), startTick(startTick)
		{
		}

		void addEvent(uint32_t tick, GossipEvent gE)
		{
			eventMap[tick].push_back(gE);
		}
	};

}