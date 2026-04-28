cmake_minimum_required(VERSION 3.26)
project(MiniginWorkspace)

# --- 1. Global Platform Setup ---
if (EMSCRIPTEN)
    set(TARGET_NAME BurgerTime_web)
    set(TARGET_PARAM "")

    # TRICK: Fool SDL3_ttf so it skips find_package(Freetype REQUIRED)
    # We provide dummy values because Emscripten handles the real linking via -sUSE_FREETYPE
    set(Freetype_FOUND TRUE CACHE BOOL "" FORCE)
    set(FREETYPE_FOUND TRUE CACHE BOOL "" FORCE)
    set(FREETYPE_INCLUDE_DIRS "EMSCRIPTEN_BUILTIN" CACHE PATH "" FORCE)
    set(FREETYPE_LIBRARIES "EMSCRIPTEN_BUILTIN" CACHE STRING "" FORCE)

    # Do the same for Harfbuzz to keep things consistent
    set(harfbuzz_FOUND TRUE CACHE BOOL "" FORCE)
    set(HarfBuzz_FOUND TRUE CACHE BOOL "" FORCE)
    set(harfbuzz_INCLUDE_DIRS "EMSCRIPTEN_BUILTIN" CACHE PATH "" FORCE)
    set(harfbuzz_LIBRARIES "EMSCRIPTEN_BUILTIN" CACHE STRING "" FORCE)
else ()
    set(TARGET_NAME BurgerTime)
    set(TARGET_PARAM WIN32)
endif ()

# --- 2. Dependencies (FetchContent) ---
include(FetchContent)

# GLM
find_package(glm CONFIG QUIET)
if (NOT glm_FOUND)
    FetchContent_Declare(glm URL https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip)
    FetchContent_MakeAvailable(glm)
endif ()

# SDL3
find_package(SDL3 3.4 CONFIG QUIET)
if (NOT SDL3_FOUND)
    if (WIN32)
        FetchContent_Declare(SDL3 URL https://www.libsdl.org/release/SDL3-devel-3.4.0-VC.zip)
        FetchContent_MakeAvailable(SDL3)
        list(PREPEND CMAKE_PREFIX_PATH "${sdl3_SOURCE_DIR}")
        find_package(SDL3 CONFIG REQUIRED)
    else ()
        FetchContent_Declare(SDL3 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git GIT_TAG release-3.4.0)
        FetchContent_MakeAvailable(SDL3)
    endif ()
endif ()

# SDL3_ttf
find_package(SDL3_ttf 3.2.2 CONFIG QUIET)
if (NOT SDL3_ttf_FOUND)
    if (WIN32)
        FetchContent_Declare(SDL3_ttf URL https://www.libsdl.org/projects/SDL_ttf/release/SDL3_ttf-devel-3.2.2-VC.zip)
        FetchContent_MakeAvailable(SDL3_ttf)
        list(PREPEND CMAKE_PREFIX_PATH "${sdl3_ttf_SOURCE_DIR}")
        find_package(SDL3_ttf CONFIG REQUIRED)
    else ()
        FetchContent_Declare(SDL3_ttf GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git GIT_TAG release-3.2.2)

        if (EMSCRIPTEN)
            # Tell SDL3_ttf to use the "System" (which we spoofed above)
            set(SDL3TTF_VENDORED OFF CACHE BOOL "" FORCE)
            set(SDL3TTF_SYSTEM_FREETYPE ON CACHE BOOL "" FORCE)
            set(SDL3TTF_HARFBUZZ ON CACHE BOOL "" FORCE)

            add_compile_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
            add_link_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
        endif ()

        FetchContent_MakeAvailable(SDL3_ttf)
    endif ()
endif ()

# ImGui & ImPlot
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui GIT_TAG v1.92.5-docking)
FetchContent_Declare(implot GIT_REPOSITORY https://github.com/epezent/implot.git GIT_TAG v0.17)
FetchContent_MakeAvailable(imgui implot)

# --- 3. Optional Features (VLD/Steam) ---
if (WIN32)
    find_package(VLD CONFIG)
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/steam")
find_package(Steamworks QUIET)

# --- 4. Add Subdirectories ---
add_subdirectory(Minigin)
add_subdirectory(Game)