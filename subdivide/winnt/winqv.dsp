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

SOURCE=.\..\..\qvlib\include\QvAsciiText.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvBasic.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvChildList.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvCone.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvCoordinate3.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvCube.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvCylinder.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvDB.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvDebugError.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvDict.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvDirectionalLight.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvDrawStyle.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvElement.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvField.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvFieldData.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvFields.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvFontStyle.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvGroup.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvIn.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvIndexedFaceSet.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvIndexedLineSet.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvInfo.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvInput.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvLists.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvLOD.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMaterial.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMaterialBinding.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMatrixTransform.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMFColor.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMFFloat.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMFInt32.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMFLong.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMFString.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMFVec2f.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvMFVec3f.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvNode.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvNodes.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvNormal.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvNormalBinding.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvOrthographicCamera.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvOut.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvPerspectiveCamera.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvPList.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvPointLight.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvPointSet.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvReadError.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvRotation.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvScale.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSeparator.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFBitMask.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFBool.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFColor.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFEnum.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFFloat.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFImage.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFLong.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFMatrix.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFRotation.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFString.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFUShort.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFVec2f.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSFVec3f.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvShapeHints.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSphere.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSpotLight.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvState.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvString.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSubField.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSubNode.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvSwitch.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvTexture2.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvTexture2Transform.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvTextureCoordinate2.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvTextureCoordinateBinding.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvTransform.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvTransformSeparator.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvTranslation.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvUnknownNode.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvWWWAnchor.h
# End Source File
# Begin Source File

SOURCE=.\..\..\qvlib\include\QvWWWInline.h
# End Source File
# End Group
# End Target
# End Project
