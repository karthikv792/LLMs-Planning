# VAL

[![Build Status](https://dev.azure.com/schlumberger/ai-planning-validation/_apis/build/status/ai-planning-tool-val-CI?branchName=master)](https://dev.azure.com/schlumberger/ai-planning-validation/_build/latest?definitionId=2&branchName=master)

This repository hosts tools for AI Planning plans and planning models.

If all you need is binaries, click the _Azure Pipeline_ link above, open the latest green build navigate to the _Summary_ tab and then download the artifact for your operating system. Binaries are available for Linux, Windows and MacOS.

Please report problems and feature requests

## Authors

* Maria Fox and Derek Long - PDDL2.2 and VAL
* Richard Howey - PDDL2.2 and VAL and Continuous Effects, derived predicates, timed initial literals and LaTeX report in VAL
* Stephen Cresswell - PDDL2.2 Parser

## Updates

* July 2019: Change license from [GNU Lesser General Public License, version 3](https://opensource.org/licenses/LGPL-3.0) to [3-Clause BSD License](https://opensource.org/licenses/BSD-3-Clause)

## [Applications](applications/README.md)

## [Libraries](libraries/README.md)

## How to compile VAL

### Windows

* Requirements:
  * [CMake](https://cmake.org/)
    * Include cmake in path for all users
  * [MinGW-w64](https://sourceforge.net/projects/mingw)
    * Install packages: mingw32-base, mingw32-gcc-g++
  * [Strawberry Perl](http://strawberryperl.com/) (required for GCC)
  * [LLVM](http://releases.llvm.org)
  * [Doxygen](http://www.doxygen.nl/download.html)

* IDE:
  * [Visual Studio Code](https://code.visualstudio.com/)
    * Extensions:
      * [CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake)
      * [CMake tools](https://marketplace.visualstudio.com/items?itemName=vector-of-bool.cmake-tools)
      * [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
      * [C++ Intellisense](https://marketplace.visualstudio.com/items?itemName=austin.code-gnu-global)
      * [Clang-Format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format)
    * Warning: You may require to restart your machine to be sure cmake, and gcc tools are identified by Visual Studio Code.

* Scripts
  * [scripts/windows/build_win64_mingw.bat](scripts/windows/build_win64_mingw.bat): Build scripts for windows 64 bits using MinGW
    * Binaries can be found in `build/win64/mingw/Release|Debug/bin`
  * [scripts/windows/build_win64_eclipse_mingw.bat](scripts/windows/build_win64_eclipse_mingw.bat): Build scripts for windows 64 bits using MinGW and creating eclipse project definition.
    * Binaries can be found in `build/win64/eclipse_mingw/Release|Debug/bin`
    * Project can be imported in eclipse following the [instructions](https://www.mantidproject.org/Setting_up_Eclipse_projects_with_CMake).
  * [scripts/windows/build_win64_vs.bat](scripts/windows/build_win64_vs.bat): Build scripts for windows 64 bits using Visual Studio Compiler
    * Binaries can be found in `build/win64/vs/Release|Debug/bin`
  * [scripts/windows/setup_dlfcn-win32.bat](scripts/windows/setup_dlfcn-win32.bat): Build scripts to build dlfcn-win32 library for windows
    * Binaries can be found in `externals/dlfcn-win32/`
  * [scripts/windows/format.bat](scripts/windows/format.bat): Scripts to format the code
  * [scripts/windows/documentation.bat](scripts/windows/documentation.bat): Generate documentation

### Linux

* Requirements:
  * Debian packages: cmake make g++ mingw-w64 flex bison
    * mingw-w64: For cross-compiling
    * flex, bison: For parser code regeneration

* IDE:
  * [Visual Studio Code](https://code.visualstudio.com/)

* Scripts
  * [scripts/linux/build_linux64.sh](scripts/linux/build_linux64.sh): Build scripts for linux 64 bits
    * Binaries can be found in `build/linux64/Release|Debug/bin`
  * [scripts/linux/build_win32.sh](scripts/linux/build_win32.sh): Build scripts for windows 32 bits
    * Binaries can be found in `build/linux_win32/Release|Debug/bin`
  * [scripts/linux/build_win64.sh](scripts/linux/build_win64.sh): Build scripts for windows 64 bits
    * Binaries can be found in `build/linux_win64/Release|Debug/bin`
  * [scripts/linux/documentation.sh](scripts/linux/documentation.sh): Generate documentation
  * [scripts/linux/format.sh](scripts/linux/format.sh): Scripts to format the code
  * [scripts/linux/setup_flex_bison.sh](scripts/linux/setup_flex_bison.sh): Build scripts to generate flex and bison header/implementation files from [libraries/VAL/src/Parser/pddl+.l](libraries/VAL/src/Parser/pddl+.l) and [libraries/VAL/src/Parser/pddl+.y](libraries/VAL/src/Parser/pddl+.y)
    * Sources files can be found in [libraries/VAL/src/Parser/pddl+.cpp](libraries/VAL/src/Parser/pddl+.cpp) and [libraries/VAL/src/Parser/pddl+.lex.yy.h](libraries/VAL/src/Parser/pddl+.lex.yy.h)

### macOS

* Requirements:
  * Xcode packages: cmake make g++ gcc flex bison
  * Note: easier to simply install Xcode in its entirety; necessary packages automatically downloaded

* IDE:
  * [Visual Studio Code](https://code.visualstudio.com/)

* Scripts
  * Note: all called (internally as well) build and clean scripts must be run through dos2unix
  * (scripts/build_macos_dev.sh): script for native macOS build
    * Binaries can be found in `build/macos64/Release/bin`
  * (scripts/clean_macos_dev.sh): script for cleaning out built binaries

  * (scripts/linux/setup_flex_bison.sh): build script to generate flex and bison header/implementation files from [libraries/VAL/src/Parser/pddl+.l](libraries/VAL/src/Parser/pddl+.l) and [libraries/VAL/src/Parser/pddl+.y](libraries/VAL/src/Parser/pddl+.y)
  * Note: flex on macOS currently causing compatibility issues
    * Do not run setup_flex_bison.sh
    * Use current Windows-generated flex header file at [libraries/VAL/src/Parser/pddl+.lex.yy.h]
  * Note: bison on macOS running well
    * Edit YACC file at [libraries/VAL/src/Parser/pddl+.y] as needed
    * Run bison command from script (bison pddl+.y -o pddl+.cpp) directly through terminal

## How to contribute to VAL

Please submit any defects as [Issues](https://github.com/KCL-Planning/VAL/issues) via GitHub.
If you are missing a feature, report it as an issue, but tag it as `[feature]` please.

We appreciate community contributions to this open sourced code repository. By submitting a pull request you certify that the contribution is your original work, you ensure the contribution is compatible with this repository [license](LICENSE) terms, and you agree (including on behalf of your employer, if applicable) that the community is free to use your contributions.

If you have a summer intern or a post-doc student and need a project for a few weeks or months, pick from the backlog of [Issues](https://github.com/KCL-Planning/VAL/issues), or just address the numerous c++ build warnings and help us modernize the codebase.
