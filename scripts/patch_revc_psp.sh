#!/bin/sh
set -eu

RW="upstream-revc/vendor/librw/rw.h"
BASE="upstream-revc/vendor/librw/src/rwbase.h"
ENG="upstream-revc/vendor/librw/src/engine.cpp"
CMAKE="upstream-revc/vendor/librw/src/CMakeLists.txt"
SRC_CMAKE="upstream-revc/src/CMakeLists.txt"
ROOT="upstream-revc/CMakeLists.txt"
CTRL="upstream-revc/src/core/ControllerConfig.h"
EAX="upstream-revc/src/audio/eax/eax-util.cpp"
COLSTORE="upstream-revc/src/collision/ColStore.cpp"
GENERAL="upstream-revc/src/core/General.h"
CARCTRL="upstream-revc/src/control/CarCtrl.cpp"
PATHFIND="upstream-revc/src/control/PathFind.cpp"

# PSP desktop EAX source is never compiled: it requires DirectSound headers.
if [ -f "$EAX" ]; then
  sed -i '1i #ifndef RW_PSP' "$EAX"
  printf '\n#endif\n' >> "$EAX"
fi

# PSP/MIPS exposes int32 as a wider integer type than host int. Add an exact
# int overload so integer literals do not ambiguously match float vs int32.
if [ -f "$GENERAL" ]; then
  if ! grep -q "static int GetRandomNumberInRange(int low, int high)" "$GENERAL"; then
    sed -i '/static void SetRandomSeed/i #ifdef RW_PSP\n\tstatic int GetRandomNumberInRange(int low, int high)\n\t\t{ return GetRandomNumberInRange((int32)low, (int32)high); }\n#endif' "$GENERAL"
  fi
  if ! grep -q "static int32 GetRandomNumberInRange(int low, int32 high)" "$GENERAL"; then
    sed -i '/static void SetRandomSeed/i #ifdef RW_PSP\n\tstatic int32 GetRandomNumberInRange(int low, int32 high)\n\t\t{ return GetRandomNumberInRange((int32)low, high); }\n\tstatic int32 GetRandomNumberInRange(int32 low, int high)\n\t\t{ return GetRandomNumberInRange(low, (int32)high); }\n#endif' "$GENERAL"
  fi
fi
if [ -f "$PATHFIND" ]; then
  sed -i \
    -e 's/CPathFind::CalcNodeCoors(float x, float y, float z, int id,/CPathFind::CalcNodeCoors(float x, float y, float z, int32 id,/' \
    -e 's/CPathFind::PreparePathDataForType(uint8 type, CTempNode \*tempnodes, CPathInfoForObject \*objectpathinfo,/CPathFind::PreparePathDataForType(uint8 type, CTempNode *tempnodes, CPathInfoForObject *objectpathinfo,/' \
    -e 's/float, CPathInfoForObject \*objectpathinfo, int)/float, CPathInfoForObject *objectpathinfo, int32)/' \
    "$PATHFIND"
fi

if [ -f "$CARCTRL" ]; then
  sed -i \
    -e 's/^int CCarCtrl::NumLawEnforcerCars;/int32 CCarCtrl::NumLawEnforcerCars;/' \
    -e 's/^int CCarCtrl::NumAmbulancesOnDuty;/int32 CCarCtrl::NumAmbulancesOnDuty;/' \
    -e 's/^int CCarCtrl::NumFiretrucksOnDuty;/int32 CCarCtrl::NumFiretrucksOnDuty;/' \
    -e 's/CCarCtrl::RemoveFromLoadedVehicleArray(int mi, int32 rating)/CCarCtrl::RemoveFromLoadedVehicleArray(int32 mi, int32 rating)/' \
    -e 's/CCarCtrl::ChooseCarModelToLoad(int rating)/CCarCtrl::ChooseCarModelToLoad(int32 rating)/' \
    "$CARCTRL"
fi

# Keep PSP/MIPS integer typedefs consistent with the class declaration.
if [ -f "$COLSTORE" ]; then
  sed -i 's/CColStore::RemoveColSlot(int slot)/CColStore::RemoveColSlot(int32 slot)/' "$COLSTORE"
fi

# PSP controller path: reuse the GL3-style JoyState representation and disable
# the Windows DirectInput layout/size validation.
if [ -f "$CTRL" ]; then
  sed -i 's@^#if defined RW_GL3$@#if defined RW_GL3 || defined RW_PSP@' "$CTRL"
  sed -i 's@^#ifdef RW_GL3$@#if defined RW_GL3 || defined RW_PSP@' "$CTRL"
  sed -i 's@^#ifndef RW_GL3$@#if 0@' "$CTRL"

  if ! grep -q '^#if defined RW_GL3 || defined RW_PSP$' "$CTRL"; then
    sed -i '/^#define ACTIONNAME_LENGTH 40$/a #if defined RW_PSP\nstruct JoyState {\n    int8 id;\n    bool isGamepad;\n    uint8 numButtons;\n    uint8 buttons[MAX_BUTTONS];\n    bool mappedButtons[MAX_BUTTONS];\n};\n#endif' "$CTRL"
  fi

  grep -q '^#if defined RW_GL3 || defined RW_PSP$' "$CTRL"
  grep -q 'JoyState' "$CTRL"
  ! grep -q 'DIJOYSTATE2' "$CTRL"
