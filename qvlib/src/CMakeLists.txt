# Set the C++ standard and compiler warnings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    QvAsciiText.cpp
    QvChildList.cpp
    QvCone.cpp
    QvCoordinate3.cpp
    QvCube.cpp
    QvCylinder.cpp
    QvDB.cpp
    QvDebugError.cpp
    QvDict.cpp
    QvDirectionalLight.cpp
    QvDrawStyle.cpp
    QvElement.cpp
    QvField.cpp
    QvFieldData.cpp
    QvFontStyle.cpp
    QvGroup.cpp
    QvIn.cpp
    QvIndexedFaceSet.cpp
    QvIndexedLineSet.cpp
    QvInfo.cpp
    QvInput.cpp
    QvLOD.cpp
    QvLib.cpp
    QvLists.cpp
    QvMFColor.cpp
    QvMFFloat.cpp
    QvMFInt32.cpp
    QvMFLong.cpp
    QvMFString.cpp
    QvMFVec2f.cpp
    QvMFVec3f.cpp
    QvMaterial.cpp
    QvMaterialBinding.cpp
    QvMatrixTransform.cpp
    QvName.cpp
    QvNode.cpp
    QvNormal.cpp
    QvNormalBinding.cpp
    QvOrthographicCamera.cpp
    QvOut.cpp
    QvPList.cpp
    QvPerspectiveCamera.cpp
    QvPointLight.cpp
    QvPointSet.cpp
    QvReadError.cpp
    QvRotation.cpp
    QvSFBitMask.cpp
    QvSFBool.cpp
    QvSFColor.cpp
    QvSFEnum.cpp
    QvSFFloat.cpp
    QvSFImage.cpp
    QvSFLong.cpp
    QvSFMatrix.cpp
    QvSFRotation.cpp
    QvSFString.cpp
    QvSFUShort.cpp
    QvSFVec2f.cpp
    QvSFVec3f.cpp
    QvScale.cpp
    QvSeparator.cpp
    QvShapeHints.cpp
    QvSphere.cpp
    QvSpotLight.cpp
    QvState.cpp
    QvString.cpp
    QvSwitch.cpp
    QvTexture2.cpp
    QvTexture2Transform.cpp
    QvTextureCoordinate2.cpp
    QvTextureCoordinateBinding.cpp
    QvTransform.cpp
    QvTransformSeparator.cpp
    QvTranslation.cpp
    QvTraverse.cpp
    QvUnknownNode.cpp
    QvWWWAnchor.cpp
    QvWWWInline.cpp
)

# Create the library
add_library(qvlib ${SOURCES})

# Set include directories
target_include_directories(qvlib
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set output directory
set_target_properties(qvlib PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install rules
install(TARGETS qvlib
    EXPORT qvlib-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)
