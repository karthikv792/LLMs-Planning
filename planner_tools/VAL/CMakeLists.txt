cmake_minimum_required (VERSION 2.8)
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Display command line run by cmake
if (NOT CMAKE_VERBOSE_MAKEFILE)
  set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

# Installation configuration
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
if( APPLE )
  set(CMAKE_MACOSX_RPATH TRUE)
  set(CMAKE_INSTALL_RPATH "@executable_path")
else()
  set(CMAKE_INSTALL_RPATH "$ORIGIN")
endif()
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

project (Val)

get_filename_component(APPLICATIONS_DIR applications ABSOLUTE)

get_filename_component(EXTERNALS_DIR externals ABSOLUTE)

get_filename_component(LIBRARIES_DIR libraries ABSOLUTE)
set(LIBRARY_VAL_DIR ${LIBRARIES_DIR}/VAL)
set(LIBRARY_VAL_INCLUDE_DIR ${LIBRARY_VAL_DIR}/include)
set(LIBRARY_VAL_SRC_DIR ${LIBRARY_VAL_DIR}/src)

include_directories(${LIBRARY_VAL_INCLUDE_DIR} ${LIBRARY_VAL_INCLUDE_DIR}/Parser)

# Build not using shared library VAL (For development purposes only)
set (VERIFY_SYMBOLS False)

# Use Debug build type by default if none specified
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug)
endif()

# Set output folder
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(CMAKE_COMPILER_IS_GNUCC)
  # needed for gcc 4.6+
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fpermissive")
endif()

# Enable/Disable external solver feature through dl library
if (NOT SUPPORT_EXTERNALS)
  set(SUPPORT_EXTERNALS False)
endif()

# VAL
set(LIB_VAL_SOURCE_FILES
    ${LIBRARY_VAL_SRC_DIR}/Parser/pddl+.cpp
    ${LIBRARY_VAL_SRC_DIR}/AccumulatedDelta.cpp
    ${LIBRARY_VAL_SRC_DIR}/Action.cpp
    ${LIBRARY_VAL_SRC_DIR}/CausalGraph.cpp
    ${LIBRARY_VAL_SRC_DIR}/DebugWriteController.cpp
    ${LIBRARY_VAL_SRC_DIR}/Environment.cpp
    ${LIBRARY_VAL_SRC_DIR}/Events.cpp
    ${LIBRARY_VAL_SRC_DIR}/FastEnvironment.cpp
    ${LIBRARY_VAL_SRC_DIR}/FuncAnalysis.cpp
    ${LIBRARY_VAL_SRC_DIR}/FuncExp.cpp
    ${LIBRARY_VAL_SRC_DIR}/HowAnalyser.cpp
    ${LIBRARY_VAL_SRC_DIR}/instantiation.cpp
    ${LIBRARY_VAL_SRC_DIR}/LaTeXSupport.cpp
    ${LIBRARY_VAL_SRC_DIR}/LibLink.cpp
    ${LIBRARY_VAL_SRC_DIR}/Ownership.cpp
    ${LIBRARY_VAL_SRC_DIR}/PinguPlanGenerator.cpp
    ${LIBRARY_VAL_SRC_DIR}/Plan.cpp
    ${LIBRARY_VAL_SRC_DIR}/Polynomial.cpp
    ${LIBRARY_VAL_SRC_DIR}/PrettyPrinter.cpp
    ${LIBRARY_VAL_SRC_DIR}/Proposition.cpp
    ${LIBRARY_VAL_SRC_DIR}/ptree.cpp
    ${LIBRARY_VAL_SRC_DIR}/random.cpp
    ${LIBRARY_VAL_SRC_DIR}/RelaxTranslator.cpp
    ${LIBRARY_VAL_SRC_DIR}/RepairAdvice.cpp
    ${LIBRARY_VAL_SRC_DIR}/RobustAnalyse.cpp
    ${LIBRARY_VAL_SRC_DIR}/SASActions.cpp
    ${LIBRARY_VAL_SRC_DIR}/SimpleEval.cpp
    ${LIBRARY_VAL_SRC_DIR}/State.cpp
    ${LIBRARY_VAL_SRC_DIR}/TIM.cpp
    ${LIBRARY_VAL_SRC_DIR}/TimSupport.cpp
    ${LIBRARY_VAL_SRC_DIR}/ToFunction.cpp
    ${LIBRARY_VAL_SRC_DIR}/TrajectoryConstraints.cpp
    ${LIBRARY_VAL_SRC_DIR}/typecheck.cpp
    ${LIBRARY_VAL_SRC_DIR}/TypedAnalyser.cpp
    ${LIBRARY_VAL_SRC_DIR}/Utils.cpp
    ${LIBRARY_VAL_SRC_DIR}/Validator.cpp
    ${LIBRARY_VAL_SRC_DIR}/valLib.cpp)
