cmake_minimum_required(VERSION 3.23)
project(SOWITZ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PULSE REQUIRED IMPORTED_TARGET libpulse)
pkg_check_modules(FFTW  REQUIRED IMPORTED_TARGET fftw3f)
pkg_check_modules(SDL REQUIRED IMPORTED_TARGET sdl3)


add_subdirectory(portaudio)

add_executable(sowitz
    src/main.cpp
    src/pulse_audio_handler.cpp
    src/fft.cpp
    src/port_audio_handler.cpp
    src/sowitz.cpp
    src/sdl_operators.cpp
)

target_link_libraries(sowitz PRIVATE
    portaudio
    PkgConfig::PULSE
    PkgConfig::FFTW
    PkgConfig::SDL
)

target_include_directories(sowitz PRIVATE lib)


# cmake_minimum_required
#         ↓
# project
#         ↓
# global configuration
#         ↓
# find packages / dependencies
#         ↓
# add your subprojects
#         ↓
# create executable/library
#         ↓
# configure target
#         ↓
# link dependencies
