project(apulse)
cmake_minimum_required (VERSION 2.8)
add_definitions(-std=gnu99 -Wall -fPIC -fvisibility=hidden)
add_definitions(-Werror=implicit-function-declaration)

find_package(PkgConfig REQUIRED)

pkg_check_modules(REQ glib-2.0 alsa REQUIRED)

# enable tracing for Debug builds
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DWITH_TRACE")

# link with pthread
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

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
    src/apulse-context.c
    src/apulse-mainloop.c
    src/apulse-misc.c
    src/apulse-operation.c
    src/apulse-proplist.c
    src/apulse-signal.c
    src/apulse-stream.c
    src/apulse-threaded-mainloop.c
    src/util.c
    src/ringbuffer.c
    src/notimplemented.c
)

add_library(pulse-simple SHARED
    src/apulse-simple.c
)
add_library(pulsecommon-5.0 SHARED
    src/apulse-common.c
)

set_target_properties(pulse PROPERTIES VERSION 0)
set_target_properties(pulse-simple PROPERTIES VERSION 0)

set(SYMBOLMAP "-Wl,-version-script=\"${CMAKE_SOURCE_DIR}/src/symbolmap\"")

target_link_libraries(pulse ${SYMBOLMAP} trace-helper ${REQ_LIBRARIES})
target_link_libraries(pulse-simple ${SYMBOLMAP} trace-helper ${REQ_LIBRARIES})
target_link_libraries(pulsecommon-5.0 ${SYMBOLMAP} trace-helper ${REQ_LIBRARIES})

add_subdirectory(tests)

set(APULSEPATH "${CMAKE_INSTALL_PREFIX}/lib/apulse" CACHE PATH "library installation directory")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/apulse.template"
               "${CMAKE_CURRENT_BINARY_DIR}/apulse" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/apulse" DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(TARGETS pulse-simple pulsecommon-5.0 pulse DESTINATION "${APULSEPATH}")
