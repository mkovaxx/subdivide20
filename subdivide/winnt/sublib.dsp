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

SOURCE=..\include\arcball.h
# End Source File
# Begin Source File

SOURCE=..\include\ballviewer.h
# End Source File
# Begin Source File

SOURCE=..\include\camera.h
# End Source File
# Begin Source File

SOURCE=..\include\compat.h
# End Source File
# Begin Source File

SOURCE=..\include\cvec3t.h
# End Source File
# Begin Source File

SOURCE=..\include\flatmesh.h
# End Source File
# Begin Source File

SOURCE=..\include\flatmeshvertex.h
# End Source File
# Begin Source File

SOURCE=..\include\general.h
# End Source File
# Begin Source File

SOURCE=..\include\geoobject.h
# End Source File
# Begin Source File

SOURCE=..\include\hmatrix.h
# End Source File
# Begin Source File

SOURCE=..\include\ipoly.h
# End Source File
# Begin Source File

SOURCE=..\include\ivgraph.h
# End Source File
# Begin Source File

SOURCE=..\include\pickablequad.h
# End Source File
# Begin Source File

SOURCE=..\include\pickabletri.h
# End Source File
# Begin Source File

SOURCE=..\include\pickedstuff.h
# End Source File
# Begin Source File

SOURCE=..\include\pickobject.h
# End Source File
# Begin Source File

SOURCE=..\include\pickviewer.h
# End Source File
# Begin Source File

SOURCE=..\include\quadmanipulator.h
# End Source File
# Begin Source File

SOURCE=..\include\quadmesh.h
# End Source File
# Begin Source File

SOURCE=..\include\quadmeshobject.h
# End Source File
# Begin Source File

SOURCE=..\include\quat.h
# End Source File
# Begin Source File

SOURCE=..\include\sectorinfo.h
# End Source File
# Begin Source File

SOURCE=..\include\tagflatmesh.h
# End Source File
# Begin Source File

SOURCE=..\include\tagivgraph.h
# End Source File
# Begin Source File

SOURCE=..\include\trimanipulator.h
# End Source File
# Begin Source File

SOURCE=..\include\trimesh.h
# End Source File
# Begin Source File

SOURCE=..\include\trimeshobject.h
# End Source File
# Begin Source File

SOURCE=..\include\vert.h
# End Source File
# Begin Source File

SOURCE=..\include\viewer.h
# End Source File
# End Group
# End Target
# End Project
