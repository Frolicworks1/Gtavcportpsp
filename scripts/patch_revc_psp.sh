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
PICKUPS="upstream-revc/src/control/Pickups.cpp"
RADAR="upstream-revc/src/core/Radar.cpp"
DEBUGMENU="upstream-revc/src/extras/debugmenu.h"
STREAMING="upstream-revc/src/core/Streaming.cpp"
CAMERA="upstream-revc/src/core/Camera.cpp"
RADAR="upstream-revc/src/core/Radar.cpp"
CDSTREAM="upstream-revc/src/core/CdStream_posix.cpp"
CDHEADER="upstream-revc/src/core/CdStream.h"
PAD="upstream-revc/src/core/Pad.cpp"
PADHEADER="upstream-revc/src/core/Pad.h"
FRONTEND="upstream-revc/src/core/Frontend.cpp"
PAD="upstream-revc/src/core/Pad.cpp"

if [ -f "$PADHEADER" ] && ! grep -q 'void CapturePad(int padID);' "$PADHEADER"; then
  {
    cat "$PADHEADER"
    printf '%s\n' '' '#ifdef RW_PSP' '#endif'
  } > "$PADHEADER.psp.tmp"
  mv "$PADHEADER.psp.tmp" "$PADHEADER"
fi

if [ -f "$FRONTEND" ]; then
  sed -i \
    -e 's@^#if !defined RW_GL3.*@#if 0@' \
    -e 's@^#elif defined(LIBRW_SDL2).*@#elif defined(RW_PSP) || defined(LIBRW_SDL2)@' "$FRONTEND"
fi

if [ -f "$PAD" ]; then
  PADTMP="$PAD.psp.tmp"
  {
    printf '%s\n' '#ifdef RW_PSP' '#include <pspctrl.h>' 'void CapturePad(int padID);' '#include <string.h>' '#include <stdlib.h>' 'extern void CapturePad(int padID);' 'struct PspPadMouseCompat { void *window; bool cursorIsInWindow; struct { double x; double y; } lastMousePos; float mouseWheel; };' 'static PspPadMouseCompat pspPadMouseCompat = { nullptr, true, { 0.0, 0.0 }, 0.0f };' '#ifndef PSGLOBAL' '#define PSGLOBAL(var) pspPadMouseCompat.var' '#endif' 'static inline void glfwGetCursorPos(void*, double *x, double *y) { if (x) *x = pspPadMouseCompat.lastMousePos.x; if (y) *y = pspPadMouseCompat.lastMousePos.y; }' 'static inline int glfwGetMouseButton(void*, int) { return 0; }' '#define GLFW_MOUSE_BUTTON_LEFT 0' '#define GLFW_MOUSE_BUTTON_RIGHT 1' '#define GLFW_MOUSE_BUTTON_MIDDLE 2' '#define GLFW_MOUSE_BUTTON_4 3' '#define GLFW_MOUSE_BUTTON_5 4' '#endif'
    cat "$PAD"
  } > "$PADTMP"
  mv "$PADTMP" "$PAD"
fi

if [ -f "$EAX" ]; then
  EAXTMP="$EAX.psp.tmp"
  {
    printf '%s\n' '#ifndef RW_PSP'
    cat "$EAX"
    printf '%s\n' '#endif'
  } > "$EAXTMP"
  mv "$EAXTMP" "$EAX"
fi

if [ -f "$CDSTREAM" ]; then
  CDTMP="$CDSTREAM.psp.tmp"
  {
    printf '%s\n' '#ifdef RW_PSP' '#include <string.h>' '#include <stdlib.h>' '#ifndef SEM_FAILED' '#define SEM_FAILED ((sem_t*)-1)' '#endif' 'static char* psp_strdup(const char *s) { size_t n = strlen(s) + 1; char *p = (char*)malloc(n); if (p) memcpy(p, s, n); return p; }' '#endif'
    cat "$CDSTREAM"
  } > "$CDTMP"
  mv "$CDTMP" "$CDSTREAM"
  sed -i \
    -e 's@realpath(gImgNames\[0\], path);@#ifdef RW_PSP\n\tstrncpy(path, gImgNames[0], sizeof(path)-1);\n\tpath[sizeof(path)-1] = 0;\n#else\n\trealpath(gImgNames[0], path);\n#endif@' \
    -e 's@realpath(real, path);@#ifdef RW_PSP\n\t\t\tstrncpy(path, real, sizeof(path)-1);\n\t\t\tpath[sizeof(path)-1] = 0;\n#else\n\t\t\trealpath(real, path);\n#endif@' \
    -e 's/strdup(path)/psp_strdup(path)/g' "$CDSTREAM"
