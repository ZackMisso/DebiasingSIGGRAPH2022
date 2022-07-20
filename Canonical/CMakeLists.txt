cmake_minimum_required (VERSION 2.8.3)
project(conver)

if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
endif()

include(CheckCXXCompilerFlag)
include(CheckCXXSourceRuns)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # Quench annoying deprecation warnings when compiling GLFW on OSX
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|Intel)$")
  CHECK_CXX_COMPILER_FLAG("-std=c++14" HAS_CPP14_FLAG)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP11_FLAG)

  if (HAS_CPP14_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -O3")
  elseif (HAS_CPP11_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
  else()
    message(FATAL_ERROR "Unsupported compiler -- pybind11 requires C++11 support!")
  endif()
endif()

IF(APPLE)
    # Fix linking on 10.14+. See https://stackoverflow.com/questions/54068035
    LINK_DIRECTORIES(/usr/local/lib)
ENDIF()

if (APPLE)
  find_library(cocoa_library Cocoa)
  find_library(opengl_library OpenGL)
  find_library(corevideo_library CoreVideo)
  find_library(iokit_library IOKit)
  list(APPEND NANOGUI_EXTRA_LIBS ${cocoa_library} ${opengl_library} ${corevideo_library} ${iokit_library})
  list(APPEND LIBNANOGUI_EXTRA_SOURCE src/darwin.mm)
 endif()

set(PCG32_INCLUDE_DIR
   ${CMAKE_CURRENT_SOURCE_DIR}/ext/pcg32)

set(IMGLIB_INCLUDE_DIR
   ${CMAKE_CURRENT_SOURCE_DIR}/ext/ImageEditLib/include)

set(IMGLIB_NEEDED_DIR
   ${CMAKE_CURRENT_SOURCE_DIR}/ext/ImageEditLib/ext)

set(JSON_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/rapidjson/include)

set(TINYEXR_INCLUDE_DIR
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/ImageEditLib/ext/tinyexr)

set(OPENVDB_INCLUDE_DIR
   /usr/local/Cellar/openvdb/7.0.0_2/include/)

set(OPENEXR_INCLUDE_DIR
   /usr/local/include/)

# find_package(PythonLibs REQUIRED)
# find_package(embree 3.0 REQUIRED)

include_directories(${PYTHON_INCLUDE_DIRS})

include_directories(
    # Include directory
    ${CMAKE_CURRENT_SOURCE_DIR}/include/

    # Random number generator
    ${PCG32_INCLUDE_DIR}/

    # Programatical image editing library
    ${IMGLIB_INCLUDE_DIR}/

    ${IMGLIB_NEEDED_DIR}/

    ${JSON_INCLUDE_DIR}

    ${TINYEXR_INCLUDE_DIR}

    ${OPENVDB_INCLUDE_DIR}

    ${OPENEXR_INCLUDE_DIR}

    #ext/
)

if(APPLE)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -flto -Wno-unused-result -fno-strict-aliasing")
else()
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -flto -Wno-unused-result -fno-strict-aliasing")
endif()


FILE ( GLOB CONVER_SOURCE
    include/conver/*.h
    src/*.cpp
    src/techniques/*.cpp
    src/problems/*.cpp
    src/experiments/*.cpp
    src/experiments/exp/*.cpp
    src/experiments/recip/*.cpp
    src/experiments/photon/*.cpp
    src/experiments/davis/*.cpp
    src/misc/*.cpp
  )

add_executable(conver
    # source files
    ${CONVER_SOURCE}

    # imedit implementation files
    ext/ImageEditLib/src/image.cpp
)

#OPENEXR_INCLUDE_DIRS
set(CompilerFlags
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO CMAKE_C_FLAGS
      CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL
      CMAKE_C_FLAGS_RELWITHDEBINFO COMPILE_DEFINITIONS U_CMAKE_BUILD_TYPE
      CMAKE_MACOSX_RPATH
      PCG32_INCLUDE_DIR
      IMGLIB_INCLUDE_DIR
      TINYEXR_INCLUDE_DIR
)

# target_link_libraries(conver tbb Half)
target_link_libraries(conver)
# target_link_libraries(feign tbb openvdb embree3 Half)
#target_link_libraries(feign tbb openvdb embree3 Half ${PYTHON_LIBRARIES})
