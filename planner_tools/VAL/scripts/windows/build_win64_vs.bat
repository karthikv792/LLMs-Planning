REM Build

echo "Build win64 Visual Studio"

REM Ensure sh is not in the path for MinGW Makefiles generator to work.
setlocal enabledelayedexpansion
where sh.exe > path.txt
set /p SH_EXE_PATH=<path.txt
del /s /q path.txt
if "%SH_EXE_PATH%" == "" ( goto BUILD )
for %%i in ("%SH_EXE_PATH%") do set SH_EXE_DIR=%%~di%%~pi
if %SH_EXE_DIR:~-1%==\ set SH_EXE_DIR=%SH_EXE_DIR:~0,-1%
set PATH=!PATH:%SH_EXE_DIR%;=!

where sh.exe > path.txt
set /p SH_EXE_PATH=<path.txt
del /s /q path.txt
if "%SH_EXE_PATH%" == "" ( goto BUILD )
for %%i in ("%SH_EXE_PATH%") do set SH_EXE_DIR=%%~di%%~pi
if %SH_EXE_DIR:~-1%==\ set SH_EXE_DIR=%SH_EXE_DIR:~0,-1%
set PATH=!PATH:%SH_EXE_DIR%;=!

:BUILD

REM set BUILD_GENERATOR="Visual Studio 15 2017"
set BUILD_GENERATOR="Visual Studio 15 2017 Win64"
set BUILD_CONFIGURATION=Debug
REM set BUILD_CONFIGURATION=Release
set BUILD_TARGET=ALL_BUILD

md build
cd build

md win64
cd win64

md vs
cd vs

md %BUILD_CONFIGURATION%
cd %BUILD_CONFIGURATION%
cmake -G %BUILD_GENERATOR% -DCMAKE_BUILD_TYPE=%BUILD_CONFIGURATION% -DCMAKE_INSTALL_PREFIX=%cd%/install ..\..\..\..
if %errorlevel% neq 0 goto ERROR
cmake --build . --config %BUILD_CONFIGURATION% --target %BUILD_TARGET%
if %errorlevel% neq 0 goto ERROR
cmake --build . --target install
if %errorlevel% neq 0 goto ERROR
cpack ../../..
if %errorlevel% neq 0 goto ERROR
cd ..

cd ..

cd ..

cd ..

goto END

:ERROR
echo Build failed
exit /b 1

:END
exit /b 0
