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
		"%{prj.name}/Dependencies/pugixml/pugixml.hpp",
		"%{prj.name}/Dependencies/pugixml/pugixml.cpp",
		"%{prj.name}/Dependencies/pugixml/pugiconfig.hpp"
	}

	includedirs
	{
		"%{prj.name}/Dependencies/pugixml"
	}

	links
	{
	}

	postbuildcommands
	{
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "on"

	filter "configurations:Realease"
		optimize "on"

	filter "configurations:Distro"
		optimize "on"