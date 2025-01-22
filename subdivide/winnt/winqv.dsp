# Microsoft Developer Studio Project File - Name="winqv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=winqv - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "winqv.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "winqv.mak" CFG="winqv - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "winqv - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "winqv - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "winqv - Win32 Release"

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
# ADD CPP /nologo /W3 /GR /GX /O2 /I ".\..\..\qvlib\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "NOMINMAX" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "winqv - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I "./../qvlib/include" /I "..\..\qvlib\src" /I "..\..\qvlib\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
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

# Name "winqv - Win32 Release"
# Name "winqv - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvAsciiText.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvChildList.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvCone.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvCoordinate3.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvCube.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvCylinder.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvDB.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvDebugError.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvDict.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvDirectionalLight.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvDrawStyle.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvElement.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvField.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvFieldData.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvFontStyle.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvIn.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvIndexedFaceSet.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvIndexedLineSet.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvInput.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvLists.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvLOD.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvMaterialBinding.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvMatrixTransform.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvMFColor.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvMFFloat.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvMFInt32.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvMFLong.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvMFString.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvMFVec2f.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvMFVec3f.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvName.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvNode.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvNormal.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvNormalBinding.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvOrthographicCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvOut.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvPerspectiveCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvPList.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvPointLight.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvPointSet.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvReadError.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvRotation.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvScale.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSeparator.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSFBitMask.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSFBool.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSFColor.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSFEnum.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSFFloat.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSFImage.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSFLong.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSFMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSFRotation.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSFString.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSFUShort.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSFVec2f.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSFVec3f.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvShapeHints.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSpotLight.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvState.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvString.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvSwitch.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvTexture2.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvTexture2Transform.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvTextureCoordinate2.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvTextureCoordinateBinding.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvTransform.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvTransformSeparator.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvTranslation.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvTraverse.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvUnknownNode.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvWWWAnchor.cpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\src\QvWWWInline.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvAsciiText.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvBasic.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvChildList.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvCone.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvCoordinate3.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvCube.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvCylinder.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvDB.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvDebugError.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvDict.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvDirectionalLight.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvDrawStyle.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvElement.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvField.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvFieldData.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvFields.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvFontStyle.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvGroup.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvIn.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvIndexedFaceSet.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvIndexedLineSet.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvInfo.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvInput.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvLists.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvLOD.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMaterial.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMaterialBinding.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMatrixTransform.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMFColor.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMFFloat.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMFInt32.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMFLong.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMFString.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMFVec2f.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMFVec3f.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvNode.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvNodes.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvNormal.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvNormalBinding.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvOrthographicCamera.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvOut.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvPerspectiveCamera.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvPList.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvPointLight.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvPointSet.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvReadError.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvRotation.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvScale.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSeparator.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFBitMask.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFBool.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFColor.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFEnum.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFFloat.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFImage.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFLong.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFMatrix.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFRotation.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFString.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFUShort.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFVec2f.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFVec3f.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvShapeHints.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSphere.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSpotLight.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvState.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvString.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSubField.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSubNode.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSwitch.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvTexture2.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvTexture2Transform.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvTextureCoordinate2.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvTextureCoordinateBinding.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvTransform.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvTransformSeparator.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvTranslation.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvUnknownNode.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvWWWAnchor.hpp
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvWWWInline.hpp
# End Source File
# End Group
# End Target
# End Project
