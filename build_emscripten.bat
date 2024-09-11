@echo off

REM Create build directory
SET BUILD_DIR=.\out\build\release-emscripten
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

REM Create binary directory
SET INTERMEDIATE_DIR=%BUILD_DIR%\intermediate
if not exist %INTERMEDIATE_DIR% mkdir %INTERMEDIATE_DIR%

REM TODO: Decide on better idl workflow
SET IDL_FILE=src\preemo.idl

REM Create glue files
SET GLUE_FILENAME=glue
SET GLUE_FILE_PATH=%INTERMEDIATE_DIR%\%GLUE_FILENAME%
call %EMSDK%\upstream\emscripten\tools\webidl_binder %IDL_FILE% %GLUE_FILE_PATH%

REM Create idl wrapper file
SET WRAPPER_FILENAME=%INTERMEDIATE_DIR%\webidl_wrapper.cpp
echo #include "PreemoRoot.h" > %WRAPPER_FILENAME%
echo #include "%GLUE_FILENAME%.cpp" >> %WRAPPER_FILENAME%
echo //--- >> %WRAPPER_FILENAME%

REM Run cmake
call emcmake cmake -B %BUILD_DIR%
call cmake --build %BUILD_DIR%