add_library(VAL SHARED ${LIB_VAL_SOURCE_FILES})
if( CMAKE_COMPILER_IS_GNUCC )
    target_compile_options(VAL PUBLIC "-DVAL_EXPORTS")
endif()
# Add source files of parser in the Debug area for debugging.
if ( ${CMAKE_BUILD_TYPE} MATCHES Debug )
    add_custom_command(TARGET VAL POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBRARY_VAL_SRC_DIR}/Parser/pddl+.y ${LIBRARY_VAL_SRC_DIR}/Parser/pddl+.cpp ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endif()
if( WIN32 )
    target_compile_options(VAL PUBLIC "-DYY_NO_UNISTD_H")
endif()
# Support of external solvers feature
if( ${SUPPORT_EXTERNALS} )
    target_compile_options(VAL PUBLIC "-DSUPPORTEXTERNALS")
    target_include_directories(VAL PUBLIC ${EXTERNALS_DIR}/dlfcn-win32/include)
    if( WIN32 )
        find_library(LIB_DLFCN libdl.dll ${EXTERNALS_DIR}/dlfcn-win32/lib/win64)
        target_link_libraries(VAL ${LIB_DLFCN})
    else()
        target_link_libraries(VAL dl)
    endif()
endif()

# Analyse
set(ANALYSE_SOURCE_FILES
    ${APPLICATIONS_DIR}/Analyse/src/main.cpp)
