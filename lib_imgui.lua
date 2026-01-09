project "ImGui"
	kind "StaticLib"
	language "C++"
	targetdir "Build/%{cfg.buildcfg}"
	objdir "Intermediate/obj/%{cfg.buildcfg}"
	location "Intermediate/ProjectFiles"

	includedirs 
	{ 
		"Libraries/imgui/",
		"Libraries/vulkan/include/",
		"Libraries/SDL3/include/",
	}
		
	files 
	{ 
		"Libraries/imgui/**.h",
		"Libraries/imgui/**.cpp"
	}
