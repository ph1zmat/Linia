rmdir ..\wxWidgets\generated /s /q
cmake.exe ..\wxWidgets\CMakeLists.txt -B..\wxWidgets\generated -DCMAKE_SYSTEM_VERSION=10.0.18362.0 -G "Visual Studio 17 2022" -A Win32
