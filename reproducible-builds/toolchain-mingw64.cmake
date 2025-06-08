set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# specify the cross compiler
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc-posix)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++-posix)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# where is the target environment
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# use static libgcc libstdc++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

# List of Qt tools to override
set(QT_TOOLS moc uic rcc lrelease)

foreach(tool IN LISTS QT_TOOLS)
    # 1) Find host-side (Linux) binary
    find_program(
        QT_${tool}_EXECUTABLE
        NAMES ${tool}
        PATHS /usr/bin
        NO_DEFAULT_PATH
    )
    if(NOT QT_${tool}_EXECUTABLE)
        message(FATAL_ERROR "Host Qt tool not found: ${tool}")
    endif()

    # 2) Create imported target only if it doesn't already exist
    if(NOT TARGET Qt5::${tool})
        add_executable(Qt5::${tool} IMPORTED)
    endif()

    # 3) Override its location to point at the native tool
    set_property(
        TARGET Qt5::${tool}
        PROPERTY IMPORTED_LOCATION "${QT_${tool}_EXECUTABLE}"
    )
endforeach()