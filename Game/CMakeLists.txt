add_executable(${TARGET_NAME} ${TARGET_PARAM}

        #Core files
        Main.cpp

        #Components
        Components/ThrashCacheComponent.cpp
        Components/LivesDisplayComponent.cpp
        Components/ScoreDisplayComponent.cpp
        Components/LivesComponent.cpp
        Components/ScoreComponent.cpp
        Components/UiAutoCycleComponent.cpp
        Components/GameInputComponent.cpp
        Components/UiGameModeSelector.cpp
        Components/CollisionGridComponent.cpp

        #Commands
        Commands/GameCommands.cpp

        #States
        States/GSMainMenu.cpp
        States/GSManager.cpp
        States/GSLobby.cpp
        States/GMSSinglePlayer.cpp
        States/GMSCoop.cpp
        States/GMSVersus.cpp
)

set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug"
        OUTPUT_NAME ${TARGET_NAME}
)

# To force it to JUST one specific folder regardless of config:
set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Inherit everything from the Engine
target_link_libraries(${TARGET_NAME} PRIVATE Minigin)
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

# Platform Specific Linkage
if(NOT EMSCRIPTEN)
    target_link_libraries(${TARGET_NAME} PUBLIC xinput)
endif()

if (MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else ()
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Werror -Wno-unknown-pragmas)
endif ()

# --- Post-Build (Copying Data & DLLs) ---
if (EMSCRIPTEN)
    set_target_properties(${TARGET_NAME} PROPERTIES
            LINK_FLAGS "-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1 -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/Data@/\""
    )
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
elseif (WIN32)
  set_target_properties(${TARGET_NAME} PROPERTIES
          RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
          RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
          RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
  )

  foreach (dllTarget SDL3::SDL3 SDL3_ttf::SDL3_ttf)
      add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
              COMMAND ${CMAKE_COMMAND} -E copy_if_different
              "$<TARGET_FILE:${dllTarget}>"
              "${CMAKE_BINARY_DIR}/bin")
  endforeach ()

  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/Data"
          "${CMAKE_BINARY_DIR}/bin/Data")
endif ()

if(WIN32 AND NOT MSVC)
    target_link_options(${TARGET_NAME} PRIVATE -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,-Bdynamic,--no-whole-archive)
endif()