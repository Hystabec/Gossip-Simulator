#pragma once

#include <Daedalus.h>
#include "pugixml.hpp"

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

	XMLFileType m_currentFileType = XMLFileType::None;
	std::string m_currentFile = "";

	pugi::xml_document m_currentXmlFile;
	bool m_selectedNode = false;
	std::string m_currentNodeName = "";
	int m_selectedTableRow = -1;

	uint32_t m_selectedItem = 0;
};