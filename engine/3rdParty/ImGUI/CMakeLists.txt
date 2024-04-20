cmake_minimum_required(VERSION 3.14)

project(ImGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} STATIC
	src/imgui.cpp
	src/imgui_demo.cpp
	src/imgui_draw.cpp
	src/imgui_impl_sdl2.cpp
	src/imgui_impl_sdlrenderer2.cpp
	src/imgui_plot.cpp
	src/imgui_tables.cpp
	src/imgui_widgets.cpp
	src/implot.cpp
	src/implot_items.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

include(FetchContent)
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
		set(SDL2_INCLUDE_DIR ${sdl2_SOURCE_DIR}/include)
		if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
			set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x64/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x64/SDL2main.lib")
			set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x64/SDL2.dll")
		else ()
			set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x86/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x86/SDL2main.lib")
			set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x86/SDL2.dll")
		endif ()
	endif ()

	target_include_directories(${PROJECT_NAME} PRIVATE ${sdl2_SOURCE_DIR}/include)
	target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES})

elseif (EMSCRIPTEN)
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
endif()

target_compile_options(${PROJECT_NAME} PRIVATE
	    $<$<CXX_COMPILER_ID:MSVC>:/W0>
		$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-w>
)
