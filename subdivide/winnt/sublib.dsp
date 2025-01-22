# Microsoft Developer Studio Project File - Name="sublib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=sublib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "sublib.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "sublib.mak" CFG="sublib - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "sublib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "sublib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sublib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /I "../template" /I "../include" /I "../../qvlib/include" /I "../viewer" /I "..\..\qvlib\src" /I "..\..\qvlib\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "NOMINMAX" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "sublib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I "../template" /I "../include" /I "../../qvlib/include" /I "../viewer" /I "..\..\qvlib\src" /I "..\..\qvlib\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "NOMINMAX" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF

# Begin Target

# Name "sublib - Win32 Release"
# Name "sublib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\flatmesh.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ivgraph.cpp
# End Source File
# Begin Source File

SOURCE=..\src\quadmanipulator.cpp
# End Source File
# Begin Source File

SOURCE=..\src\quadmesh.cpp
# End Source File
# Begin Source File

SOURCE=..\src\quadrule.cpp
# End Source File
# Begin Source File

SOURCE=..\src\subquad.cpp
# End Source File
# Begin Source File

SOURCE=..\src\subtri.cpp
# End Source File
# Begin Source File

SOURCE=..\src\tagflatmesh.cpp
# End Source File
# Begin Source File

SOURCE=..\src\tagivgraph.cpp
# End Source File
# Begin Source File

SOURCE=..\src\trimanipulator.cpp
# End Source File
# Begin Source File

SOURCE=..\src\trimesh.cpp
# End Source File
# Begin Source File

SOURCE=..\src\trirule.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\arcball.hpp
# End Source File
# Begin Source File

SOURCE=..\include\ballviewer.hpp
# End Source File
# Begin Source File

SOURCE=..\include\camera.hpp
# End Source File
# Begin Source File

SOURCE=..\include\compat.hpp
# End Source File
# Begin Source File

SOURCE=..\include\cvec3t.hpp
# End Source File
# Begin Source File

SOURCE=..\include\flatmesh.hpp
# End Source File
# Begin Source File

SOURCE=..\include\flatmeshvertex.hpp
# End Source File
# Begin Source File

SOURCE=..\include\general.hpp
# End Source File
# Begin Source File

SOURCE=..\include\geoobject.hpp
# End Source File
# Begin Source File

SOURCE=..\include\hmatrix.hpp
# End Source File
# Begin Source File

SOURCE=..\include\ipoly.hpp
# End Source File
# Begin Source File

SOURCE=..\include\ivgraph.hpp
# End Source File
# Begin Source File

SOURCE=..\include\pickablequad.hpp
# End Source File
# Begin Source File

SOURCE=..\include\pickabletri.hpp
# End Source File
# Begin Source File

SOURCE=..\include\pickedstuff.hpp
# End Source File
# Begin Source File

SOURCE=..\include\pickobject.hpp
# End Source File
# Begin Source File

SOURCE=..\include\pickviewer.hpp
# End Source File
# Begin Source File

SOURCE=..\include\quadmanipulator.hpp
# End Source File
# Begin Source File

SOURCE=..\include\quadmesh.hpp
# End Source File
# Begin Source File

SOURCE=..\include\quadmeshobject.hpp
# End Source File
# Begin Source File

SOURCE=..\include\quat.hpp
# End Source File
# Begin Source File

SOURCE=..\include\sectorinfo.hpp
# End Source File
# Begin Source File

SOURCE=..\include\tagflatmesh.hpp
# End Source File
# Begin Source File

SOURCE=..\include\tagivgraph.hpp
# End Source File
# Begin Source File

SOURCE=..\include\trimanipulator.hpp
# End Source File
# Begin Source File

SOURCE=..\include\trimesh.hpp
# End Source File
# Begin Source File

SOURCE=..\include\trimeshobject.hpp
# End Source File
# Begin Source File

SOURCE=..\include\vert.h
# End Source File
# Begin Source File

SOURCE=..\include\viewer.hpp
# End Source File
# End Group
# End Target
# End Project
