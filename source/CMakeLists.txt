file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS *.hpp *.cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})

add_executable(JVM ${SRCS})