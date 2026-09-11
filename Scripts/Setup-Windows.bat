@echo off
setlocal

set CORE_TESTS=OFF
set BUILD_TYPE=Release

:parse
if "%~1"=="" goto build

if "%~1"=="--core-tests" (
    set CORE_TESTS=ON
    shift
    goto parse
)

if "%~1"=="--debug" (
    set BUILD_TYPE=Debug
    shift
    goto parse
)

echo Unknown option: %~1
echo Usage: %~nx0 [--core-tests] [--debug]
exit /b 1

:build
if not exist ..\build mkdir ..\build

pushd ..\build

cmake .. ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DBUILD_CORE_TESTS=%CORE_TESTS%

cmake --build . --config %BUILD_TYPE%

popd

endlocal
