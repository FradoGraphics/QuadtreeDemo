cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(QuadtreeDemo CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "Win32")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################



























set(PROJECT_NAME QuadtreeDemo_VS-2022)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "docs/MIT_License.txt"
    "docs/troubleshooting_guide.txt"
    "README.md"
)
source_group("" FILES ${no_group_source_files})

set(Header_Files
    "src/QuadTree.h"
    "src/QuadtreeApp.h"
    "src/resource.h"
    "src/stdafx.h"
    "src/Version.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Header_Files__Data
    "src/Data/MeshData.h"
    "src/Data/QuadTreeData.h"
    "src/Data/RenderData.h"
    "src/Data/ResultData.h"
)
source_group("Header Files\\Data" FILES ${Header_Files__Data})

set(Header_Files__Dialogs
    "src/Dialogs/AboutDialog.h"
    "src/Dialogs/DialogBase.h"
    "src/Dialogs/MsgBoxDialog.h"
    "src/Dialogs/OpenglDialog.h"
    "src/Dialogs/QuadtreeInfoDialog.h"
)
source_group("Header Files\\Dialogs" FILES ${Header_Files__Dialogs})

set(Header_Files__Freeglut
    "src/Freeglut/freeglut_ext.h"
    "src/Freeglut/freeglut_std.h"
)
source_group("Header Files\\Freeglut" FILES ${Header_Files__Freeglut})

set(Header_Files__Misc
    "src/Misc/Vector.h"
)
source_group("Header Files\\Misc" FILES ${Header_Files__Misc})

set(Header_Files__OpenGL
    "src/OpenGL/glext.h"
    "src/OpenGL/OpenGLCamera.h"
    "src/OpenGL/OpenGLDefinitions.h"
    "src/OpenGL/OpenGLExtensions.h"
    "src/OpenGL/OpenGLFragmentShader.h"
    "src/OpenGL/OpenGLInterface.h"
    "src/OpenGL/OpenGLProgram.h"
    "src/OpenGL/OpenGLRenderer.h"
    "src/OpenGL/OpenGLShader.h"
    "src/OpenGL/OpenGLVertexShader.h"
)
source_group("Header Files\\OpenGL" FILES ${Header_Files__OpenGL})

set(Header_Files__Utilities
    "src/Utilities/AppMessageHook.h"
    "src/Utilities/StreamUtils.h"
    "src/Utilities/trace.h"
    "src/Utilities/WindowUtils.h"
)
source_group("Header Files\\Utilities" FILES ${Header_Files__Utilities})


set(Resource_Files
    "src/QuadtreeDemo.rc"
	"src/Resources/title_quad.ico"
)
source_group("Resource Files" FILES ${Resource_Files})


set(Source_Files
    "src/QuadTree.cpp"
    "src/QuadtreeApp.cpp"
    "src/QuadtreeDemo.cpp"
    "src/stdafx.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(Source_Files__Data
    "src/Data/MeshData.cpp"
    "src/Data/QuadTreeData.cpp"
    "src/Data/RenderData.cpp"
)
source_group("Source Files\\Data" FILES ${Source_Files__Data})

set(Source_Files__Dialogs
    "src/Dialogs/AboutDialog.cpp"
    "src/Dialogs/MsgBoxDialog.cpp"
    "src/Dialogs/OpenglDialog.cpp"
    "src/Dialogs/QuadtreeInfoDialog.cpp"
)
source_group("Source Files\\Dialogs" FILES ${Source_Files__Dialogs})

set(Source_Files__Misc
    "src/Misc/Vector.cpp"
)
source_group("Source Files\\Misc" FILES ${Source_Files__Misc})


set(Source_Files__OpenGL
    "src/OpenGL/OpenGLCamera.cpp"
    "src/OpenGL/OpenGLExtensions.cpp"
    "src/OpenGL/OpenGLFragmentShader.cpp"
    "src/OpenGL/OpenGLProgram.cpp"
    "src/OpenGL/OpenGLRenderer.cpp"
    "src/OpenGL/OpenGLShader.cpp"
    "src/OpenGL/OpenGLVertexShader.cpp"
)
source_group("Source Files\\OpenGL" FILES ${Source_Files__OpenGL})

set(Source_Files__Utilities
    "src/Utilities/AppMessageHook.cpp"
    "src/Utilities/trace.cpp"
    "src/Utilities/WindowUtils.cpp"
)
source_group("Source Files\\Utilities" FILES ${Source_Files__Utilities})

set(ALL_FILES
    ${no_group_source_files}
    ${Header_Files}
    ${Header_Files__Data}
    ${Header_Files__Dialogs}
    ${Header_Files__Freeglut}
    ${Header_Files__Misc}
    ${Header_Files__OpenGL}
    ${Header_Files__Utilities}
	${Resource_Files}
    ${Source_Files}
    ${Source_Files__Data}
    ${Source_Files__Dialogs}
    ${Source_Files__Misc}
    ${Source_Files__OpenGL}
    ${Source_Files__Utilities}
	"manifests/DPIaware.manifest"
	"manifests/CommonControls.manifest"
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.h>"
)

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE QuadtreeDemo)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "QuadtreeDemo"
    TARGET_NAME_RELEASE "QuadtreeDemo"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/$<CONFIG>"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/$<CONFIG>"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebug
    >
    $<$<CONFIG:Release>:
        MultiThreaded
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/glm;"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/WinSDK;"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Misc;"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities;"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Data;"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Dialogs;"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/OpenGL;"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Freeglut;"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "WIN32;"
    "_CONSOLE;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /RTC1;
            /ZI
        >
        $<$<CONFIG:Release>:
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        /W3;
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Zc:forScope
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /DEBUG;
            /INCREMENTAL;
            /NODEFAULTLIB:libcmt.lib
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /MANIFEST;
        /MACHINE:X86;
        /SUBSYSTEM:WINDOWS;
        /ENTRY:mainCRTStartup
    )
endif()

################################################################################
# Post build events
################################################################################

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  			# which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/bin/freeglut.dll"      		# <--this is in-file
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)               	# <--this is out-file path


target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/./lib"
)

