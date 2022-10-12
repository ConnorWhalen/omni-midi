cmake_minimum_required(VERSION 3.6)
project(omni)

set(CMAKE_CXX_STANDARD 17)

find_library(SDL2 SDL2)
if (NOT SDL2)
    message(FATAL_ERROR "SDL2 not found")
endif()

find_library(SDL_TTF SDL2_ttf)
if (NOT SDL_TTF)
    message(FATAL_ERROR "SDL_TTF not found")
endif()

find_library(SDL_IMAGE SDL2_image)
if (NOT SDL_IMAGE)
    message(FATAL_ERROR "SDL_IMAGE not found")
endif()

find_library(YAML_CPP yaml-cpp)
if (NOT YAML_CPP)
    message(FATAL_ERROR "YAML_CPP not found")
endif()

find_package(RtMidi)
find_package(Threads REQUIRED)

add_executable(omni
        # controller
        src/controller/Controller.h
        src/controller/MidiControl.cpp
        src/controller/MidiControl.h
        src/controller/OmniController.cpp
        src/controller/OmniController.h
        # device
        src/device/Device.h
        src/device/OmniDevice.cpp
        src/device/OmniDevice.h
        # input
        src/input/Input.h
        src/input/KeyboardInput.cpp
        src/input/KeyboardInput.h
        src/input/TouchInput.cpp
        src/input/TouchInput.h
        # UI
        src/UI/Button.cpp
        src/UI/Button.h
        src/UI/Harp.cpp
        src/UI/Harp.h
        src/UI/TextArea.cpp
        src/UI/TextArea.h
        src/UI/Window.cpp
        src/UI/Window.h
        src/UI/WindowArea.h
        # src
        src/Config.cpp
        src/Config.h
        src/main.cpp
        )

message("${SDL2}")
message("${SDL_TTF}")
message("${SDL_IMAGE}")
message("${YAML_CPP}")
target_link_libraries(omni ${SDL2})
target_link_libraries(omni ${SDL_TTF})
target_link_libraries(omni ${SDL_IMAGE})
target_link_libraries(omni ${YAML_CPP})
target_link_libraries(omni RtMidi::rtmidi)
target_link_libraries(omni Threads::Threads)
include_directories(/usr/local/include)

