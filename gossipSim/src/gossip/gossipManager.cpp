#include "gossipManager.h"

namespace GS { namespace gossip {

    const Gossip& GossipManager::createGossip(GossipType type, const std::string& about, const npc::NPC& npcToStartFrom)
    {
        uint32_t id = m_activeGossips.size() + 1;
        m_activeGossips.push_back({ type,about, id });
        m_activeGossipMap.insert({ id, {&npcToStartFrom}});
        return m_activeGossips.back();
    }

} }