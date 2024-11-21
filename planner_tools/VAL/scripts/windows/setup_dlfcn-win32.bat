mkdir externals
cd externals

rmdir /s /q dlfcn-win32

mkdir tmp
cd tmp

REM dlfcn-win32
git clone https://github.com/dlfcn-win32/dlfcn-win32
cd dlfcn-win32

mkdir build
cd build

mkdir win64
cd win64
cmake -G "MinGW Makefiles" -DCMAKE_C_FLAGS=-m64 -DCMAKE_CXX_FLAGS=-m64 ../..
mingw32-make all
cd ..

cd ..

cd ..

cd ..

mkdir dlfcn-win32
cd dlfcn-win32
mkdir include
copy ..\tmp\dlfcn-win32\dlfcn.h include
mkdir lib\win64
copy ..\tmp\dlfcn-win32\build\win64\libdl.dll lib\win64

cd ..

rmdir /s /q tmp

cd ..