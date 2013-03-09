import os
import sys
import uuid
filterList = set()

rootdir = sys.argv[1]
print """<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|Win32">
      <Configuration>Debug</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|Win32">
      <Configuration>Release</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
  </ItemGroup>"""
 
print "\t<ItemGroup>"
for root, subFolders, files in os.walk(rootdir):
    root = root.replace(".\\", "")
    for file in files:
        if file.find(".sln") == -1 and file.find(".suo") == -1 and file.find("vcxproj") == -1 and root.find(".git") == -1 and root.find("\\build\\") == -1 :
            fullPath = os.path.join(root,file).replace(".\\", "")
            print "\t\t<None Include=\""+fullPath+"\" />"
print "\t</ItemGroup>"

print """  <PropertyGroup Label="Globals">
    <ProjectGuid>{A220E5EC-C5F4-484B-AD00-CF8B3A24116F}</ProjectGuid>
    <Keyword>MakeFileProj</Keyword>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'" Label="Configuration">
    <ConfigurationType>Makefile</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <OutDir>$(SolutionDir)/bin/$(Configuration)\</OutDir>
    <IntDir>$(SolutionDir)/build/$(Configuration)\</IntDir>
    <PlatformToolset>v110</PlatformToolset>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'" Label="Configuration">
    <ConfigurationType>Makefile</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <PlatformToolset>v110</PlatformToolset>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings">
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <NMakeBuildCommandLine>c:\Python27\scons.bat  @build_all --verbose --site-dir=gclbuildscript/site_scons</NMakeBuildCommandLine>
    <NMakeOutput>kinevox.exe</NMakeOutput>
    <NMakePreprocessorDefinitions>WIN32;_DEBUG;$(NMakePreprocessorDefinitions)</NMakePreprocessorDefinitions>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <NMakeBuildCommandLine>c:\Python27\scons.bat  @build_all --verbose --site-dir=gclbuildscript/site_scons</NMakeBuildCommandLine>
    <NMakeOutput>kinevox.exe</NMakeOutput>
    <NMakePreprocessorDefinitions>OS_WIN32;WIN32;NDEBUG;$(NMakePreprocessorDefinitions)</NMakePreprocessorDefinitions>
  </PropertyGroup>
  <ItemDefinitionGroup>
  </ItemDefinitionGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
  <ImportGroup Label="ExtensionTargets">
  </ImportGroup>
</Project>"""
