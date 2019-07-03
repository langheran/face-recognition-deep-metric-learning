mkdir build
cd build
rd /s /q Release
mkdir Release
cd Release
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
cmake.exe -G "Ninja" -DCMAKE_BUILD_TYPE=Release ../..
cd ../..
pause
exit