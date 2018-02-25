project(apulse)
cmake_minimum_required (VERSION 2.8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -Wall -fPIC -fvisibility=hidden")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=implicit-function-declaration")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

find_package(PkgConfig REQUIRED)

pkg_check_modules(REQ glib-2.0 alsa REQUIRED)

list(APPEND REQ_LIBRARIES m)

set(WITH_TRACE 0 CACHE STRING "trace level: no tracing (0) / unimplemented only (1) / all (2)")
set(LOG_TO_STDERR 0 CACHE STRING "send all log output to stderr")

# Options.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_TRACE_LEVEL=${WITH_TRACE}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_LOG_TO_STDERR=${LOG_TO_STDERR}")

include_directories(${REQ_INCLUDE_DIRS})

set(USE_BUNDLED_PULSEAUDIO_HEADERS 1 CACHE BOOLEAN "Use bundled PulseAudio headers instead of system ones")
if (${USE_BUNDLED_PULSEAUDIO_HEADERS})
    include_directories(3rdparty/pulseaudio-headers)
else()
    pkg_check_modules(PA libpulse>=5.0 REQUIRED)
    include_directories(${PA_INCLUDE_DIRECTORIES})
endif()

link_directories(${REQ_LIBRARY_DIRS})

add_library(trace-helper STATIC
    src/trace.c
)

add_library(pulse SHARED
    src/apulse-channel-map.c
    src/apulse-common.c
    src/apulse-context.c
    src/apulse-ext.c
    src/apulse-format-info.c
    src/apulse-mainloop.c
    src/apulse-misc.c
    src/apulse-operation.c
    src/apulse-proplist.c
    src/apulse-signal.c
    src/apulse-stream.c
    src/apulse-threaded-mainloop.c
    src/apulse-volume.c
    src/ringbuffer.c
    src/util.c
)

add_library(pulse-simple SHARED
    src/apulse-simple.c
)

add_library(pulse-mainloop-glib SHARED
    src/apulse-mainloop-glib.c
)

set_target_properties(pulse PROPERTIES VERSION 0)
set_target_properties(pulse-mainloop-glib PROPERTIES VERSION 0)
set_target_properties(pulse-simple PROPERTIES VERSION 0)

set(SYMBOLMAP "-Wl,-version-script=\"${CMAKE_SOURCE_DIR}/src/symbolmap\"")

target_link_libraries(pulse ${SYMBOLMAP} trace-helper ${REQ_LIBRARIES})
target_link_libraries(pulse-mainloop-glib ${SYMBOLMAP} trace-helper ${REQ_LIBRARIES})
target_link_libraries(pulse-simple ${SYMBOLMAP} trace-helper ${REQ_LIBRARIES})

add_subdirectory(tests)

set(APULSEPATH "${CMAKE_INSTALL_PREFIX}/lib/apulse" CACHE PATH "library installation directory")
set(APULSE_SEARCH_PATHS "${APULSEPATH}" CACHE PATH "directory list for LD_LIBRARY_PATH")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/apulse.template"
               "${CMAKE_CURRENT_BINARY_DIR}/apulse" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/apulse" DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(TARGETS pulse-simple pulse pulse-mainloop-glib DESTINATION "${APULSEPATH}")
install(FILES "${CMAKE_SOURCE_DIR}/man/apulse.1" DESTINATION share/man/man1)
