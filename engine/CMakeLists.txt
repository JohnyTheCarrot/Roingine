cmake_minimum_required(VERSION 3.14)

project(RoingusEngine LANGUAGES CXX)

if (EMSCRIPTEN)
	message("Using Emscripten!")
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(RoingusEngine STATIC
  src/roingus_engine.cpp

)

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_compile_definitions(RoingusEngine PRIVATE ROINGUSENGINE_LIBRARY)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_options(${PROJECT_NAME} PRIVATE /diagnostics:caret)
endif()


# Libraries
include(FetchContent)

# ImGUI
target_link_libraries(${PROJECT_NAME} PUBLIC ImGUI)

# GLM
FetchContent_Declare(
	    glm
		URL https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip
		DOWNLOAD_NO_PROGRESS ON
		DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)

FetchContent_GetProperties(glm)
if (NOT glm_POPULATED)
	FetchContent_Populate(glm)
endif ()


# SDL2
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
		set(SDL2_INCLUDE_DIR ${sdl2_SOURCE_DIR}/include)
		if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
			set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x64/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x64/SDL2main.lib")
			set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x64/SDL2.dll")
		else ()
			set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x86/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x86/SDL2main.lib")
			set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x86/SDL2.dll")
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
		3rdParty
	)

    # set libraries to link with
	target_link_libraries(${PROJECT_NAME} PUBLIC
		${SDL2_LIBRARIES}
		${SDL2_IMAGE_LIBRARIES}
		${SDL2_TTF_LIBRARIES}
		${VLD_LIBRARY}
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
    target_link_libraries("${PROJECT_NAME}" PUBLIC
		SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf
	)
endif ()


if (EMSCRIPTEN)
	message(FATAL_ERROR "TODO")
elseif(WIN32)
	# add post build commands
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
	)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_IMAGE_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
	)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_TTF_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
	)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/EngineResources" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/EngineResources"
	)
endif()
