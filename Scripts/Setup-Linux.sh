#!/bin/bash

CORE_TESTS=OFF
BUILD_TYPE=Release
COMPILE_COMMANDS=OFF

while [[ $# -gt 0 ]]; do
    case "$1" in
        --core-tests)
            CORE_TESTS=ON
            shift
            ;;
        --debug)
            BUILD_TYPE=Debug
            shift
            ;;
        --compile-commands)
            COMPILE_COMMANDS=ON
            shift
            ;;
        *)
            echo "Unknown option: $1"
            echo "Usage: $0 [--core-tests] [--debug] [--compile-commands]"
            exit 1
            ;;
    esac
done

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$ROOT_DIR/build"

mkdir -p "$BUILD_DIR"

pushd "$BUILD_DIR" > /dev/null

cmake "$ROOT_DIR" \
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
    -DBUILD_CORE_TESTS="$CORE_TESTS" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS="$COMPILE_COMMANDS"

cmake --build .

popd > /dev/null
