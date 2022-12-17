import os
main_dir = os.getcwd()
dependencies = os.path.join(main_dir, "dependencies")
sdk_path = os.path.join(dependencies, "sdk")
sdk_headers_path = os.path.join(sdk_path, "include")
sdk_build_artifacts_path = os.path.join(sdk_path, "build-arm64-linux-mega-Release")
sdk_cmake_builder_path = os.path.join(sdk_path, "contrib", "cmake")
project_libs_path = os.path.join(dependencies, "libs")
project_mega_sdk_include_path = os.path.join(dependencies, "include")
project_artifacts_path = os.path.join(main_dir, "cmake-build-debug")
build_triplet = "arm64-linux-mega"

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
    runCmd(f'cmake --build {project_artifacts_path} --target megasdkrestpp -j 1')

def configureCmake():
    changeDir(main_dir)
    runCmd(f'cmake -S . -B {project_artifacts_path}')

def patchMegaSDKTestsCode():
    test_file = os.path.join(sdk_path, "tests", "integration", "SdkTest_test.cpp")
    if 'megaApi[0]->moveOrRemoveDeconfiguredBackupNodes(backupRoots->get(j)->getHandle(), INVALID_HANDLE, &rt);' in open(test_file, 'r').read():
        runCmd(f'sed -i "817d" "{test_file}"')

if __name__ == "__main__":
    createDirs()
    patchMegaSDKTestsCode()
    buildMegaSDK()
    setupIncludeDir()
    setupMegaSDKLibs()
    configureCmake()
    buildProject()