fi
if [ -f "$CDHEADER" ]; then
  sed -i 's/^int CdStreamGetLastPosn(void);$/int32 CdStreamGetLastPosn(void);/' "$CDHEADER"
fi

if [ -f "$STREAMING" ]; then
  sed -i 's/^CStreaming::LoadCdDirectory(const char \*dirname, int n)/CStreaming::LoadCdDirectory(const char *dirname, int32 n)/' "$STREAMING"
fi

if [ -f "$GENERAL" ]; then
  if ! grep -q 'static int GetRandomNumberInRange(int low, int high)' "$GENERAL"; then
    sed -i '/static void SetRandomSeed/i\#endif' "$GENERAL"
    sed -i '/static void SetRandomSeed/i\    static int GetRandomNumberInRange(int low, int high) { return low + (high - low) * (GetRandomNumber()/float(MYRAND_MAX + 1)); }' "$GENERAL"
    sed -i '/static void SetRandomSeed/i\#ifdef RW_PSP' "$GENERAL"
  fi
  if ! grep -q 'static int32 GetRandomNumberInRange(int low, int32 high)' "$GENERAL"; then
    sed -i '/static void SetRandomSeed/i\#endif' "$GENERAL"
    sed -i '/static void SetRandomSeed/i\    static int32 GetRandomNumberInRange(int32 low, int high) { return low + (high - low) * (GetRandomNumber()/float(MYRAND_MAX + 1)); }' "$GENERAL"
    sed -i '/static void SetRandomSeed/i\    static int32 GetRandomNumberInRange(int low, int32 high) { return low + (high - low) * (GetRandomNumber()/float(MYRAND_MAX + 1)); }' "$GENERAL"
    sed -i '/static void SetRandomSeed/i\#ifdef RW_PSP' "$GENERAL"
  fi
fi

if [ -f "$DEBUGMENU" ]; then
  DM_TMP="$DEBUGMENU.psp.tmp"
  {
    printf '%s\n' '#ifdef RW_PSP' '#include <stdint.h>' 'inline DebugMenuEntry *DebugMenuAddVar(const char *path, const char *name, int *ptr, TriggerFunc triggerFunc, int step, int lowerBound, int upperBound, const char **strings) {' '  return DebugMenuAddVar(path, name, reinterpret_cast<int32_t *>(ptr), triggerFunc, (int32_t)step, (int32_t)lowerBound, (int32_t)upperBound, strings);' '}' '#endif'
    cat "$DEBUGMENU"
  } > "$DM_TMP"
  mv "$DM_TMP" "$DEBUGMENU"
fi

if [ -f "$RADAR" ]; then
  RADARTMP="$RADAR.psp.tmp"
  {
    printf '%s\n' '#ifndef M_SQRT2' '#define M_SQRT2 1.41421356237309504880'
    cat "$RADAR"
    printf '%s\n' '#endif'
  } > "$RADARTMP"
  mv "$RADARTMP" "$RADAR"
  sed -i -e 's/^int CRadar::SetCoordBlip(/int32 CRadar::SetCoordBlip(/' -e 's/^int CRadar::SetShortRangeCoordBlip(/int32 CRadar::SetShortRangeCoordBlip(/' -e 's/^int CRadar::SetEntityBlip(/int32 CRadar::SetEntityBlip(/' "$RADAR"
fi

if [ -f "$CAMERA" ]; then
  sed -i 's/CCamera::TryToStartNewCamMode(int obbeMode)/CCamera::TryToStartNewCamMode(int32 obbeMode)/' "$CAMERA"
fi

if [ -f "$PICKUPS" ]; then
  sed -i 's/^int32 CPacManPickups::PillsEatenInRace;/int CPacManPickups::PillsEatenInRace;/' "$PICKUPS"
fi

if [ -f "$PATHFIND" ]; then
  sed -i \
    -e 's/CPathFind::CalcNodeCoors(float x, float y, float z, int id,/CPathFind::CalcNodeCoors(float x, float y, float z, int32 id,/' \
    -e 's/CPathFind::PreparePathDataForType(uint8 type, CTempNode \*tempnodes, CPathInfoForObject \*objectpathinfo,/CPathFind::PreparePathDataForType(uint8 type, CTempNode *tempnodes, CPathInfoForObject *objectpathinfo,/' \
    -e 's/float maxdist, CPathInfoForObject \*detachednodes, int numDetached/float maxdist, CPathInfoForObject *detachednodes, int32 numDetached/' \
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

FRONTEND="upstream-revc/src/core/Frontend.cpp"
if [ -f "$FRONTEND" ]; then
  sed -i 's/#if !defined RW_GL3/#if !defined RW_GL3 \\&\\& !defined RW_PSP/' "$FRONTEND"
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
