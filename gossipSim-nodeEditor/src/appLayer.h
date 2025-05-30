#pragma once

#include <Daedalus.h>
#include "pugixml.hpp"
#include <set>

enum class XMLFileType
{
	NPC,
	Gossip,
	Empty,
	None
};

class AppLayer : public daedalusCore::application::Layer
{
public:
	AppLayer();

	void attach() override;
	void detach() override;
	void update(const daedalusCore::application::DeltaTime& dt) override;
	void imGuiRender() override;
	void onEvent(daedalusCore::event::Event& e) override;

private:
	void setOpenFile(const std::string& file);
	bool getIDsFromFile(const pugi::xml_document& curDoc);

	XMLFileType m_currentFileType = XMLFileType::None;
	std::string m_currentFile = "";

	pugi::xml_document m_currentXmlFile;
	bool m_selectedNode = false;
	std::string m_currentNodeName = "";
	int m_selectedTableRow = -1;
	std::set<std::string> m_idSet;

	int m_selectedItem = -1;
};