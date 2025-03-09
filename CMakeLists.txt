cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)
# cmake_policy(SET CMP0135 NEW)
include(ExternalProject)

project (ltc-tools)

# define NPI_VERSION
add_definitions(-DNAPI_VERSION=7)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(${CMAKE_JS_INC})

set(HEADER_FILES
  "src/ltcwrapper.h"
)
set(SOURCE_FILES
  "src/ltcwrapper.cc"
)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/lib")

# if(UNIX AND NOT APPLE)
#   # build shared on linux, to avoid conflicts with lib???? built into electron
#   set(ENABLE_SHARED 1)
#   set(ENABLE_STATIC 0)

# else()
#   # build static elsewhere
#   set(ENABLE_SHARED 0)
#   set(ENABLE_STATIC 1)
# endif()

# Download and build libltc
#ExternalProject_Add(libltc
#  URL ${libltc_url}
#  URL_MD5 ${libltc_md5}
#  CMAKE_ARGS -DENABLE_SHARED=${ENABLE_SHARED} -DENABLE_STATIC=${ENABLE_STATIC} -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
#  INSTALL_COMMAND ""
#)

# find_program(MAKE_EXECUTABLE NAMES make gmake REQUIRED)

# set_property(DIRECTORY PROPERTY EP_UPDATE_DISCONNECTED true)

ExternalProject_Add(libltc
    GIT_REPOSITORY      https://github.com/x42/libltc.git
    GIT_TAG             master
    SOURCE_DIR          "${CMAKE_BINARY_DIR}/libltc"
    BUILD_IN_SOURCE     TRUE
    CONFIGURE_COMMAND   "./autogen.sh" "--prefix=${CMAKE_INSTALL_PREFIX}" "--enable-static" "--disable-shared"
    BUILD_COMMAND       "make"
)


ExternalProject_Get_Property(libltc SOURCE_DIR)
set(LTC_INCLUDE_DIR "${SOURCE_DIR}/src")
ExternalProject_Get_Property(libltc BINARY_DIR)
set(LTC_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(LTC_LIB libltc)

message("----------------LTC--------------- ${LTC_LIB}")

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_include_directories(${PROJECT_NAME} PRIVATE ${LTC_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PRIVATE ${LTC_LIB_DIR})
# target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${LTC_LIB})
add_dependencies(${PROJECT_NAME} libltc)

# if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
#   # Generate node.lib
#   execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
# endif()

if(UNIX AND NOT APPLE)
  # copy built lib into build dir
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${LTC_LIB_DIR}/libltc.a ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}
  )
endif()
