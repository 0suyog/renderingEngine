project "App"
kind "ConsoleApp"
language "C++"
cppdialect "C++20"
targetdir "Binaries/%{cfg.buildcfg}"
staticruntime "off"


includedirs
{
  "Source",

  -- Include Core
  "../Core/Renderer/",
  "../Core/Source/",
  "../Core/Vendors/glad/include/",
  "../Core/Vendors"
}

files { "Source/**.h", "Source/**.cpp", "../Core/Vendors/glad/src/glad.c"}

links
{
  "Core",
  "GLFW",
}

targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")


filter "system:windows"
systemversion "latest"
defines { "WINDOWS" }

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
