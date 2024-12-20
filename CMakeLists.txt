option(TEST "" OFF)

# Set project name and language
cmake_minimum_required(VERSION 3.10)
project(MyProject CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# if(TEST)
#     set(PROG test)
# else()
#     set(PROG main)
# endif()

# if(TEST)
#     set(SRC test)
# else()
#     set(SRC main)
# endif()

# Define directories for QuickJS and LiteHTML
set(QJS_DIR ${CMAKE_SOURCE_DIR}/lib/quickjs)
set(OBJ_DIR ${QJS_DIR}/.obj)
set(QJS_LIB ${OBJ_DIR}/libquickjs.a)

set(LH_DIR ${CMAKE_SOURCE_DIR}/lib/litehtml)
set(LH_BUILD_DIR ${LH_DIR}/build)
set(LH_LIB ${LH_BUILD_DIR}/liblitehtml.a)

# Define directory for Canvas_ity
set(CANVAS_ITY_DIR ${CMAKE_SOURCE_DIR}/lib)
set(CITY_PATH ${CANVAS_ITY_DIR}/canvas_ity.hpp)
set(CONTE ${CMAKE_SOURCE_DIR}/lib/litehtml/containers/test)

# Step 1: Build QuickJS library
add_custom_command(
    OUTPUT ${QJS_LIB}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OBJ_DIR}
    COMMAND ${CMAKE_COMMAND} -E chdir ${QJS_DIR} ${CMAKE_MAKE_PROGRAM} libquickjs.a
    COMMAND ${CMAKE_COMMAND} -E copy ${QJS_DIR}/libquickjs.a ${QJS_LIB}
    COMMENT "Building QuickJS static library"
    DEPENDS ${QJS_DIR}/Makefile
)

# Create a dummy target for QuickJS to resolve dependencies
add_custom_target(quickjs ALL
    DEPENDS ${QJS_LIB}
)

# Step 2: Build LiteHTML library
add_custom_command(
    OUTPUT ${LH_LIB}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${LH_BUILD_DIR}
    COMMAND ${CMAKE_COMMAND} -E chdir ${LH_BUILD_DIR} ${CMAKE_COMMAND} .. && ${CMAKE_MAKE_PROGRAM}
    COMMENT "Building LiteHTML static library"
    DEPENDS ${LH_DIR}/CMakeLists.txt
)

# Create a dummy target for LiteHTML to resolve dependencies
add_custom_target(litehtml ALL
    DEPENDS ${LH_LIB}
)


# Step 3: Define main executable
if(TEST)
add_executable(main ${CONTE}/lodepng.cpp ${CITY_PATH} ${CMAKE_SOURCE_DIR}/test/test.cpp)
target_compile_definitions(main PUBLIC CANVAS_ITY_IMPLEMENTATION=1)
else()
add_executable(main ${CITY_PATH} ${CMAKE_SOURCE_DIR}/src/main.cpp)
endif()

# Link the main executable with the static libraries
add_dependencies(main quickjs litehtml)

# Include directories for QuickJS, LiteHTML, and Canvas_ity
target_include_directories(main PRIVATE 
    ${QJS_DIR} 
    ${LH_DIR}/include 
    ${CANVAS_ITY_DIR}
)

if(TEST)
target_include_directories(main PRIVATE ${CONTE})
endif()

# Link the required libraries (QuickJS, LiteHTML, and X11) to the main executable
find_package(X11 REQUIRED)
target_link_libraries(main PRIVATE ${QJS_LIB} ${LH_LIB} ${X11_LIBRARIES})

# Include X11 headers
include_directories(${X11_INCLUDE_DIR})

if(TEST)
add_custom_command(
  TARGET main POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/test/res $<TARGET_FILE_DIR:main>/res
)
endif()
