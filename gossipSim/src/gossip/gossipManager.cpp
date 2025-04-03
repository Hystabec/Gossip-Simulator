#include "gossipManager.h"

namespace GS { namespace gossip {

    const Gossip& GossipManager::createGossip(GossipType type, std::string& about, npc::NPC* npcToStartFrom)
    {
        // TODO: insert return statement here
        m_activeGossipVec.push_back({ type, about });
        return m_activeGossipVec.back();
    }

} }