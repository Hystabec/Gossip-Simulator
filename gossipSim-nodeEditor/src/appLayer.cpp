#include "appLayer.h"

#include "imgui.h"
#include "windowsFileManipulation.h"

AppLayer::AppLayer()
{
}

void AppLayer::attach()
{
}

void AppLayer::detach()
{
}

void AppLayer::update(const daedalusCore::application::DeltaTime& dt)
{
	daedalusCore::graphics::RenderCommands::setClearColour({ 0.14f, 0.14f, 0.14f, 1.0f });
	daedalusCore::graphics::RenderCommands::clear();
}

void AppLayer::imGuiRender()
{
	ImGui::DockSpaceOverViewport(0, 0, ImGuiDockNodeFlags_PassthruCentralNode);

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	static ImGuiWindowFlags dockSpaceFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	static ImGuiWindowFlags noMovingFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	dockSpaceFlags |= noMovingFlags;
	dockSpaceFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	static bool open = true;
	ImGui::Begin("DockSpace", &open, dockSpaceFlags);
	
	ImGui::PopStyleVar(2);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open")) 
			{ 
				std::string res = open_file_dialog();
				if (res != "")
					setOpenFile(res);
			}

			if (ImGui::MenuItem("Save"))
				m_currentXmlFile.save_file(m_currentFile.c_str());

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("New NPC", NULL, false, m_currentFileType == XMLFileType::NPC))
			{
				int i = 1;
				std::string currCheck = "New NPC";
				while (m_idSet.contains(currCheck))
				{
					currCheck = daedalusCore::debug::Log::formatLogMessage("New NPC({})", i);
					i++;
				}

				auto newNPC = m_currentXmlFile.child("listOfNPC").append_child("NPC");
				newNPC.append_attribute("name");
				newNPC.attribute("name").set_value(currCheck);
				newNPC.append_child("relationships");
				m_idSet.insert(currCheck);
			}

			if (ImGui::MenuItem("New Gossip", NULL, false, m_currentFileType == XMLFileType::Gossip))
			{
				int i = 1;
				std::string currCheck = "New Gossip";
				while (m_idSet.contains(currCheck))
				{
					currCheck = daedalusCore::debug::Log::formatLogMessage("New Gossip({})", i);
					i++;
				}

				auto newNPC = m_currentXmlFile.child("listOfGossipEvents").append_child("Event");
				newNPC.append_attribute("id");
				newNPC.attribute("id").set_value(currCheck);
				newNPC.append_attribute("type");
				newNPC.attribute("type").set_value("neutral");
				newNPC.append_attribute("about");
				newNPC.append_attribute("startingFrom");
				newNPC.append_attribute("startTick");
				newNPC.attribute("startTick").set_value(0);
				m_idSet.insert(currCheck);
			}

			if (ImGui::MenuItem("Rename Selected Item", NULL, false, (m_currentFileType == XMLFileType::Gossip || m_currentFileType == XMLFileType::NPC) && m_selectedItem != -1))
			{
				pugi::xml_node toRemove;
				for (auto node : m_currentXmlFile.child(m_currentFileType == XMLFileType::Gossip ? "listOfGossipEvents" : "listOfNPC"))
				{
					if (node.attribute(m_currentFileType == XMLFileType::Gossip ? "id" : "name").value() == m_currentNodeName)
					{
						toRemove = node;
						break;
					}
				}

				DD_LOG_INFO(open_text_dialog("Hello World"));
			}

			if (ImGui::MenuItem("Delete Selected Item", NULL, false, (m_currentFileType == XMLFileType::Gossip || m_currentFileType == XMLFileType::NPC) && m_selectedItem != -1))
			{
				pugi::xml_node toRemove;
				for (auto node : m_currentXmlFile.child(m_currentFileType == XMLFileType::Gossip ? "listOfGossipEvents" : "listOfNPC"))
				{
					if (node.attribute(m_currentFileType == XMLFileType::Gossip ? "id" : "name").value() == m_currentNodeName)
					{
						toRemove = node;
						break;
					}
				}
				
				m_currentXmlFile.child(m_currentFileType == XMLFileType::Gossip ? "listOfGossipEvents" : "listOfNPC").remove_child(toRemove);
				m_currentNodeName = "";
				m_selectedItem = -1;
				m_selectedNode = false;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

#pragma region Handle Empty or None types
	if (m_currentFileType == XMLFileType::None)
	{
		ImGui::Text("No file selected");
		ImGui::Text("Select the XML file to modify (File -> Open)");
	}
	else if (m_currentFileType == XMLFileType::Empty)
	{
		ImGui::Text("Select file is not formated");
		if (ImGui::Button("Format as NPC file"))
		{
			m_currentXmlFile.append_child("listOfNPC");
			m_currentFileType = XMLFileType::NPC;
		}
		if (ImGui::Button("Format as Gossip file"))
		{
			m_currentXmlFile.append_child("listOfGossipEvents");
			m_currentFileType = XMLFileType::Gossip;
		}
	}
#pragma endregion

	

	ImGui::End();

	if (m_currentFileType == XMLFileType::NPC)
	{
		ImGui::Begin("NPC List");
		
		int i = 0;
		for (pugi::xml_node& child : m_currentXmlFile.child("listOfNPC"))
		{
			if (ImGui::Selectable(child.attribute("name").as_string(), m_selectedItem == i && m_selectedNode) && m_selectedItem != i)
			{
				m_selectedItem = i;
				m_currentNodeName = child.attribute("name").as_string();
				m_selectedNode = true;
			}

			i++;
		}

		ImGui::End();

		ImGui::Begin("NPC Data");

		if (m_currentNodeName != "")
		{
			pugi::xml_node npcNode = m_currentXmlFile.child("listOfNPC").find_child_by_attribute("name", m_currentNodeName.c_str());

			ImGui::Text("NPC: %s", m_currentNodeName.c_str());

			ImGui::Separator();

			pugi::xml_node personalityNode = npcNode.child("personality");
			if (personalityNode)
				ImGui::Text("Pesonality: %s", personalityNode.attribute("type").as_string());
			else
				ImGui::Text("Pesonality: None");

			ImGui::Separator();

			if (ImGui::BeginTable("relationshipTable", 2, ImGuiTableFlags_Borders))
			{
				ImGui::TableSetupColumn("NPC");
				ImGui::TableSetupColumn("Relation");
				//ImGui::TableSetupColumn("Remove");
				ImGui::TableHeadersRow();

				int i = 0;
				for (pugi::xml_node node_relation : npcNode.child("relationships"))
				{
					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text(node_relation.attribute("npc").as_string());

					ImGui::TableSetColumnIndex(1);
					ImGui::Text(node_relation.attribute("value").as_int() > 0 ? "positve" : "negative");

					/*
					ImGui::TableSetColumnIndex(2);
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 1.0f, 1.0f));
					ImGui::PushID(i);
					ImGui::Button("X");
					ImGui::PopID();
					ImGui::PopStyleColor();
					*/

					i++;
				}
			}
			ImGui::EndTable();
		}
		else
		{
			ImGui::Text("Please select an NPC from the NPC list");
		}

		ImGui::End();
	}
	else if (m_currentFileType == XMLFileType::Gossip)
	{
		ImGui::Begin("Gossip List");
		
		int i = 0;
		for (pugi::xml_node& child : m_currentXmlFile.child("listOfGossipEvents"))
		{
			if (ImGui::Selectable(child.attribute("id").as_string(), m_selectedItem == i && m_selectedNode) && m_selectedItem != i)
			{
				m_selectedItem = i;
				m_currentNodeName = child.attribute("id").as_string();
				m_selectedNode = true;
			}

			i++;
		}

		ImGui::End();

		ImGui::Begin("Gossip Data");

		if (m_currentNodeName != "")
		{
			pugi::xml_node npcNode = m_currentXmlFile.child("listOfGossipEvents").find_child_by_attribute("id", m_currentNodeName.c_str());

			char strInputBuffer[128];

			ImGui::Text("Gossip ID: %s", m_currentNodeName.c_str());
			/*ImGui::Text("Gossip ID:");
			ImGui::SameLine();
			ImGui::PushID("gossipDataName");
			sprintf_s(strInputBuffer, m_currentNodeName.c_str());
			if (ImGui::InputText("##", strInputBuffer, sizeof(strInputBuffer) / sizeof(char)))
			{
				if (m_idSet.contains(strInputBuffer))
				{

				}
				else
				{
					npcNode.attribute("id").set_value(strInputBuffer);
					m_idSet.erase(m_currentNodeName);
					m_currentNodeName = strInputBuffer;
				}
			}
			ImGui::PopID();*/

			ImGui::Separator();

			ImGui::Text("Type:"); //npcNode.attribute("type").as_string());
			ImGui::SameLine();

			const char* gossipTypes[] = { "positive", "negative", "neutral" };
			std::string gossipBuffer = npcNode.attribute("type").as_string();
			int currSel;
			if (gossipBuffer == "positive")
				currSel = 0;
			else if (gossipBuffer == "negative")
				currSel = 1;
			else
				currSel = 2;

			ImGui::PushID("gossipDataType");
			if (ImGui::Combo("", &currSel, gossipTypes, sizeof(gossipTypes) / sizeof(*gossipTypes)))
			{
				if (currSel == 0)
					npcNode.attribute("type").set_value("positive");
				else if(currSel == 1)
					npcNode.attribute("type").set_value("negative");
				else
					npcNode.attribute("type").set_value("neutral");
			}
			ImGui::PopID();

			ImGui::Separator();
			ImGui::Text("NPC about:");
			ImGui::SameLine();
			ImGui::PushID("gossipDataAbout");
			strcpy_s(strInputBuffer, npcNode.attribute("about").as_string());
			if (ImGui::InputText("", strInputBuffer, sizeof(strInputBuffer)/sizeof(char)))
			{
				npcNode.attribute("about").set_value(strInputBuffer);
			}
			ImGui::PopID();

			ImGui::Separator();
			ImGui::Text("NPC starting from:");
			ImGui::SameLine();
			ImGui::PushID("gossipDataFrom");
			sprintf_s(strInputBuffer, npcNode.attribute("startingFrom").as_string());
			if (ImGui::InputText("", strInputBuffer, sizeof(strInputBuffer) / sizeof(char)))
			{
				npcNode.attribute("startingFrom").set_value(strInputBuffer);
			}
			ImGui::PopID();
			ImGui::Separator();

			ImGui::Text("Starting on NPC tick:");
			ImGui::SameLine();
			ImGui::PushID("gossipDataTick");
			int tickInt = npcNode.attribute("startTick").as_int();
			if (ImGui::InputInt("", &tickInt))
				npcNode.attribute("startTick").set_value(tickInt);
			ImGui::PopID();
		}
		else
		{
			ImGui::Text("Please select an gossip instance from the gossip list");
		}

		ImGui::End();
	}

	ImGui::ShowDemoWindow();
}

