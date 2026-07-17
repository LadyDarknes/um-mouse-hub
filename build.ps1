$p = Resolve-Path "$env:LOCALAPPDATA\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.MCF.UCRT_*\mingw64"
cmake -B build -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM="$p\bin\mingw32-make.exe" -DCMAKE_C_COMPILER="$p\bin\gcc.exe"
cmake --build build
