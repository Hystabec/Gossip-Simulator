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
		
		int i = -1;
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
		
		int i = -1;
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

			ImGui::Text("Gossip ID: %s", m_currentNodeName.c_str());
			ImGui::Separator();
			ImGui::Text("Type: %s", npcNode.attribute("type").as_string());
			ImGui::Separator();
			ImGui::Text("NPC about: %s", npcNode.attribute("about").as_string());
			ImGui::Separator();
			ImGui::Text("NPC starting from: %s", npcNode.attribute("startingFrom").as_string());
			ImGui::Separator();
			ImGui::Text("Starting on NPC tick: %s", npcNode.attribute("startTick").as_string());
		}
		else
		{
			ImGui::Text("Please select an gossip instance from the gossip list");
		}

		ImGui::End();
	}

}

void AppLayer::onEvent(daedalusCore::event::Event& e)
{
}

void AppLayer::setOpenFile(const std::string& file)
{
	if (file.find(".xml") != std::string::npos)
	{
		pugi::xml_parse_result result = m_currentXmlFile.load_file(file.c_str());

		if (!result)
			DD_ASSERT(false, DD_ASSERT_FORMAT_MESSAGE("Couldnt find {}", npcFile));

		//Try NPC File
		if (m_currentXmlFile.child("listOfNPC"))
			m_currentFileType = XMLFileType::NPC;
		//Try Gossip File
		else if (m_currentXmlFile.child("listOfGossipEvents"))
			m_currentFileType = XMLFileType::Gossip;
		//Check if empty
		else if (!m_currentXmlFile.document_element())	
			m_currentFileType = XMLFileType::Empty;
		//Else make a warning
		else
		{
			DD_LOG_WARN("File contain invalid data format");
			return;
		}
	}
	else
	{
		DD_LOG_WARN("Selected file is not .XML format");
		return;
	}

	//only gets here if a new file is choosen - reset any data reqired
	m_currentFile = file;
	m_selectedItem = 0;
	m_currentNodeName = "";
	m_selectedNode = false;
	m_selectedTableRow = -1;
}
