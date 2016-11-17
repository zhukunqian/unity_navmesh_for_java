--
-- premake5 file to build RecastDemo
-- http://premake.github.io/
--

local action = _ACTION or ""
local todir = "Build/" .. action

solution "recastdemo"
	configurations { 
		"Debug",
		"Release"
	}
	location (todir)

	-- extra warnings, no exceptions or rtti
	flags { 
		"ExtraWarnings",
		"FloatFast",
		"Symbols"
	}
	exceptionhandling "Off"
	rtti "Off"

	-- debug configs
	configuration "Debug*"
		defines { "DEBUG" }
		targetdir ( todir .. "/lib/Debug" )
 
 	-- release configs
	configuration "Release*"
		defines { "NDEBUG" }
		flags { "Optimize" }
		targetdir ( todir .. "/lib/Release" )

	-- windows specific
	configuration "windows"
		defines { "WIN32", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }

project "recast"
	language "C++"
	kind "WindowedApp"
	includedirs { 
		"Include"
	}
	files	{ 
		"Include/*.h",
		"Source/*.cpp",
		"DemoSource/*.cpp"
	}

	-- distribute executable in RecastDemo/Bin directory
	targetdir "Bin"

 project "recastDll"
	language "C++"
	kind "SharedLib"
	includedirs { 
		"Include"
	}
	files	{ 
		"Include/*.h",
		"Source/*.cpp",
		"DemoSource/*.cpp",
		"DemoSource/*.h"
	}

	-- distribute executable in RecastDemo/Bin directory
	targetdir "Bin"