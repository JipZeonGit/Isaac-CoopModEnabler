cmake_minimum_required(VERSION 3.15)
project(CoopModEnabler C)

# Force 32-bit on MSVC (use: cmake -A Win32 ..)
# Force 32-bit on GCC/MinGW (automatic via -m32)
if(MINGW OR (CMAKE_C_COMPILER_ID STREQUAL "GNU"))
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32")
endif()

add_library(CoopModEnabler SHARED
    src/dllmain.c
    src/patcher.c
    src/winmm_proxy.c
)

# Output as winmm.dll (no "lib" prefix)
set_target_properties(CoopModEnabler PROPERTIES
    OUTPUT_NAME "winmm"
    PREFIX ""
    SUFFIX ".dll"
)

# Use the module definition file for exports
if(MSVC)
    target_sources(CoopModEnabler PRIVATE exports.def)
else()
    # For MinGW, pass .def to linker
    target_link_options(CoopModEnabler PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/exports.def")
endif()

target_link_libraries(CoopModEnabler PRIVATE kernel32)

# C99 standard
set_property(TARGET CoopModEnabler PROPERTY C_STANDARD 99)
