import os
import platform

def addArm64Triplet(build_triplet):
    triplet = """set(VCPKG_TARGET_ARCHITECTURE arm64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_CMAKE_SYSTEM_NAME Linux)
set(VCPKG_LINUX_ARCHITECTURES arm64) # unused in build_from_scratch

if(PORT MATCHES "curl" OR
   PORT MATCHES "ffmpeg" OR
   PORT MATCHES "c-ares")
    # build this library as dynamic (usually because it is LGPL licensed)
    set(VCPKG_LIBRARY_LINKAGE dynamic)
else()
    # build this library statically (much simpler installation, debugging, etc)
    set(VCPKG_LIBRARY_LINKAGE static)
endif()

set(VCPKG_C_FLAGS "${VCPKG_C_FLAGS}")
set(VCPKG_CXX_FLAGS "${VCPKG_CXX_FLAGS}")
set(VCPKG_LINKER_FLAGS "${VCPKG_LINKER_FLAGS}")
"""
    triplet_dir = os.path.join(sdk_cmake_builder_path, "vcpkg_extra_triplets")
    arm64_triplet_path = os.path.join(triplet_dir, f"{build_triplet}.cmake")
    with open(arm64_triplet_path, "w") as f:
        f.write(triplet)
        f.flush()

main_dir = os.getcwd()
dependencies = os.path.join(main_dir, "dependencies")
sdk_path = os.path.join(dependencies, "sdk")
sdk_headers_path = os.path.join(sdk_path, "include")
sdk_cmake_builder_path = os.path.join(sdk_path, "contrib", "cmake")
project_libs_path = os.path.join(dependencies, "libs")
project_mega_sdk_include_path = os.path.join(dependencies, "include")
project_artifacts_path = os.path.join(main_dir, "cmake-build-debug")
if platform.machine() == "aarch64":
    build_triplet = "arm64-linux-mega"
    addArm64Triplet(build_triplet) #megasdk atm does not have an arm64-linux triplet in repo.
else:
    build_triplet = "x64-linux-mega"
sdk_build_artifacts_path = os.path.join(sdk_path, f"build-{build_triplet}-Release")

dep_static_libssl = "libssl.a"
dep_static_libcrypto = "libcrypto.a"
dep_static_libcryptopp = "libcryptopp.a"
dep_static_libsodium = "libsodium.a"
dep_static_libsqlite3 = "libsqlite3.a"
dep_static_libmega = "libMega.a"


def runCmd(cmd):
    print(cmd)
    os.system(cmd)

def changeDir(directory):
    print(f"changing current dir to: {directory}")
    os.chdir(directory)

def createDirs():
    os.makedirs(project_libs_path, exist_ok=True)
    os.makedirs(project_artifacts_path, exist_ok=True)

def isPortBlacklisted(line):
    blacklisted_libs = ["ffmpeg", "freeimage","libpng","libjpeg-turbo","tiff","openjpeg","libwebp","libraw","jxrlib","openexr","jasper","liblzma","python3","libffi","opengl","freeglut", "pdfium","pdfium-freetype","icu","icu","lcms","libjpeg-turbo","openjpeg","bzip2","libpng", "pcre", "libuv"]
    for lib in blacklisted_libs:
        if line.startswith(lib):
            return True
    return False 

def patchPreferredPorts():
    preferred_ports_file_path = os.path.join(sdk_cmake_builder_path, "preferred-ports-sdk.txt")
    with open(preferred_ports_file_path, "r") as f:
        file_data =  f.readlines()
    new_file_data = []
    for line in file_data:
        if isPortBlacklisted(line):
            line = f"#{line}"
        new_file_data.append(line)
    with open(preferred_ports_file_path, "w") as f:
        for line in new_file_data:
            f.write(line)

def buildMegaSDK():
    changeDir(sdk_cmake_builder_path)
    runCmd(f'cmake -DTRIPLET={build_triplet} -DEXTRA_ARGS="-DUSE_PDFIUM=0;-DUSE_FREEIMAGE=0;-DENABLE_CHAT=0;-DENABLE_SYNC=0;-DHAVE_FFMPEG=0;-DUSE_MEDIAINFO=0" -P build_from_scratch.cmake')

def setupMegaSDKLibs():
    dep_libs_base_path = os.path.join(dependencies, "3rdparty_sdk", "vcpkg", "installed", build_triplet, "lib")
    runCmd(f'cp "{os.path.join(dep_libs_base_path, dep_static_libssl)}" "{os.path.join(project_libs_path, dep_static_libssl)}"')
    runCmd(f'cp "{os.path.join(dep_libs_base_path, dep_static_libcrypto)}" "{os.path.join(project_libs_path, dep_static_libcrypto)}"')
    runCmd(f'cp "{os.path.join(dep_libs_base_path, dep_static_libcryptopp)}" "{os.path.join(project_libs_path, dep_static_libcryptopp)}"')
    runCmd(f'cp "{os.path.join(dep_libs_base_path, dep_static_libsodium)}" "{os.path.join(project_libs_path, dep_static_libsodium)}"')
    runCmd(f'cp "{os.path.join(dep_libs_base_path, dep_static_libsqlite3)}" "{os.path.join(project_libs_path, dep_static_libsqlite3)}"')

    runCmd(f'cp "{os.path.join(sdk_build_artifacts_path, dep_static_libmega)}" "{os.path.join(project_libs_path, dep_static_libmega)}"')

def setupIncludeDir():
    runCmd(f' cp -r "{sdk_headers_path}" "{project_mega_sdk_include_path}"')

def buildProject():
    changeDir(main_dir)
    runCmd(f'cmake --build {project_artifacts_path} --target megasdkrestpp')

def configureCmake():
    changeDir(main_dir)
    runCmd(f'cmake -S . -B {project_artifacts_path}')

def patchMegaSDKTestsCode():
    test_file = os.path.join(sdk_path, "tests", "integration", "SdkTest_test.cpp")
    if 'megaApi[0]->moveOrRemoveDeconfiguredBackupNodes(backupRoots->get(j)->getHandle(), INVALID_HANDLE, &rt);' in open(test_file, 'r').read():
        runCmd(f'sed -i "817d" "{test_file}"')

if __name__ == "__main__":
    createDirs()
    patchPreferredPorts()
    patchMegaSDKTestsCode()
    buildMegaSDK()
    setupIncludeDir()
    setupMegaSDKLibs()
    configureCmake()
    buildProject()
