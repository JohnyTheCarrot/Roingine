cmake_minimum_required(VERSION 3.26)

project(Roingine LANGUAGES CXX)

if (EMSCRIPTEN)
    message("Using Emscripten!")
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(3rdParty)

set(INCLUDE_HEADERS_COMPONENTS
        include/roingine/components/component.h
        include/roingine/components/rect_renderer.h
        include/roingine/components/transform.h
        include/roingine/components/rect_collider.h
        include/roingine/components/texture_renderer.h
        include/roingine/components/animation_renderer.h
        include/roingine/components/camera.h
)

set(INCLUDE_HEADERS_COMMANDS
        include/roingine/commands/command.h
        include/roingine/commands/gameobject_command.h
        include/roingine/commands/registered_keyboard_command.h
        include/roingine/commands/registered_controller_command.h
)

set(INCLUDE_HEADERS
        include/roingine/game_time.h
        include/roingine/keyboard_input.h
        include/roingine/scene.h
        include/roingine/scene_manager.h
        include/roingine/gameobject.h
        include/roingine/roingine.h
        include/roingine/singleton.h
        include/roingine/gameobject_types.h
        include/roingine/pair_hash.h
        include/roingine/event_queue.h
        include/roingine/service_locator.h
        include/roingine/audio_service.h
        include/roingine/sound_clip.h
        include/roingine/game_info.h
        include/roingine/engine_event_queue.h
        include/roingine/component_init_types.h
        include/roingine/gl_texture.h
        include/roingine/controller.h
        include/roingine/image_scaling.h
        include/roingine/reusable_texture.h
        include/roingine/music.h
)

set(SRC_FILES
        src/roingine.cpp
        src/game_time.cpp
        src/keyboard_input.cpp
        src/scene.cpp
        src/scene_manager.cpp
        src/gameobject.cpp
        src/sound_impl.cpp
        src/components/rect_renderer.cpp
        src/components/transform.cpp
        src/components/rect_collider.cpp
        src/commands/gameobject_command.cpp
        src/commands/registered_keyboard_command.cpp
        src/game_info.cpp
        src/component_init_types.cpp
        src/components/texture_renderer.cpp
        src/components/animation_renderer.cpp
        src/sdl_smart_pointers.cpp
        src/gl_texture.cpp
        src/components/camera.cpp
        src/controller.cpp
        src/unique_sdl_game_controller.cpp
        src/commands/registered_controller_command.cpp
        src/sdl_context.cpp
        src/image_scaling.cpp
        src/reusable_texture.cpp
        src/components/component.cpp
        src/music.cpp
)

set(INTERNAL_HEADERS
        internal_headers/keyboard_input_impl.h
        internal_headers/scene_manager_impl.h
        internal_headers/sound_impl.h
        internal_headers/scene_loader_impl.h
        internal_headers/sdl_smart_pointers.h
        internal_headers/controller_impl.h
        internal_headers/unique_sdl_game_controller.h
        internal_headers/sdl_context.h
        internal_headers/music_impl.h
)

add_library(${PROJECT_NAME} STATIC
        ${INTERNAL_HEADERS}
        ${SRC_FILES}
        ${INCLUDE_HEADERS}
        ${INCLUDE_HEADERS_COMPONENTS}
        ${INCLUDE_HEADERS_COMMANDS}
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>

        PRIVATE
        internal_headers
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ROINGUSENGINE_LIBRARY)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /diagnostics:caret)
endif ()

target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

# Libraries
include(FetchContent)

# ImGUI
target_link_libraries(${PROJECT_NAME} PRIVATE "$<BUILD_LOCAL_INTERFACE:ImGUI>")

# GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
        GIT_SHALLOW TRUE
        SYSTEM
)

FetchContent_MakeAvailable(glm)

# Nlohmann JSON
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

find_package(OpenGL REQUIRED)

