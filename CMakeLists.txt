cmake_minimum_required(VERSION 3.24)
project(project)

set(CMAKE_CXX_STANDARD 14)

include_directories(dependencies/include)
include_directories(dependencies/served/src)
include_directories(dependencies/SimpleJSON)
add_subdirectory(dependencies/served)
set(CUSTOM_LIB_DIR, ${CMAKE_SOURCE_DIR}/dependencies/libs)
include_directories(.)
#FIND_LIBRARY(mega NAMES "libMega"
#        PATHS ${PROJECT_SOURCE_DIR}/dependencies/libs NO_DEFAULT_PATH)
add_executable(project main.cpp MegaAppRequestListener.cpp MegaAppTransferListener.cpp MegaDownload.cpp MegaDownloader.cpp Utils.cpp log.cpp)
#find_library(cryptopp cryptopp)
#find_library(sodium sodium)

target_link_libraries(project ${CUSTOM_LIB_DIR}/libMega.a ${CUSTOM_LIB_DIR}/libcryptopp.a pthread sqlite3 curl ssl crypto cares sodium rt dl served)