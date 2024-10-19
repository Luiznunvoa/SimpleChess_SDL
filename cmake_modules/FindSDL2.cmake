# SDL2 search paths
SET(SDL2_SEARCH_PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw # Fink
        /opt/local # DarwinPorts
        /opt/csw # Blastwave
        /opt
        ${SDL2_PATH}
)

# SDL2 include directory
FIND_PATH(SDL2_INCLUDE_DIR SDL.h
        HINTS
        $ENV{SDL2DIR}
        PATH_SUFFIXES include/SDL2 include
        PATHS ${SDL2_SEARCH_PATHS}
)

# SDL2_ttf include directory
FIND_PATH(SDL2_TTF_INCLUDE_DIR SDL_ttf.h
        HINTS
        $ENV{SDL2DIR}
        PATH_SUFFIXES include/SDL2 include
        PATHS ${SDL2_SEARCH_PATHS}
)

# Set path suffixes based on architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PATH_SUFFIXES lib64 lib/x64 lib)
else()
    set(PATH_SUFFIXES lib/x86 lib)
endif()

# SDL2 library
FIND_LIBRARY(SDL2_LIBRARY_TEMP
        NAMES SDL2
        HINTS
        $ENV{SDL2DIR}
        PATH_SUFFIXES ${PATH_SUFFIXES}
        PATHS ${SDL2_SEARCH_PATHS}
)

# SDL2_ttf library
FIND_LIBRARY(SDL2_TTF_LIBRARY_TEMP
        NAMES SDL2_ttf
        HINTS
        $ENV{SDL2DIR}
        PATH_SUFFIXES ${PATH_SUFFIXES}
        PATHS ${SDL2_SEARCH_PATHS}
)

IF(NOT SDL2_BUILDING_LIBRARY)
    IF(NOT ${SDL2_INCLUDE_DIR} MATCHES ".framework")
        # Non-OS X framework versions expect you to also dynamically link to SDL2main
        FIND_LIBRARY(SDL2MAIN_LIBRARY
                NAMES SDL2main
                HINTS
                $ENV{SDL2DIR}
                PATH_SUFFIXES ${PATH_SUFFIXES}
                PATHS ${SDL2_SEARCH_PATHS}
        )
    ENDIF(NOT ${SDL2_INCLUDE_DIR} MATCHES ".framework")
ENDIF(NOT SDL2_BUILDING_LIBRARY)

# Find threads for non-Apple systems
IF(NOT APPLE)
    FIND_PACKAGE(Threads)
ENDIF(NOT APPLE)

# MinGW additional link flag
IF(MINGW)
    SET(MINGW32_LIBRARY mingw32 "-mwindows" CACHE STRING "mwindows for MinGW")
ENDIF(MINGW)

IF(SDL2_LIBRARY_TEMP)
    IF(NOT SDL2_BUILDING_LIBRARY)
        IF(SDL2MAIN_LIBRARY)
            SET(SDL2_LIBRARY_TEMP ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY_TEMP})
        ENDIF(SDL2MAIN_LIBRARY)
    ENDIF(NOT SDL2_BUILDING_LIBRARY)

    IF(APPLE)
        SET(SDL2_LIBRARY_TEMP ${SDL2_LIBRARY_TEMP} "-framework Cocoa")
    ENDIF(APPLE)

    IF(NOT APPLE)
        SET(SDL2_LIBRARY_TEMP ${SDL2_LIBRARY_TEMP} ${CMAKE_THREAD_LIBS_INIT})
    ENDIF(NOT APPLE)

    IF(MINGW)
        SET(SDL2_LIBRARY_TEMP ${MINGW32_LIBRARY} ${SDL2_LIBRARY_TEMP})
    ENDIF(MINGW)

    SET(SDL2_LIBRARY ${SDL2_LIBRARY_TEMP} CACHE STRING "Where the SDL2 Library can be found")
    SET(SDL2_LIBRARY_TEMP "${SDL2_LIBRARY_TEMP}" CACHE INTERNAL "")
ENDIF(SDL2_LIBRARY_TEMP)

# SDL2_ttf library final setting
IF(SDL2_TTF_LIBRARY_TEMP)
    SET(SDL2_TTF_LIBRARY ${SDL2_TTF_LIBRARY_TEMP} CACHE STRING "Where the SDL2_ttf Library can be found")
    SET(SDL2_TTF_LIBRARY_TEMP "${SDL2_TTF_LIBRARY_TEMP}" CACHE INTERNAL "")
ENDIF(SDL2_TTF_LIBRARY_TEMP)

# FindPackage handle for SDL2 and SDL2_ttf
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2 REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR)
