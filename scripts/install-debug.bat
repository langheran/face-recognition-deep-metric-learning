cd C:\dlib-19.8

rd /s /q build
mkdir build
cd build

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64

cmake -G "Ninja" ^
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON ^
-D "CMAKE_C_COMPILER=C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin/amd64/cl.exe" ^
-D "CMAKE_CXX_COMPILER=C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin/amd64/cl.exe" ^
-DCMAKE_CXX_FLAGS="/FS /EHsc /FC /Zi" ^
-DCMAKE_SHARED_LINKER_FLAGS="/DEBUG /OPT:REF /OPT:ICF" ^
-DJPEG_INCLUDE_DIR=..\dlib\external\libjpeg ^
-DJPEG_LIBRARY=..\dlib\external\libjpeg ^
-DPNG_PNG_INCLUDE_DIR=..\dlib\external\libpng ^
-DPNG_LIBRARY_RELEASE=..\dlib\external\libpng ^
-DZLIB_INCLUDE_DIR=..\dlib\external\zlib ^
-DZLIB_LIBRARY_RELEASE=..\dlib\external\zlib ^
-DCMAKE_BUILD_TYPE=Debug ^
-DCMAKE_INSTALL_PREFIX=install ..

cmake --build . --config Debug --target all

ninja install

cd ..
rd /s /q debug_build
echo d | xcopy build "C:\dlib-19.8\debug_build" /s /e /y
echo f | xcopy "C:\dlib-19.8\debug_build\dlib\CMakeFiles\dlib.dir\dlib.pdb" "C:\dlib-19.8\debug_build\install\lib" /s /e /y