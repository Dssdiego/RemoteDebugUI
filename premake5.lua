workspace "RemoteDebugUI"
	configurations { "Debug", "Release" }
	platforms { "x64" }
	startproject "RemoteDebugUI"
	toolset ("msc")

group "Libraries"
	include "lib_imgui.lua"
group ""

project "RemoteDebugUI"
	kind "ConsoleApp"
	language "C++"
	cppdialect  "C++20"
	targetdir "Build/%{cfg.buildcfg}"
	debugdir "Build/%{cfg.buildcfg}"
	objdir "Intermediate/obj/%{cfg.buildcfg}"
	location "Intermediate/ProjectFiles"
	debugformat "c7"

	links 
	{ 
		"SDL3",
		"vulkan-1",
		"ImGui"
	}
	
	flags 
	{
		"Maps",						-- Enable Generate Map File for Visual Studio
		"MultiProcessorCompile", 	-- Enable Visual Studio to use multiple compiler processes when building
	}

	files 
	{ 
		-- "Include/**.h",
		
		"Source/**.h", 
		"Source/**.cpp",

		"Source/Tests/**.cc",

		"*.lua"
	}

	libdirs
	{
		"Libraries/SDL3/lib/x64",
		"Libraries/vulkan/lib/"
	}

	includedirs
	{
		-- "Include",
		"Libraries",
		"Libraries/SDL3/include/",
		"Libraries/SDL3/include/SDL3",
		"Libraries/vulkan/include/"
	}
	
	-- Copy DLLs and other files to the Build directory (needed to run the game/application)
	-- This runs only after builds!
	postbuildcommands 
	{
		print("Copying DLLs to the project output directory..."),
		"{COPY} ../../Libraries/SDL3/lib/x64/SDL3.dll %{cfg.targetdir}",
		"{COPY} ../../Libraries/vulkan/lib/vulkan-1.dll %{cfg.targetdir}",
	}

	filter { "platforms:x64" }
		system "Windows"
		architecture "x64"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		optimize "Off"
		links { }

	filter "configurations:Release"
		runtime "Release"
		symbols "Off"
		optimize "Full"
		links { }
