cmake_minimum_required(VERSION 3.10)
project("CITB520_PlantsVsZombies" VERSION 1.0.0)

# https://cmake.org/cmake/help/latest/prop_tgt/CXX_STANDARD.html
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

# Include external libraries
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GL3W)
add_subdirectory("libs/imgui-docking")

add_subdirectory("libs/glm-0.9.9.8")

set( SDL_STATIC ON CACHE BOOL "" FORCE )
set( SDL_SHARED OFF CACHE BOOL "" FORCE )
add_subdirectory("libs/SDL2-2.0.12")

# executable
add_executable(${PROJECT_NAME}
    # "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "src/main.cpp"
    "src/ComponentSystem/NativeScriptSystem.cpp"
    "src/ComponentSystem/PhysicsSystem.cpp"
    "src/FileLoaders/RawImage.cpp"
    "src/Game/Game.cpp"
    "src/Game/ImGuiMenu.cpp"
    "src/Game/NativeScripts/LawnMowerScript.cpp"
    "src/Game/NativeScripts/PlantScript.cpp"
    "src/Game/NativeScripts/PlantSpawnScript.cpp"
    "src/Game/NativeScripts/ZombieScript.cpp"
    "src/Game/NativeScripts/ZombieSpawnScript.cpp"
    "src/Physics/Colliders.cpp"
    "src/Rendering/BatchRecorder.cpp"
    "src/Rendering/BatchRenderer.cpp"
    "src/Rendering/OpenGLWrapper/BufferGPU.cpp"
    "src/Rendering/OpenGLWrapper/Shader.cpp"
    "src/Rendering/OpenGLWrapper/ShaderProgram.cpp"
    "src/Rendering/OpenGLWrapper/TextureGPU.cpp"
    "src/Rendering/OpenGLWrapper/VAO.cpp"
    "src/Rendering/OpenGLWrapper/DebugGL.cpp"
 	"src/Game/NativeScripts/BulletSpawnScript.cpp"
	"src/Game/NativeScripts/BulletScript.cpp"
    "src/Game/NativeScripts/SunScript.cpp"
    "src/Game/NativeScripts/SunSpawnScript.cpp"
    "src/Game/NativeScripts/WaveControllerScript.cpp"
    "src/Game/NativeScripts/SunCollector.cpp")

# OpenGL
find_package(OpenGL REQUIRED)

if(NOT ${OPENGL_FOUND})
    message("OPENGL not found")
endif()

message("opengl include ${OPENGL_INCLUDE_DIR} lib ${OPENGL_LIBRARIES}")

target_include_directories(${PROJECT_NAME} PUBLIC
    "${OPENGL_INCLUDE_DIR}"
    "libs"
    "src"
)

target_link_libraries(${PROJECT_NAME}
    ImGui

    # GL
    ${OPENGL_LIBRARIES}
    SDL2main
    SDL2-static

    # GLM
    glm
)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets")
