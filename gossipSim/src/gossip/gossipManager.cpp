#include "gossipManager.h"

#include <pugixml.hpp>
#include "../npc/npcManager.h"

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

    void GossipManager::tick(uint32_t currentTick)
    {
        m_curTick = currentTick;

        if (m_gossipEvents.empty())
            return;

        if (m_gossipEvents.begin()->startTick == currentTick)
        {
            const GS::npc::NPC& npcFound = npc::NPCManager::get().findNPC(m_gossipEvents.begin()->startingFrom);
            const_cast<GS::npc::NPC&>(npcFound).storeGossip(this->createGossip(m_gossipEvents.begin()->fileID, m_gossipEvents.begin()->type, m_gossipEvents.begin()->aboutNPC, npcFound));

            DD_LOG_INFO("Gossip created on Tick {} | ID = {} Type = {}, about = '{}', startingFrom = '{}'", currentTick, m_gossipEvents.begin()->fileID, gossip_to_string(m_gossipEvents.begin()->type), m_gossipEvents.begin()->aboutNPC, m_gossipEvents.begin()->startingFrom);
            m_gossipEvents.erase(m_gossipEvents.begin());
        }
    }

    void GossipManager::restart()
    {
        m_activeGossipMap.clear();
        m_activeGossips.clear();
        m_detailedGossips.clear();
        m_gossipEvents.clear();
        readGossipDataFile(m_readFileLoc);
    }

    void GossipManager::readGossipDataFile(const std::string& fileLoc)
    {
        m_readFileLoc = fileLoc;

        pugi::xml_document gossipEvent;
        pugi::xml_parse_result result = gossipEvent.load_file(fileLoc.c_str());

        if (!result)
            DD_ASSERT(false, DD_ASSERT_FORMAT_MESSAGE("Couldnt find {}", fileLoc));

        for (pugi::xml_node node_gossip : gossipEvent.child("listOfGossipEvents"))
        {
            m_gossipEvents.insert({ 
                node_gossip.attribute("id").as_string(),
                string_to_gossip_type(node_gossip.attribute("type").as_string()),
                node_gossip.attribute("about").as_string(),
                node_gossip.attribute("startingFrom").as_string(),
                (uint32_t)node_gossip.attribute("startTick").as_uint()
                });
        }
    }

    uint32_t GossipManager::createGossip(std::string id, GossipType type, const std::string& about, const npc::NPC& npcToStartFrom)
    {
        uint32_t tackingid = m_activeGossips.size() + 1;
        m_activeGossips.push_back({ type,about, id, tackingid });
        m_detailedGossips.push_back({ m_activeGossips.back(), npcToStartFrom.getName(), m_curTick });
        m_activeGossipMap.insert({ tackingid, {&npcToStartFrom}});
        return tackingid;
    }

    void GossipManager::registerGossipListener(uint32_t gossipID, const npc::NPC* listener)
    {
        m_activeGossipMap[gossipID].push_back(listener);
    }

    void GossipManager::registerGossipEvent(uint32_t gossipID, const npc::NPC* listener, const npc::NPC* teller, bool outcome, const std::string& reason)
    {
        m_detailedGossips[gossipID - 1].addEvent(m_curTick, { teller->getName(), listener->getName(), outcome, reason });
    }

} }