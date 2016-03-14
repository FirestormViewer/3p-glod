#!/bin/bash

# turn on verbose debugging output for parabuild logs.
set -x
# make errors fatal
set -e
# complain about unreferenced env variables
set -u

if [ -z "$AUTOBUILD" ] ; then 
    fail
fi

if [ "$OSTYPE" = "cygwin" ] ; then
    autobuild="$(cygpath -u $AUTOBUILD)"
else
    autobuild="$AUTOBUILD"
fi

# run build from root of checkout
cd "$(dirname "$0")"

# load autbuild provided shell functions and variables
set +x
eval "$("$autobuild" source_environment)"
set -x

# set LL_BUILD
set_build_variables convenience Release

top="$(pwd)"
STAGING_DIR="$(pwd)/stage"

GLOD_VERSION=1.0pre4
build=${AUTOBUILD_BUILD_ID:=0}
echo "${GLOD_VERSION}.${build}" > "${STAGING_DIR}/VERSION.txt"

case "$AUTOBUILD_PLATFORM" in
    windows*)
        build_sln "glodlib.sln" "Release|$AUTOBUILD_WIN_VSPLATFORM"

        mkdir -p stage/lib/release

        cp "lib/release/glod.lib" "stage/lib/release/glod.lib"
        cp "lib/release/glod.dll" "stage/lib/release/glod.dll"
    ;;
    darwin*)
        libdir="$top/stage/lib"
        mkdir -p "$libdir"/release
        export CFLAGS="-m$AUTOBUILD_ADDRSIZE $LL_BUILD"
        export LFLAGS="$CFLAGS"
        make -C src clean
        make -C src release
        install_name_tool -id "@executable_path/../Resources/libGLOD.dylib" "lib/libGLOD.dylib" 
        cp "lib/libGLOD.dylib" \
            "$libdir/release/libGLOD.dylib"
    ;;
    linux*)
        libdir="$top/stage/lib"
        mkdir -p "$libdir"/release
        export CFLAGS="-m$AUTOBUILD_ADDRSIZE $LL_BUILD"
        export LFLAGS="$CFLAGS"
        make -C src clean
        make -C src release
        cp "lib/libGLOD.so" \
            "$libdir/release/libGLOD.so"
    ;;
esac
mkdir -p "stage/include/glod"
cp "include/glod.h" "stage/include/glod/glod.h"
mkdir -p stage/LICENSES
cp LICENSE stage/LICENSES/GLOD.txt

pass