add_executable(Analyse ${ANALYSE_SOURCE_FILES})
target_link_libraries(Analyse VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(Analyse2 ${ANALYSE_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# DomainView
set(DOMAIN_VIEW_SOURCE_FILES
    ${APPLICATIONS_DIR}/DomainView/src/main.cpp)
add_executable(DomainView ${DOMAIN_VIEW_SOURCE_FILES})
target_link_libraries(DomainView VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(DomainView2 ${DOMAIN_VIEW_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# HowWhatWhen
set(HOW_WHAT_WHEN_SOURCE_FILES
    ${APPLICATIONS_DIR}/HowWhatWhen/src/main.cpp)
add_executable(HowWhatWhen ${HOW_WHAT_WHEN_SOURCE_FILES})
target_link_libraries(HowWhatWhen VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(HowWhatWhen2 ${HOW_WHAT_WHEN_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# Instantiate
set(INSTANTIATE_SOURCE_FILES
    ${APPLICATIONS_DIR}/Instantiate/src/main.cpp)
add_executable(Instantiate ${INSTANTIATE_SOURCE_FILES})
target_link_libraries(Instantiate VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(Instantiate2 ${INSTANTIATE_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# Instantiation IP
#set(INSTANTIATION_IP_SOURCE_FILES
#    ${APPLICATIONS_DIR}/InstantiationIP/src/main.cpp)
#add_executable(InstantiationIP ${INSTANTIATION_IP_SOURCE_FILES})
#target_link_libraries(InstantiationIP VAL)
#if( ${VERIFY_SYMBOLS} )
#    add_executable(InstantiationIP2 ${INSTANTIATION_IP_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
#endif()

# Parser
set(PARSER_SOURCE_FILES
    ${APPLICATIONS_DIR}/Parser/src/main.cpp)
add_executable(Parser ${PARSER_SOURCE_FILES})
target_link_libraries(Parser VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(Parser2 ${PARSER_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# PinguPlan
set(PINGUPLAN_SOURCE_FILES
    ${APPLICATIONS_DIR}/PinguPlan/src/main.cpp)
add_executable(PinguPlan ${PINGUPLAN_SOURCE_FILES})
target_link_libraries(PinguPlan VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(PinguPlan2 ${PINGUPLAN_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# PlanRec
set(PLANREC_SOURCE_FILES
    ${APPLICATIONS_DIR}/PlanRec/src/main.cpp)
add_executable(PlanRec ${PLANREC_SOURCE_FILES})
target_link_libraries(PlanRec VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(PlanRec2 ${PLANREC_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# PlanSeqStep
set(PLAN_SEQ_STEP_SOURCE_FILES
    ${APPLICATIONS_DIR}/PlanSeqStep/src/main.cpp)
add_executable(PlanSeqStep ${PLAN_SEQ_STEP_SOURCE_FILES})
target_link_libraries(PlanSeqStep VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(PlanSeqStep2 ${PLAN_SEQ_STEP_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# PlanToValStep
set(PLAN_TO_VAL_STEP_SOURCE_FILES
    ${APPLICATIONS_DIR}/PlanToValStep/src/main.cpp)
add_executable(PlanToValStep ${PLAN_TO_VAL_STEP_SOURCE_FILES})
target_link_libraries(PlanToValStep VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(PlanToValStep2 ${PLAN_TO_VAL_STEP_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# Relax
set(RELAX_SOURCE_FILES
    ${APPLICATIONS_DIR}/Relax/src/main.cpp)
add_executable(Relax ${RELAX_SOURCE_FILES})
target_link_libraries(Relax VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(Relax2 ${RELAX_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# TIM
set(TIM_SOURCE_FILES
    ${APPLICATIONS_DIR}/TIM/src/main.cpp)
add_executable(TIM ${TIM_SOURCE_FILES})
target_link_libraries(TIM VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(TIM2 ${TIM_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# ToFn
set(TOFN_SOURCE_FILES
    ${APPLICATIONS_DIR}/ToFn/src/main.cpp)
add_executable(ToFn ${TOFN_SOURCE_FILES})
target_link_libraries(ToFn VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(ToFn2 ${TOFN_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# TypedAnalysis
set(TAN_SOURCE_FILES
    ${APPLICATIONS_DIR}/TypeAnalysis/src/main.cpp)
add_executable(TypeAnalysis ${TAN_SOURCE_FILES})
target_link_libraries(TypeAnalysis VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(TypeAnalysis2 ${TAN_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# Validate
set(VALIDATE_SOURCE_FILES
    ${APPLICATIONS_DIR}/Validate/src/main.cpp)    
add_executable(Validate ${VALIDATE_SOURCE_FILES})
target_link_libraries(Validate VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(Validate2 ${VALIDATE_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# ValStep
set(VALSTEP_SOURCE_FILES
    ${APPLICATIONS_DIR}/ValStep/src/main.cpp)
add_executable(ValStep ${VALSTEP_SOURCE_FILES})
target_link_libraries(ValStep VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(ValStep2 ${VALSTEP_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# ValueSeq
set(VALUE_SEQ_SOURCE_FILES
    ${APPLICATIONS_DIR}/ValueSeq/src/main.cpp)
add_executable(ValueSeq ${VALUE_SEQ_SOURCE_FILES})
target_link_libraries(ValueSeq VAL)
if( ${VERIFY_SYMBOLS} )
    add_executable(ValueSeq2 ${VALUE_SEQ_SOURCE_FILES} ${LIB_VAL_SOURCE_FILES})
endif()

# Targets to be installed
install(
    TARGETS VAL Analyse DomainView HowWhatWhen Instantiate Parser PinguPlan PlanRec PlanSeqStep PlanToValStep Relax TIM ToFn TypeAnalysis Validate ValStep ValueSeq
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION bin
)

install(
    FILES ${APPLICATIONS_DIR}/README.md
    DESTINATION .
)
if( WIN32 )
    if( TOOLCHAIN_PREFIX STREQUAL i686-w64-mingw32 )
        file(GLOB_RECURSE LIBGCC_PATH /usr/lib/gcc/${TOOLCHAIN_PREFIX}/*win32/libgcc_s_sjlj-1.dll)
        file(GLOB_RECURSE LIBSTD_PATH /usr/lib/gcc/${TOOLCHAIN_PREFIX}/*win32/libstdc++-6.dll)
        file(GLOB_RECURSE LIBWINPTHREAD_PATH /usr/${TOOLCHAIN_PREFIX}/lib/libwinpthread-1.dll)
        install(
            FILES ${LIBGCC_PATH} ${LIBSTD_PATH} ${LIBWINPTHREAD_PATH}
            DESTINATION bin
        )
    elseif( TOOLCHAIN_PREFIX STREQUAL x86_64-w64-mingw32 )
        file(GLOB_RECURSE LIBGCC_PATH /usr/lib/gcc/${TOOLCHAIN_PREFIX}/*win32/libgcc_s_seh-1.dll)
        file(GLOB_RECURSE LIBSTD_PATH /usr/lib/gcc/${TOOLCHAIN_PREFIX}/*win32/libstdc++-6.dll)
        file(GLOB_RECURSE LIBWINPTHREAD_PATH /usr/${TOOLCHAIN_PREFIX}/lib/libwinpthread-1.dll)
        install(
            FILES ${LIBGCC_PATH} ${LIBSTD_PATH} ${LIBWINPTHREAD_PATH}
            DESTINATION bin
        )
    endif()
endif()

# Configuration of CPack
include (InstallRequiredSystemLibraries)
if( WIN32 )
    set(CPACK_GENERATOR "ZIP")    
    SET(CPACK_SOURCE_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "ZIP;TGZ")
    SET(CPACK_SOURCE_GENERATOR "ZIP;TGZ")
endif()
set (CPACK_PACKAGE_VERSION "$ENV{BUILD_BUILDNUMBER}")
include (CPack)