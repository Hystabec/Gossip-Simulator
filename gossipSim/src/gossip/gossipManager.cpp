#include "gossipManager.h"

namespace GS { namespace gossip {

    GossipManager* GossipManager::s_instance = nullptr;

    GossipManager::GossipManager()
    {
        DD_ASSERT(!s_instance, "Duplicate GossipManager");
        s_instance = this;
    }

    GossipManager::~GossipManager()
    {
        s_instance = nullptr;
    }

    uint32_t GossipManager::createGossip(GossipType type, const std::string& about, const npc::NPC& npcToStartFrom)
    {
        uint32_t id = m_activeGossips.size() + 1;
        m_activeGossips.push_back({ type,about, id });
        m_activeGossipMap.insert({ id, {&npcToStartFrom}});
        return id;
    }

    void GossipManager::registerGossipListener(uint32_t gossipID, const npc::NPC* listener)
    {
        m_activeGossipMap[gossipID].push_back(listener);
    }

} }