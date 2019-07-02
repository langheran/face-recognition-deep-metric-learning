mkdir build
cd build
rd /s /q Debug
mkdir Debug
cd Debug
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
cmake.exe -G "Ninja" -DCMAKE_BUILD_TYPE=Debug ../..
cd ../..
pause
exit