void AppLayer::onEvent(daedalusCore::event::Event& e)
{
}

void AppLayer::setOpenFile(const std::string& file)
{
	if (file.find(".xml") != std::string::npos)
	{
		pugi::xml_document checkDoc;
		checkDoc.load_file(file.c_str());
		//pugi::xml_parse_result result = m_currentXmlFile.load_file(file.c_str());
		
		//if (!result)
		//	DD_ASSERT(false, DD_ASSERT_FORMAT_MESSAGE("Couldnt find {}", file));

		//Try NPC File
		if (checkDoc.child("listOfNPC"))
		{
			m_currentFileType = XMLFileType::NPC;
			if (!getIDsFromFile(checkDoc))
			{
				DD_LOG_ERROR("FILE LOAD ERROR: Duplicate NPC names found in XML File");
				m_currentFileType = XMLFileType::None;
				return;
			}
		}
		//Try Gossip File
		else if (checkDoc.child("listOfGossipEvents"))
		{
			m_currentFileType = XMLFileType::Gossip;
			if (!getIDsFromFile(checkDoc))
			{
				DD_LOG_ERROR("FILE LOAD ERROR: Duplicate Gossip IDs found in XML File");
				m_currentFileType = XMLFileType::None;
				return;
			}
		}
		//Check if empty
		else if (!checkDoc.document_element())	
			m_currentFileType = XMLFileType::Empty;
		//Else make a warning
		else
		{
			DD_LOG_ERROR("FILE LOAD ERROR: File contain invalid data format");
			return;
		}
	}
	else
	{
		DD_LOG_ERROR("FILE LOAD ERROR: Selected file is not .XML format");
		return;
	}

	//only gets here if a new file is choosen - reset any data reqired
	m_currentXmlFile.load_file(file.c_str()); //loading a second time isnt ideal but it stops a bug occuring 

	m_currentFile = file;
	m_selectedItem = -1;
	m_currentNodeName = "";
	m_selectedNode = false;
	m_selectedTableRow = -1;
}

bool AppLayer::getIDsFromFile(const pugi::xml_document& curDoc)
{
	m_idSet.clear();

	if (m_currentFileType == XMLFileType::NPC)
	{
		for (auto npc : curDoc.child("listOfNPC"))
		{
			if (!(m_idSet.insert(npc.attribute("name").value()).second))
			{
				DD_LOG_WARN("Durplicate NPC name: {}", npc.attribute("name").value());
				return false;
			}
		}
	}
	else if (m_currentFileType == XMLFileType::Gossip)
	{
		for (auto gossip : curDoc.child("listOfGossipEvents"))
		{
			if (!(m_idSet.insert(gossip.attribute("id").value()).second))
			{
				DD_LOG_WARN("Durplicate Gossip id: {}", gossip.attribute("id").value());
				return false;
			}
		}
	}

	return true;
}
