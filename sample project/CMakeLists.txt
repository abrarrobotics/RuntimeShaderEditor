
cmake_minimum_required ( VERSION 2.5 )
project (3DBasicFramework)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable ( 3DApplication src/main.cpp
                               src/Scene.cpp
                               src/WindowManager/WindowManager.cpp
                               src/InputManager/InputManager.cpp
                               src/ShaderProgram/ShaderProgram.cpp
                               src/Camera/Camera.cpp
                               src/Textures/Texture.cpp
                               src/ModelLoader/ModelLoader.cpp
                               src/ModelLoader/FormatDetector.cpp
                               src/ModelLoader/PLYLoader.cpp
                               src/Utils/tga.cpp
                               src/Monkey.cpp)

if(WIN32)
find_library( SDL2MAIN_PATH
              NAMES          SDL2main
              PATHS          "$ENV{CC_COMMON_LIBS}/SDL/VS2012/"
              PATH_SUFFIXES  "/lib/x86" )

find_library( SDL2_PATH
              NAMES          SDL2
              PATHS          "$ENV{CC_COMMON_LIBS}/SDL/VS2012/"
              PATH_SUFFIXES  "/lib/x86" )

find_library (GLEW_PATH
              NAMES         glew32
              PATHS         "$ENV{CC_COMMON_LIBS}/glew/VS2012/"
              PATH_SUFFIXES "lib/Release/Win32" )
else()
              set ( SDL2MAIN_PATH SDL2main)
              set ( SDL2_PATH     SDL2)
              set ( GLEW_PATH     GLEW)
endif()


if (WIN32)
include_directories ( "${PROJECT_SOURCE_DIR}/src"
                      "$ENV{CC_COMMON_LIBS}/SDL/VS2012/include"
                      "$ENV{CC_COMMON_LIBS}/glew/VS2012/include"
                      "$ENV{CC_COMMON_LIBS}/glm" )


else()
include_directories ( "${PROJECT_SOURCE_DIR}"
                      "$ENV{CC_COMMON_LIBS}/glm" )
endif()


if(WIN32)
    set ( OPENGL opengl32 )
else()
    set ( OPENGL GL )
endif()

target_link_libraries ( 3DApplication ${SDL2MAIN_PATH}
                                      ${SDL2_PATH}
                                      ${OPENGL}
                                      ${GLEW_PATH} )


if(WIN32)
    ADD_CUSTOM_COMMAND( TARGET 3DApplication POST_BUILD
    COMMAND copy
    ARGS "$ENV{CC_COMMON_LIBS}\\SDL\\VS2012\\lib\\x86\\SDL2.dll" "${PROJECT_SOURCE_DIR}\\build\\Debug\\SDL2.dll"
    COMMAND copy
    ARGS "$ENV{CC_COMMON_LIBS}\\glew\\VS2012\\bin\\Release\\Win32\\glew32.dll" "${PROJECT_SOURCE_DIR}\\build\\Debug\\glew32.dll" )
endif()
