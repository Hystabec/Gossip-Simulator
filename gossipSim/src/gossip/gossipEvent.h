#pragma once
#include <string>

namespace GS::gossip {

	struct GossipEvent
	{
		std::string spreader;
		std::string listener;
		bool outcome;
		std::string reason;

		GossipEvent()
			: spreader("NULL"), listener("NULL"), outcome(false), reason("NULL")
		{
		}

		GossipEvent(const std::string& spreader, const std::string& listener, bool outcome, const std::string& reason)
			: spreader(spreader), listener(listener), outcome(outcome), reason(reason)
		{
		}
	};

}