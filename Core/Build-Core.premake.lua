project "Core"
kind "StaticLib"
language "C++"
cppdialect "C++20"
targetdir "Binaries/%{cfg.buildcfg}"
staticruntime "off"



includedirs
{
  "Source",
  "Source/Renderer",
  "./Vendors/glfw/include",
  "./Vendors/glad/include/",
  "./Vendors/glm/",
  "./Vendors/stb/"
}

files { "./Source/**.h", "./Source/**.cpp", "./Renderer/**.h","./Renderer/**.cpp", "./Vendors/glad/src/glad.c", "./Vendors/stb/stb_image.cpp" }

group "deps"
include "./Vendors/glfw/Build.premake.lua"
group ""

links{
  "GLFW",
}

targetdir (RootDir.."/Binaries/" .. OutputDir .. "/%{prj.name}")
objdir (RootDir.."/Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

filter "system:windows"
systemversion "latest"
defines { }

filter "configurations:Debug"
defines { "DEBUG" }
runtime "Debug"
symbols "On"

filter "configurations:Release"
defines { "RELEASE" }
runtime "Release"
optimize "On"
symbols "On"

filter "configurations:Dist"
defines { "DIST" }
runtime "Release"
optimize "On"
symbols "Off"
