cmake_minimum_required(VERSION 2.7)

# Name of the project
project(MinimalOpenGLExample)

# Use glob to get the list of all source files.
file(GLOB_RECURSE SOURCES "MinimalOpenGLExample/*.cpp" "MinimalOpenGLExample/glad/src/*.c")

# We don't really need to include header and resource files to build, but it's
# nice to have them show up in IDEs.
file(GLOB_RECURSE HEADERS "MinimalOpenGLExample/*.h" "MinimalOpenGLExample/glad/*/*.h")

include_directories("MinimalOpenGLExample/glad/include")

# Set the executable.
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

# Add GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} ${GLFW_LIBRARIES})
# target_link_libraries(${CMAKE_PROJECT_NAME} ${GLFW_STATIC_LIBRARIES})


# OS specific options and libraries
if(WIN32)
  # c++0x is enabled by default.
  # -Wall produces way too many warnings.
  # -pedantic is not supported.
  target_link_libraries(${CMAKE_PROJECT_NAME} opengl32.lib)
else()
  # Enable all pedantic warnings.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -pedantic")

  if(APPLE)
    # Add required frameworks for GLFW.
    target_link_libraries(${CMAKE_PROJECT_NAME} "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
  else()
    #Link the Linux OpenGL library
    target_link_libraries(${CMAKE_PROJECT_NAME} "GL" "dl")
  endif()
endif()
