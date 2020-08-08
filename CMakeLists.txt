cmake_minimum_required(VERSION 2.8.9)
project (craw-tool)
include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/libs/)
file(GLOB SOURCE "src/*.cpp")
add_executable(craw-tool ${SOURCE})

#####Include necessary for CMake#####
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(FindPkgConfig)
include(GenerateExportHeader)

######Inlcude pthread#####
find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(my_app PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(craw-tool "${CMAKE_THREAD_LIBS_INIT}")
endif()

######Include curl#####
pkg_check_modules(CURL libcurl REQUIRED)
include_directories(
  SYSTEM ${CURL_INCLUDE_DIRS}
)
target_link_libraries(craw-tool
  ${CURL_LIBRARIES}
)

######Include spdlog#####
pkg_check_modules(SPDLOG spdlog REQUIRED)
include_directories(
  SYSTEM ${SPDLOG_INCLUDE_DIRS}
)
target_link_libraries(craw-tool
  ${SPDLOG_LIBRARIES}
)



