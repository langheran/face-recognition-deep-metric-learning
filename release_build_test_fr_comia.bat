:: Change also the CMakeLists.txt file to point to the CUDA dlib release version.

cd ./build/Release
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
cmake.exe --build . --config Release --target all -- -j 6 && exit || pause && exit
cd ../..