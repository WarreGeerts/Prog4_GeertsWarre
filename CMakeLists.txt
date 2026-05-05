cmake_minimum_required(VERSION 3.26)
project(MiniginWorkspace)

# --- 1. Global Platform Setup ---
if (EMSCRIPTEN)
    set(TARGET_NAME BurgerTime_web)
    add_compile_options("-sUSE_FREETYPE=1" "-sUSE_HARFBUZZ=1")
    add_link_options("-sUSE_FREETYPE=1" "-sUSE_HARFBUZZ=1")
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

# Add SDL3_ttf
find_package(SDL3_ttf 3.2.2 CONFIG QUIET)
if (NOT SDL3_ttf_FOUND)
    if (WIN32)
        FetchContent_Declare(SDL3_ttf URL https://www.libsdl.org/projects/SDL_ttf/release/SDL3_ttf-devel-3.2.2-VC.zip)
        FetchContent_MakeAvailable(SDL3_ttf)
        list(PREPEND CMAKE_PREFIX_PATH "${sdl3_ttf_SOURCE_DIR}")
        find_package(SDL3_ttf CONFIG REQUIRED)
    else ()
        FetchContent_Declare(SDL3_ttf GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git GIT_TAG release-3.2.2)
        set(SDL3TTF_INSTALL OFF)
        if (EMSCRIPTEN)
            list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/emscripten")
            set(SDL3TTF_USE_SYSTEM_FREETYPE ON)
            set(SDL3TTF_VENDORED OFF)
            set(SDLTTF_PLUTOSVG OFF)
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

# ImGui FileBrowser
FetchContent_Declare(
        imgui_filebrowser
        GIT_REPOSITORY https://github.com/AirGuanZ/imgui-filebrowser.git
        GIT_TAG master
)
FetchContent_MakeAvailable(imgui_filebrowser)

# nlohmann json
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# --- 3. Optional Features (VLD/Steam) ---
if (WIN32)
    find_package(VLD CONFIG)
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/steam")
find_package(Steamworks QUIET)

# --- 4. Add Subdirectories ---
add_subdirectory(Minigin)
add_subdirectory(Game)

# --- 5. Install Rules for Release Packaging ---
if (NOT EMSCRIPTEN)
    # Install the executable
    install(TARGETS ${TARGET_NAME}
            RUNTIME DESTINATION .
    )

    # Install Data folder
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/Data"
            DESTINATION .
    )

    # Install SDL3 DLLs
    if (WIN32)
        install(FILES $<TARGET_FILE:SDL3::SDL3>
                DESTINATION .
        )
        install(FILES $<TARGET_FILE:SDL3_ttf::SDL3_ttf>
                DESTINATION .
        )
    endif ()
endif ()

# --- 6. CPack Configuration for Creating Zip ---
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "BurgerTime")
set(CPACK_PACKAGE_VENDOR "YourName")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")
include(CPack)