# add SDL2
if (WIN32)
    FetchContent_Declare(
            SDL2
            URL https://www.libsdl.org/release/SDL2-devel-2.28.5-VC.zip
            DOWNLOAD_NO_PROGRESS ON
            DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_GetProperties(SDL2)
    if (NOT sdl2_POPULATED)
        FetchContent_Populate(SDL2)
    endif ()

    set(SDL2_INCLUDE_DIR ${sdl2_SOURCE_DIR}/include)
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x64/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x64/SDL2main.lib")
        set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x64/SDL2.dll")
    else ()
        set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x86/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x86/SDL2main.lib")
        set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x86/SDL2.dll")
    endif ()

    FetchContent_Declare(
            SDL2_mixer
            URL https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.4-VC.zip
            DOWNLOAD_NO_PROGRESS ON
            DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_GetProperties(SDL2_mixer)
    if (NOT sdl2_mixer_POPULATED)
        FetchContent_Populate(SDL2_mixer)
        set(SDL2_MIXER_INCLUDE_DIR ${sdl2_mixer_SOURCE_DIR}/include)
        if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
            set(SDL2_MIXER_LIBRARIES "${sdl2_mixer_SOURCE_DIR}/lib/x64/SDL2_mixer.lib")
            set(SDL2_MIXER_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x64/SDL2_mixer.dll")
        else ()
            set(SDL2_MIXER_LIBRARIES "${sdl2_mixer_SOURCE_DIR}/lib/x86/SDL2_mixer.lib")
            set(SDL2_MIXER_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x86/SDL2_mixer.dll")
        endif ()
    endif ()

    # add SDL2_image
    FetchContent_Declare(
            SDL2_image
            URL https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.8.2-VC.zip
            DOWNLOAD_NO_PROGRESS ON
            DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_GetProperties(SDL2_image)
    if (NOT SDL2_image_POPULATED)
        FetchContent_Populate(SDL2_image)
        set(SDL2_IMAGE_INCLUDE_DIR ${sdl2_image_SOURCE_DIR}/include)
        if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
            set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.lib")
            set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.dll")
        else ()
            set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.lib")
            set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.dll")
        endif ()
    endif ()

    # add SDL2_ttf
    FetchContent_Declare(
            SDL2_ttf
            URL https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.20.2-VC.zip
            DOWNLOAD_NO_PROGRESS ON
            DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_GetProperties(SDL2_ttf)
    if (NOT sdl2_ttf_POPULATED)
        FetchContent_Populate(SDL2_ttf)
        set(SDL2_TTF_INCLUDE_DIR ${sdl2_ttf_SOURCE_DIR}/include)
        if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
            set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.lib)
            set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.dll")
        else ()
            set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.lib)
            set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.dll")
        endif ()
    endif ()

    set(VLD_DIR "$ENV{ProgramFiles\(x86\)}/Visual Leak Detector")
    IF (EXISTS ${VLD_DIR})
        set(VLD_INCLUDE_DIR "${VLD_DIR}/include")
        if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
            set(VLD_LIBRARY "${VLD_DIR}/lib/Win64/vld.lib")
        else ()
            set(VLD_LIBRARY "${VLD_DIR}/lib/Win32/vld.lib")
        endif ()
    endif ()

    set(BUILD_SHARED_LIBS FALSE)

    # set all include directories
    target_include_directories(${PROJECT_NAME} PRIVATE
            ${VLD_INCLUDE_DIR}
            ${sdl2_SOURCE_DIR}/include
            ${sdl2_image_SOURCE_DIR}/include
            ${SDL2_TTF_INCLUDE_DIR}
            ${sdl2_mixer_SOURCE_DIR}/include
            3rdParty
    )

    # set libraries to link with
    target_link_libraries(${PROJECT_NAME} PUBLIC
            ${SDL2_LIBRARIES}
            ${SDL2_IMAGE_LIBRARIES}
            ${SDL2_TTF_LIBRARIES}
            ${SDL2_MIXER_LIBRARIES}
            ${OPENGL_LIBRARIES}
            ${VLD_LIBRARY}
            glm::glm
    )

elseif (NOT EMSCRIPTEN)
    FetchContent_Declare(
            SDL2
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-2.28.5
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(SDL2)

    FetchContent_Declare(
            SDL2_image
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
            GIT_TAG release-2.8.2
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    set(SDL2IMAGE_INSTALL OFF)
    FetchContent_MakeAvailable(SDL2_image)

    FetchContent_Declare(
            SDL2_ttf
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
            GIT_TAG release-2.20.2
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
    )
    set(SDL2TTF_INSTALL OFF)
    FetchContent_MakeAvailable(SDL2_ttf)

    target_include_directories("${PROJECT_NAME}" PUBLIC
            ${sdl2_SOURCE_DIR}/include
            ${sdl2_image_SOURCE_DIR}/include
            ${sdl2_ttf_SOURCE_DIR}
    )
    target_link_libraries("${PROJECT_NAME}" PRIVATE
            SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf
    )
endif ()

if (EMSCRIPTEN)
    set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -g")
    set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/Data@/\" -sSDL2_IMAGE_FORMATS=['tga','png']")

    # work around https://github.com/emscripten-core/emscripten/issues/11513
    set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -fno-stack-protector")

    # Use the Emscripten toolchain
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/EmscriptenToolchain.cmake)

    # Additional settings for Emscripten build
    set_target_properties(${TARGET_NAME} PROPERTIES
            COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
            LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
    )

    # Have emscripten generate a html page too.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
elseif (WIN32)
endif ()
