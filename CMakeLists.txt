cmake_minimum_required(VERSION 3.24)
project(project)

set(CMAKE_CXX_STANDARD 14)

include_directories(dependencies/include)
set(LIB_PATH "/home/zerocool/mnt/megaserver/3rdparty_sdk/vcpkg/installed/x64-linux-mega/lib")

add_subdirectory(dependencies/served)

include_directories(.)
FIND_LIBRARY(mega NAMES "libMega"
        PATHS ${PROJECT_SOURCE_DIR}/dependencies/libs NO_DEFAULT_PATH)
add_executable(project main.cpp MegaAppRequestListener.cpp MegaAppTransferListener.cpp MegaDownload.cpp MegaDownloader.cpp Utils.cpp log.cpp HttpServer.cpp HttpServer.h)
find_library(cryptopp cryptopp)
find_library(sodium sodium)

target_link_libraries(project ${CMAKE_SOURCE_DIR}/dependencies/libs/libMega.a ${LIB_PATH}/libcryptopp.a pthread sqlite3 curl ssl crypto cares sodium rt dl)