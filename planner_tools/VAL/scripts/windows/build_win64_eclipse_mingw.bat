REM Build
REM https://www.mantidproject.org/Setting_up_Eclipse_projects_with_CMake

echo "Build win64 Eclipse MinGW"

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

set BUILD_GENERATOR="Eclipse CDT4 - MinGW Makefiles"
set BUILD_CONFIGURATION=Debug
REM set BUILD_CONFIGURATION=Release
set BUILD_TARGET=all
set BUILD_THREAD=4

md build
cd build

md win64
cd win64

md eclipse_mingw
cd eclipse_mingw

md %BUILD_CONFIGURATION%
cd %BUILD_CONFIGURATION%
cmake -G %BUILD_GENERATOR% -DCMAKE_BUILD_TYPE=%BUILD_CONFIGURATION% -DCMAKE_INSTALL_PREFIX=%cd%/install ..\..\..\..
if %errorlevel% neq 0 goto ERROR
cmake --build . --config %BUILD_CONFIGURATION% --target %BUILD_TARGET% -- -j %BUILD_THREAD%
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
