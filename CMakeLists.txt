project(qhtml CXX)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)


set(TEST ON)

set(QJS_DIR ${CMAKE_SOURCE_DIR}/lib/quickjs)
set(QJS_OBJ ${QJS_DIR}/.obj)
set(QJS_LIB ${QJS_OBJ}/libquickjs.a)

set(LH ${CMAKE_SOURCE_DIR}/lib/litehtml)


if(!TEST)
    set(SRC src)
set(EXE ${SRC}/main.cpp)

else()
#test configuration
    set(SRC test)
set(EXE ${SRC}/test.cpp)

endif()


add_executable(${PROJECT_NAME} ${EXE})


add_custom_command(
    OUTPUT ${QJS_LIB}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${QJS_OBJ}
    COMMAND ${CMAKE_COMMAND} -E chdir ${QJS_DIR} ${CMAKE_MAKE_PROGRAM} libquickjs.a
    COMMAND ${CMAKE_COMMAND} -E copy ${QJS_DIR}/libquickjs.a ${QJS_LIB}
    COMMENT "Building QuickJS static library"
    DEPENDS ${QJS_DIR}/Makefile
)

add_custom_target(quickjs ALL DEPENDS ${QJS_LIB})
add_dependencies(${PROJECT_NAME} quickjs)
target_include_directories(${PROJECT_NAME} PRIVATE ${QJS_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${QJS_LIB})


add_subdirectory(${LH})
add_dependencies(${PROJECT_NAME} litehtml)
target_link_libraries(${PROJECT_NAME} PRIVATE litehtml)
target_include_directories(${PROJECT_NAME} PRIVATE ${LH}/include ${LH}/include/litehtml)