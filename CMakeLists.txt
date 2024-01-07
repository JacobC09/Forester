# == Project == #

cmake_minimum_required(VERSION 3.0)
project(MedivalCityBuilder)

# == Sub Directories == #

add_subdirectory("external/raylib")
add_subdirectory("external/entt")

# == Executable == #

set(PRELOAD_ASSET_DIR "assets")

add_executable(game 
    "src/main.cpp"
    "src/game.cpp"
    "src/textures.cpp"
    "src/systems.cpp"
    "src/animation.cpp"
    "src/entity/factory.cpp"
    "src/entity/render.cpp"
)

set(CMAKE_BUILD_TYPE Release)
set(BUILD_EXAMPLES OFF)

target_include_directories(game PRIVATE "include")
target_precompile_headers(game PUBLIC "include/common.h")
target_link_libraries(game PRIVATE raylib EnTT::EnTT)

if (NOT MSVC)
    target_compile_options(game PRIVATE -std=c++20 -Wall)
else()
    target_compile_options(game PRIVATE /std:c++20 /W3 /wd4244)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# == Copy Assets == #

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/${PRELOAD_ASSET_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${PRELOAD_ASSET_DIR}
)

add_dependencies(game copy_assets)

# == Emscripten == #

if (EMSCRIPTEN) # emcmake cmake .. -DPLATFORM=Web && cmake --build .
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -std=c++17 -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ${PRELOAD_DIR} --shell-file 'C:/Users/jakec/Documents/Coding/shell.html'")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()
