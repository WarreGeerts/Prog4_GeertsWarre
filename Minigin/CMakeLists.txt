# Create ImGui Helper Library
add_library(imgui_internal STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)
target_include_directories(imgui_internal PUBLIC ${imgui_SOURCE_DIR} ${sdl3_SOURCE_DIR}/include)
target_link_libraries(imgui_internal PUBLIC SDL3::SDL3)
target_compile_features(imgui_internal PUBLIC cxx_std_20)

# Build the Engine
add_library(Minigin STATIC
        #core files
        Font.cpp
        GameObject.cpp
        Minigin.cpp
        Scene.cpp
        Texture2D.cpp

        #subfolder files
        Input/InputManager.cpp
        Input/Controller.cpp
        Singletons/RealTimeEditor/EditorGui.cpp
        Singletons/Renderer.cpp
        Singletons/ResourceManager.cpp
        Singletons/SceneManager.cpp
        Singletons/RealTimeEditor/SceneSerializer.cpp
        Singletons/RealTimeEditor/SceneSerializer.h
        Singletons/RealTimeEditor/ComponentFactory.cpp
        Singletons/RealTimeEditor/ComponentFactory.h

        #components
        Components/TransformComponent.cpp
        Components/RenderComponent.cpp
        Components/FPSComponent.cpp
        Components/TextComponent.cpp
        Components/RotateComponent.cpp
        Components/CharacterControllerComponent.cpp
        Components/SpriteComponent.cpp
        Components/Component.cpp

        #sound
        Sound/servicelocator.cpp
        Sound/SdlSoundSystem.cpp

        # External Source
        ${implot_SOURCE_DIR}/implot.cpp
        ${implot_SOURCE_DIR}/implot_items.cpp
        Singletons/EventManager.cpp
        Singletons/DeltaTime.cpp
        Commands/GameActorCommand.cpp
        Commands/MoveCommand.cpp
        Sound/SoundSystem.cpp
)

# Propagate headers and dependencies to anyone who links to us (the Game)
target_include_directories(Minigin PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${imgui_SOURCE_DIR}
        ${implot_SOURCE_DIR}
        ${VLD_INCLUDE_DIRS}
        ${imgui_filebrowser_SOURCE_DIR}
)

target_link_libraries(Minigin PUBLIC
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        glm::glm
        nlohmann_json::nlohmann_json
        imgui_internal
        ${VLD_LIBRARIES}
)