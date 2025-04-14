workspace "Gossip-Simulator"
	architecture "x64"
	startproject "gossipSim"

	configurations
	{
		"Debug",
		"Release",
		"Distro"
	}

outputdir = "%{cfg.buildcfg}/%{cfg.system}-%{cfg.architecture}"

include "gossipSim/Dependencies/Daedalus"

project "gossipSim"
	location "gossipSim"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/application.cpp",
		"%{prj.name}/Dependencies/pugixml/pugixml.hpp",
		"%{prj.name}/Dependencies/pugixml/pugixml.cpp",
		"%{prj.name}/Dependencies/pugixml/pugiconfig.hpp"
	}

	includedirs
	{
		"%{prj.name}/Dependencies/pugixml",
		"%{prj.name}/Dependencies/Daedalus/Engine/Daedalus-Core",
		"%{prj.name}/Dependencies/Daedalus/Engine/Daedalus-Core/src",
		"%{prj.name}/Dependencies/Daedalus/Engine/Daedalus-Core/Dependencies/ImGui/include"
	}

	links
	{
		"Daedalus-Core"
	}

	postbuildcommands
	{
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines 
		{
			"DD_DEBUG",
			"DD_USING_ASSERTS",
			"DD_USING_PROFILING"
		}
		symbols "on"

	filter "configurations:Realease"
		defines
		{
			"DD_RELEASE",
			"DD_USING_ASSERTS",
			"DD_USING_PROFILING"
		} 
		optimize "on"

	filter "configurations:Distro"
		defines "DD_DISTRO"
		optimize "on"