fi

# Remove non-PSP device/plugin headers from common librw headers.
for f in "$RW" "$ENG"; do
  sed -i \
    -e '/rwps2plg\.h/d' \
    -e '/src\/ps2\/rwps2.h/d' \
    -e '/src\/d3d\/rwxbox.h/d' \
    -e '/src\/d3d\/rwd3d.h/d' \
    -e '/src\/d3d\/rwd3d8.h/d' \
    -e '/src\/d3d\/rwd3d9.h/d' \
    -e '/src\/gl\/rwwdgl.h/d' \
    -e '/src\/gl\/rwgl3.h/d' \
    -e '/src\/gl\/rwgl3shader.h/d' \
    -e '/src\/gl\/rwgl3plg.h/d' \
    "$f"
done

# Expose the PSP device namespace from rw.h.
if ! grep -q 'src/psp/rwpsp.h' "$RW"; then
  sed -i '/#include "src\/rwobjects.h"/a #include "src/psp/rwpsp.h"' "$RW"
fi

# Select the PSP RenderWare device.
if ! grep -q '#define RWDEVICE psp' "$BASE"; then
  sed -i '/#ifdef RW_GL3/i #ifdef RW_PSP\n#define RWDEVICE psp\n#endif' "$BASE"
fi

if ! grep -q '#include "psp/rwpsp.h"' "$ENG"; then
  sed -i '/#include "rwengine.h"/a #include "psp/rwpsp.h"' "$ENG"
fi
sed -i 's/ps2::registerPlatformPlugins()/psp::registerPlatformPlugins()/g' "$ENG"
sed -i '/xbox::registerPlatformPlugins()/d;/d3d8::registerPlatformPlugins()/d;/d3d9::registerPlatformPlugins()/d;/wdgl::registerPlatformPlugins()/d;/gl3::registerPlatformPlugins()/d' "$ENG"
sed -i '/#ifdef RW_PS2/a #elif defined(RW_PSP)\n\tengine->device = psp::renderdevice;' "$ENG"

# Upstream librw lists all desktop/PS2 backend sources unconditionally. PSP must
# build only the common core plus the native GU backend.
sed -i \
  -e '/^[[:space:]]*d3d\//d' \
  -e '/^[[:space:]]*gl\//d' \
  -e '/^[[:space:]]*ps2\//d' \
  "$CMAKE"

if ! grep -q 'psp/rwpsp.cpp' "$CMAKE"; then
  sed -i '/    lodepng\/lodepng.h/i\\    psp/rwpsp.cpp\n    psp/rwpsp.h' "$CMAKE"
fi

grep -q 'psp/rwpsp.cpp' "$CMAKE"
! grep -q '^[[:space:]]*ps2/' "$CMAKE"
! grep -q '^[[:space:]]*d3d/' "$CMAKE"
! grep -q '^[[:space:]]*gl/' "$CMAKE"


# PSP uses the built-in null audio path. This prevents desktop OpenAL/DirectInput
# dependencies from entering the Allegrex build; no fake OpenAL headers are used.
if [ -f "$ROOT" ]; then
  sed -i \
    -e 's@set(\${PROJECT}_AUDIOS "OAL")@set(\${PROJECT}_AUDIOS "NULL")@' \
    -e 's@set(\${PROJECT}_AUDIO "OAL" CACHE STRING "Audio")@set(\${PROJECT}_AUDIO "NULL" CACHE STRING "Audio")@' \
    "$ROOT"
fi
sed -i 's@if(NOT TARGET MPG123::libmpg123)@if(NOT RW_PSP AND NOT TARGET MPG123::libmpg123)@' "$SRC_CMAKE"
if [ -f "$SRC_CMAKE" ]; then
  sed -i '/^target_link_libraries(\${EXECUTABLE} PRIVATE$/i if(RW_PSP AND NOT TARGET MPG123::libmpg123)\n  add_library(MPG123::libmpg123 INTERFACE IMPORTED)\nendif()' "$SRC_CMAKE"
fi
sed -i 's@if(\${PROJECT}_WITH_OPUS)@if(NOT RW_PSP AND \${PROJECT}_WITH_OPUS)@' "$SRC_CMAKE"
if [ -f "$SRC_CMAKE" ]; then
  sed -i '/^file(GLOB_RECURSE /a if(RW_PSP)\n  list(REMOVE_ITEM \\${PROJECT}_SOURCES\n    "\${CMAKE_CURRENT_SOURCE_DIR}/audio/eax/eax-util.cpp"\n    "\${CMAKE_CURRENT_SOURCE_DIR}/audio/oal/aldlist.cpp"\n    "\${CMAKE_CURRENT_SOURCE_DIR}/audio/oal/channel.cpp"\n    "\${CMAKE_CURRENT_SOURCE_DIR}/audio/oal/oal_utils.cpp"\n    "\${CMAKE_CURRENT_SOURCE_DIR}/audio/oal/stream.cpp"\n    "\${CMAKE_CURRENT_SOURCE_DIR}/audio/sampman_oal.cpp")\nendif()' "$SRC_CMAKE"
fi

exit 0
