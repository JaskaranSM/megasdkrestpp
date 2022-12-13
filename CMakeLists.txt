cmake_minimum_required(VERSION 3.24)
project(megasdkrestpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(dependencies/include)
include_directories(dependencies/served/src)
include_directories(dependencies/SimpleJSON)
include_directories(dependencies/loguru)
add_subdirectory(dependencies/served)
include_directories(.)
#FIND_LIBRARY(mega NAMES "libMega"
#        PATHS ${PROJECT_SOURCE_DIR}/dependencies/libs NO_DEFAULT_PATH)
add_executable(megasdkrestpp main.cpp MegaAppRequestListener.cpp MegaAppTransferListener.cpp MegaDownload.cpp MegaDownloader.cpp Utils.cpp log.cpp)
#find_library(cryptopp cryptopp)
#find_library(sodium sodium)

target_link_libraries(megasdkrestpp ${CMAKE_SOURCE_DIR}/dependencies/libs/libMega.a ${CMAKE_SOURCE_DIR}/dependencies/libs/libcryptopp.a pthread ${CMAKE_SOURCE_DIR}/dependencies/libs/libsqlite3.a curl ${CMAKE_SOURCE_DIR}/dependencies/libs/libssl.a ${CMAKE_SOURCE_DIR}/dependencies/libs/libcrypto.a cares ${CMAKE_SOURCE_DIR}/dependencies/libs/libsodium.a rt dl served)