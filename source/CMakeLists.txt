cmake_minimum_required(VERSION 3.10)

project(JVM)

set(CMAKE_CXX_STANDARD 20)

file(GLOB HEADERS CONFIGURE_DEPENDS *.h)
file(GLOB SOURCES CONFIGURE_DEPENDS *.cpp)
file(GLOB CONSTANT_POOL CONFIGURE_DEPENDS constant_pool/*)
file(GLOB PARSER CONFIGURE_DEPENDS parser/*)

source_group(Headers FILES ${HEADERS})
source_group(Sources FILES ${SOURCES})
source_group(constant_pool FILES ${CONSTANT_POOL})
source_group(parser FILES ${PARSER})

add_executable(JVM ${HEADERS} ${SOURCES} ${CONSTANT_POOL} ${PARSER})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT JVM)