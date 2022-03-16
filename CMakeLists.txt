cmake_minimum_required(VERSION 3.16)
project(ProceduralWorld)

set(CMAKE_CXX_STANDARD 14)

add_executable(ProceduralWorld
        Main.cpp
        glad.c
        library/math/CGMath.cpp
        library/math/Quaternion.cpp
        library/math/Vector.cpp
        library/math/Matrix.cpp
        library/Utility.cpp
        library/Utility.h
        library/Types.h
        library/gui/RenderList2D.cpp
        library/gui/RenderList2D.h
        library/opengl/OpenGL_Utility.cpp
        library/opengl/OpenGL_Utility.h
        game/Game.cpp
        game/Game.h
        game/GameRenderer.cpp
        game/GameRenderer.h
        library/DebugLog.h
        library/platform/Platform.h
        library/platform/PlatformGLFW.cpp
        library/DataStructures.h
        library/opengl/OpenGL_ImmediateRenderer.h
        library/opengl/OpenGL_ImmediateRenderer.cpp
        library/Input.cpp
        library/Input.h
        stb/stb_truetype.h
        library/gui/GUI.cpp
        library/gui/GUI.h
        Application.cpp
        library/Application.h
        library/opengl/OpenGL_GUI_Impl.cpp library/Color.h library/Color.cpp stb/stb_truetype.h.cpp projects/noise/NoiseProject.cpp projects/noise/NoiseProject.h library/math/Ray3.cpp library/math/Ray3.h library/math/Random.h library/Noise.h library/Noise.cpp library/Animations.cpp library/Animations.h
        projects/snake/SnakeGame.h projects/snake/SnakeGame.cpp)

find_library(GLFW glfw3 lib)

target_link_libraries(ProceduralWorld LINK_PUBLIC ${GLFW})
include_directories(library)
include_directories(library/math)

include_directories(include)