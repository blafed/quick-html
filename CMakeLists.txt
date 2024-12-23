project(qhtml CXX)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)


set(TEST ON)

set(QJS_DIR ${CMAKE_SOURCE_DIR}/lib/quickjs)
set(QJS_OBJ ${QJS_DIR}/.obj)
set(QJS_LIB ${QJS_OBJ}/libquickjs.a)

set(LH ${CMAKE_SOURCE_DIR}/lib/litehtml)
set(CITY ${CMAKE_SOURCE_DIR}/lib/canvas_ity.hpp)


set(EXE src/dom.cpp)

if(!TEST)
    set(SRC src)
set(EXE ${EXE} ${SRC}/main.cpp)

else()
#test configuration
set(SRC test)
set(CONT ${SRC}/container)

set(EXE 
${EXE}
${CONT}/lodepng.cpp
${CONT}/test_container.cpp
${CONT}/Font.cpp
${CONT}/Bitmap.cpp
# ${CONT}/container.cpp
# ${CONT}/Bitmap.cpp
${CONT}/Font.cpp
# ${CONT}/test_container.cpp
${SRC}/test.cpp)

endif()


add_executable(${PROJECT_NAME} ${EXE})


target_include_directories(${PROJECT_NAME} PRIVATE lib)

#quickjs
add_custom_command(
    OUTPUT ${QJS_LIB}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${QJS_OBJ}
    COMMAND ${CMAKE_COMMAND} -E chdir ${QJS_DIR} ${CMAKE_MAKE_PROGRAM} libquickjs.a
    COMMAND ${CMAKE_COMMAND} -E copy ${QJS_DIR}/libquickjs.a ${QJS_LIB}
    COMMENT "Building QuickJS static library"
    DEPENDS ${QJS_DIR}/Makefile
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

add_custom_target(quickjs ALL DEPENDS ${QJS_LIB})
add_dependencies(${PROJECT_NAME} quickjs)
target_include_directories(${PROJECT_NAME} PRIVATE ${QJS_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${QJS_LIB})

#litehtml
add_subdirectory(${LH})
add_dependencies(${PROJECT_NAME} litehtml)
target_link_libraries(${PROJECT_NAME} PRIVATE litehtml)
target_include_directories(${PROJECT_NAME} PRIVATE ${LH}/include ${LH}/include/litehtml)

#canvas_ity
add_library(canvas_ity STATIC ${CITY})
# target_sources(canvas_ity PRIVATE ${CITY})
target_compile_features(canvas_ity PRIVATE cxx_std_17)
set_target_properties(canvas_ity PROPERTIES LINKER_LANGUAGE CXX)
# target_compile_definitions(canvas_ity PUBLIC CANVAS_ITY_IMPLEMENTATION=1)
target_link_libraries(${PROJECT_NAME} PRIVATE canvas_ity)


if(TEST)
target_include_directories(${PROJECT_NAME} PRIVATE ${CONT} test)
# Include X11 headers
find_package(X11 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${X11_INCLUDE_DIR})

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/test/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)
endif()