include "Engine/Daedalus-Core/Dependencies/ImGui"

dependFol = "Daedalus-Core/Dependencies"
dependDir = {}
dependDir["GLFW"] = dependFol .. "/GLFW"
dependDir["GLEW"] = dependFol .. "/GLEW"
dependDir["spdlog"] = dependFol .. "/spdlog"
dependDir["ImGui"] = dependFol .. "/ImGui"
dependDir["stb_image"] = dependFol .. "/stb_image"

project "Daedalus-Core"
	location "Engine/Daedalus-Core"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "ddpch.h"
	pchsource "Engine/Daedalus-Core/src/ddpch.cpp"

    files
	{
		"Engine/%{prj.name}/Daedalus.h",
		"Engine/%{prj.name}/entryPoint.h",
		"Engine/%{prj.name}/src/**.h",
		"Engine/%{prj.name}/src/**.cpp",
		"Engine/%{prj.name}/Dependencies/stb_image/**.h",
		"Engine/%{prj.name}/Dependencies/stb_image/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{dependDir.GLFW}/include",
		"%{dependDir.GLEW}/include",
		"%{dependDir.spdlog}/include",
		"%{dependDir.ImGui}/include",
		"%{dependDir.stb_image}"
	}

	libdirs
	{
		"%{dependDir.GLFW}/lib-vc2022",
		"%{dependDir.GLEW}/lib",
	}

	links
	{
		"ImGui",
		"glfw3_mt.lib",
		"opengl32.lib",
		"glew32s.lib"
	}

	buildoptions "/utf-8"

	filter "system:windows"
	systemversion "latest"

	defines
	{
		"DD_RENDER_USING_OPENGL",
		"GLEW_STATIC"
	}

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