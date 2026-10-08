#!/bin/sh
set -eux

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
DEBUGMENUCPP="upstream-revc/src/extras/debugmenu.cpp"
STREAMING="upstream-revc/src/core/Streaming.cpp"
RE3="upstream-revc/src/core/re3.cpp"
DEBUGMENU="upstream-revc/src/extras/debugmenu.h"
FAKERW="upstream-revc/src/fakerw/fake.cpp"
CAMERA="upstream-revc/src/core/Camera.cpp"
RADAR="upstream-revc/src/core/Radar.cpp"
CDSTREAM="upstream-revc/src/core/CdStream_posix.cpp"
CDHEADER="upstream-revc/src/core/CdStream.h"
PAD="upstream-revc/src/core/Pad.cpp"
PADHEADER="upstream-revc/src/core/Pad.h"
FRONTEND="upstream-revc/src/core/Frontend.cpp"
PEDH="upstream-revc/src/peds/Ped.h"
PEDATTR="upstream-revc/src/peds/PedAttractor.h"
POP="upstream-revc/src/peds/Population.cpp"
POPH="upstream-revc/src/peds/Population.h"
PAD="upstream-revc/src/core/Pad.cpp"

if [ -f "$PADHEADER" ] && ! grep -q 'void CapturePad(int padID);' "$PADHEADER"; then
  {
    cat "$PADHEADER"
    printf '%s\n' '' '#ifdef RW_PSP' '#endif'
  } > "$PADHEADER.psp.tmp"
  mv "$PADHEADER.psp.tmp" "$PADHEADER"
fi

if [ -f "$POPH" ]; then
  sed -i \
    -e 's/static int32 ChooseGangOccupation(int);/static int32 ChooseGangOccupation(int32);/' \
    -e 's/static void PlaceGangMembers(ePedType pedType, int pedAmount/static void PlaceGangMembers(ePedType pedType, int32 pedAmount/' \
    -e 's/static void PlaceGangMembersInFormation(ePedType pedType, int pedAmount/static void PlaceGangMembersInFormation(ePedType pedType, int32 pedAmount/' \
    -e 's/static void PlaceGangMembersInCircle(ePedType pedType, int pedAmount/static void PlaceGangMembersInCircle(ePedType pedType, int32 pedAmount/' \
    "$POPH"
fi

if [ -f "$POP" ]; then
  # Keep Population definitions identical to int32 declarations under PSP.
  # Use separate substitutions for BusyBox/POSIX sed portability and verify each result.
  # Match only ChooseGangOccupation's return type and parameter typedef.
  awk 'prev == "int" && $0 == "CPopulation::ChooseGangOccupation(int gangId)" { print "int32"; print "CPopulation::ChooseGangOccupation(int32 gangId)"; prev=""; next } { if (prev != "") print prev; prev=$0 } END { if (prev != "") print prev }' "$POP" > "$POP.psp.tmp"
  mv "$POP.psp.tmp" "$POP"
  sed -i 's/CPopulation::PlaceGangMembers(ePedType pedType, int pedAmount/CPopulation::PlaceGangMembers(ePedType pedType, int32 pedAmount/' "$POP"
  sed -i 's/CPopulation::PlaceGangMembersInFormation(ePedType pedType, int pedAmount/CPopulation::PlaceGangMembersInFormation(ePedType pedType, int32 pedAmount/' "$POP"
  sed -i 's/CPopulation::PlaceGangMembersInCircle(ePedType pedType, int pedAmount/CPopulation::PlaceGangMembersInCircle(ePedType pedType, int32 pedAmount/' "$POP"
  grep -Fq 'CPopulation::ChooseGangOccupation(int32 gangId)' "$POP"
  grep -Fq 'CPopulation::PlaceGangMembers(ePedType pedType, int32 pedAmount' "$POP"
  grep -Fq 'CPopulation::PlaceGangMembersInFormation(ePedType pedType, int32 pedAmount' "$POP"
  grep -Fq 'CPopulation::PlaceGangMembersInCircle(ePedType pedType, int32 pedAmount' "$POP"
fi

if [ -f "$PEDATTR" ]; then
  # Keep PedAttractor declarations identical to PSP int32 definitions.
  sed -i 's/ComputeAttractPos(int qid,/ComputeAttractPos(int32 qid,/g; s/ComputeAttractHeading(int qid,/ComputeAttractHeading(int32 qid,/g' "$PEDATTR"
fi

if [ -f "$PEDH" ]; then
  # Keep declaration and definition identical under PSP's int32 typedef.
  sed -i '/SetNewAttraction/s/, int);/, int32);/' "$PEDH"
  grep -Fq 'SetNewAttraction(CPedAttractor* pAttractor, const CVector& pos, float, float, int32);' "$PEDH"
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
CDSTREAMPOSIX="upstream-revc/src/core/CdStream_posix.cpp"
if [ -f "$CDSTREAMPOSIX" ]; then
  awk 'prev == "int32" && $0 == "CdStreamGetLastPosn(void)" { print "int"; print; prev=""; next } { if (prev != "") print prev; prev=$0 } END { if (prev != "") print prev }' "$CDSTREAMPOSIX" > "$CDSTREAMPOSIX.psp.tmp"
  mv "$CDSTREAMPOSIX.psp.tmp" "$CDSTREAMPOSIX"
  awk 'prev == "int" && $0 == "CdStreamGetLastPosn(void)" { found=1 } { prev=$0 } END { exit(found ? 0 : 1) }' "$CDSTREAMPOSIX"
fi
if [ -f "$CDHEADER" ]; then
  sed -i 's/^int32 CdStreamGetLastPosn(void);$/int CdStreamGetLastPosn(void);/' "$CDHEADER"
  sed -i 's/^int CdStreamGetLastPosn(void);$/int CdStreamGetLastPosn(void);/' "$CDHEADER"
fi

if [ -f "$STREAMING" ]; then
  sed -i 's/^CStreaming::LoadCdDirectory(const char \*dirname, int n)/CStreaming::LoadCdDirectory(const char *dirname, int32 n)/' "$STREAMING"
fi

if [ -f "$GENERAL" ]; then
  if ! grep -q 'static int32 GetRandomNumberInRange(int low, int32 high)' "$GENERAL"; then
    sed -i '/static void SetRandomSeed/i\#endif' "$GENERAL"
    sed -i '/static void SetRandomSeed/i\    static int32 GetRandomNumberInRange(int low, int32 high) { return (int32)(low + (high - low) * (GetRandomNumber()/float(MYRAND_MAX + 1))); }' "$GENERAL"
    sed -i '/static void SetRandomSeed/i\#ifdef RW_PSP' "$GENERAL"
  fi
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
    sed -i '/^#define ACTIONNAME_LENGTH 40$/a #if defined RW_PSP\nstruct JoyState {
    int8 id;
    bool isGamepad;
    uint8 numButtons;
    uint8 buttons[MAX_BUTTONS];
    bool mappedButtons[MAX_BUTTONS];\n};\n#endif' "$CTRL"
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
  awk '/    lodepng\/lodepng.h/ { print "    psp/rwpsp.cpp"; print "    psp/rwpsp.h"; print "    psp/rwpsp_plugins.cpp" } { print }' "$CMAKE" > "$CMAKE.psp.tmp"
  mv "$CMAKE.psp.tmp" "$CMAKE"
fi

grep -q 'psp/rwpsp.cpp' "$CMAKE"
grep -q 'psp/rwpsp_plugins.cpp' "$CMAKE"
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
  awk '/^target_link_libraries/ {
    print "if(RW_PSP AND NOT TARGET MPG123::libmpg123)"
    print "  add_library(MPG123::libmpg123 INTERFACE IMPORTED)"
    print "endif()"
  } { print }' "$SRC_CMAKE" > "$SRC_CMAKE.psp.tmp"
  mv "$SRC_CMAKE.psp.tmp" "$SRC_CMAKE"
fi
if [ -f "$SRC_CMAKE" ]; then
  if ! grep -q 'pspgu pspgum pspctrl pspge pspdisplay pspdebug' "$SRC_CMAKE"; then
    sed -i '/^target_link_libraries(\${EXECUTABLE} PRIVATE/i if(RW_PSP)\ntarget_link_libraries(\${EXECUTABLE} PRIVATE pspgu pspgum pspctrl pspge pspdisplay)\nendif()' "$SRC_CMAKE"
  fi
  grep -q 'pspgu pspgum pspctrl pspge pspdisplay' "$SRC_CMAKE"
fi
sed -i 's@if(\${PROJECT}_WITH_OPUS)@if(NOT RW_PSP AND \${PROJECT}_WITH_OPUS)@' "$SRC_CMAKE"
if [ -f "$SRC_CMAKE" ]; then
  awk '/^file\(GLOB_RECURSE / {
    print "if(RW_PSP)"
    print "  list(REMOVE_ITEM ${PROJECT}_SOURCES"
    print "    ${CMAKE_CURRENT_SOURCE_DIR}/extras/custompipes_d3d9.cpp"
    print "    ${CMAKE_CURRENT_SOURCE_DIR}/extras/custompipes_gl.cpp"
    print "    ${CMAKE_CURRENT_SOURCE_DIR}/core/CdStream_posix.cpp"
    print "  )"
    print "endif()"
  } { print }' "$SRC_CMAKE" > "$SRC_CMAKE.psp.tmp"
  mv "$SRC_CMAKE.psp.tmp" "$SRC_CMAKE"
  awk '/^file\(GLOB_RECURSE / {
    print "if(RW_PSP)"
    print "  list(REMOVE_ITEM ${PROJECT}_SOURCES"
    print "    ${CMAKE_CURRENT_SOURCE_DIR}/audio/eax/eax-util.cpp"
    print "    ${CMAKE_CURRENT_SOURCE_DIR}/audio/oal/aldlist.cpp"
    print "    ${CMAKE_CURRENT_SOURCE_DIR}/audio/oal/channel.cpp"
    print "    ${CMAKE_CURRENT_SOURCE_DIR}/audio/oal/oal_utils.cpp"
    print "    ${CMAKE_CURRENT_SOURCE_DIR}/audio/oal/stream.cpp"
    print "    ${CMAKE_CURRENT_SOURCE_DIR}/audio/sampman_oal.cpp"
    print "  )"
    print "endif()"
  } { print }' "$SRC_CMAKE" > "$SRC_CMAKE.psp.tmp"
  mv "$SRC_CMAKE.psp.tmp" "$SRC_CMAKE"
fi
if [ -f "$SRC_CMAKE" ]; then
  sed -i '/CdStream_posix.cpp/d' "$SRC_CMAKE"
fi

if [ -f "$RADAR" ]; then
  sed -i     -e 's/^int CRadar::SetCoordBlip(/int32 CRadar::SetCoordBlip(/'     -e 's/^int CRadar::SetShortRangeCoordBlip(/int32 CRadar::SetShortRangeCoordBlip(/'     -e 's/^int CRadar::SetEntityBlip(/int32 CRadar::SetEntityBlip(/'     "$RADAR"
fi

if [ -f "$CAMERA" ]; then
  sed -i 's/^CCamera::TryToStartNewCamMode(int obbeMode)/CCamera::TryToStartNewCamMode(int32 obbeMode)/' "$CAMERA"
fi

if [ -f "$RE3" ]; then
  sed -i '/^void StoreIni(const char \*cat, const char \*key, uint32 val)/i\#ifdef RW_PSP\nvoid StoreIni(const char *cat, const char *key, bool val) { cfg[cat][key] = val ? "1" : "0"; }\n#endif' "$RE3"
fi

if [ -f "$DEBUGMENU" ]; then
  if ! grep -q 'const char \*path, const char \*name, int \*ptr' "$DEBUGMENU"; then
    sed -i '/^inline DebugMenuEntry \*DebugMenuAddVar(const char \*path, const char \*name, int32_t \*ptr/i\#ifdef RW_PSP\ninline DebugMenuEntry *DebugMenuAddVar(const char *path, const char *name, int *ptr, TriggerFunc triggerFunc, int step, int lowerBound, int upperBound, const char **strings)\n{ return DebugMenuAddInt32(path, name, reinterpret_cast<int32_t *>(ptr), triggerFunc, (int32_t)step, (int32_t)lowerBound, (int32_t)upperBound, strings); }\n#endif' "$DEBUGMENU"
  fi
fi

if [ -f "$DEBUGMENUCPP" ]; then
  DMTMP="$DEBUGMENUCPP.psp.tmp"
  {
    printf '%s\n' '#include <string.h>' '#include <stdlib.h>' '#ifdef RW_PSP' 'static char *psp_strdup_local(const char *s) { size_t n = strlen(s) + 1; char *p = (char *)malloc(n); if (p) memcpy(p, s, n); return p; }' '#define strdup psp_strdup_local' '#endif'
    cat "$DEBUGMENUCPP"
  } > "$DMTMP"
  mv "$DMTMP" "$DEBUGMENUCPP"
fi

FAKERW="upstream-revc/src/fakerw/fake.cpp"
if [ -f "$FAKERW" ]; then
  sed -i '/rw::d3d::/d; /rw::xbox::registerVertexFormatPlugin()/d' "$FAKERW"
fi

if [ -f "$FAKERW" ]; then
  sed -i '/rw::d3d::isP8supported = false;/s/^/#ifndef RW_PSP/' "$FAKERW"
  sed -i '/rw::d3d::isP8supported = false;/a #endif' "$FAKERW"
  sed -i '/rw::xbox::registerVertexFormatPlugin();/s/^/#ifndef RW_PSP/' "$FAKERW"
  sed -i '/rw::xbox::registerVertexFormatPlugin();/a #endif' "$FAKERW"
fi

# Font declaration/definition consistency for PSP.
FONTCPP="upstream-revc/src/renderer/Font.cpp"
if [ -f "$FONTCPP" ]; then
  sed -i 's/^int CFont::ButtonsSlot = -1;/int32 CFont::ButtonsSlot = -1;/' "$FONTCPP"
  grep -Fq 'int32 CFont::ButtonsSlot = -1;' "$FONTCPP"
fi
WATERCREATURESCPP="upstream-revc/src/renderer/WaterCreatures.cpp"
if [ -f "$WATERCREATURESCPP" ]; then
  sed -i "s/^int CWaterCreatures::nNumActiveSeaLifeForms;/int32 CWaterCreatures::nNumActiveSeaLifeForms;/" "$WATERCREATURESCPP"
  grep -Fq "int32 CWaterCreatures::nNumActiveSeaLifeForms;" "$WATERCREATURESCPP"
fi
FLUFFH="upstream-revc/src/renderer/Fluff.h"
if [ -f "$FLUFFH" ]; then
  sed -i 's/static int TonightsEvent;/static int32 TonightsEvent;/' "$FLUFFH"
  grep -Fq 'static int32 TonightsEvent;' "$FLUFFH"
fi

WATERCREATURESCPP="upstream-revc/src/renderer/WaterCreatures.cpp"
if [ -f "$WATERCREATURESCPP" ]; then
  sed -i 's/^int CWaterCreatures::nNumActiveSeaLifeForms;/int32 CWaterCreatures::nNumActiveSeaLifeForms;/' "$WATERCREATURESCPP"
  grep -Fq 'int32 CWaterCreatures::nNumActiveSeaLifeForms;' "$WATERCREATURESCPP"
fi

CROSSPLATFORM="upstream-revc/src/skel/crossplatform.cpp"
if [ -f "$CROSSPLATFORM" ]; then
  sed -i '1a #ifdef RW_PSP\n#include <alloca.h>\n#include <stdlib.h>\n#include <string.h>\n#include <strings.h>\n#include <unistd.h>\n#define alloca __builtin_alloca\nstatic char *psp_strsep_local(char **sp, const char *delim) { if (!sp || !*sp) return 0; char *s=*sp; char *p=s; while (*p && !strchr(delim,*p)) ++p; if (*p) { *p=0; *sp=p+1; } else { *sp=0; } return s; }\n#endif' "$CROSSPLATFORM"
  sed -i 's@realpath(relativepath, path);@#ifdef RW_PSP\n\t\t\tstrncpy(path, relativepath, sizeof(path)-1);\n\t\t\tpath[sizeof(path)-1] = 0;\n#else\n\t\t\trealpath(relativepath, path);\n#endif@' "$CROSSPLATFORM"
  sed -i 's/strsep(&p, "\/\\\\")/psp_strsep_local(\&p, "\/\\\\")/' "$CROSSPLATFORM"
  grep -q '#include <alloca.h>' "$CROSSPLATFORM"
  grep -q 'psp_strsep_local' "$CROSSPLATFORM"
fi

# Link PSP SDK graphics/input libraries into the native executable target.
if [ -f "$SRC_CMAKE" ]; then
  if ! grep -q 'pspgu' "$SRC_CMAKE"; then
    sed -i '/^target_link_libraries(\${EXECUTABLE} PRIVATE/i if(RW_PSP)\ntarget_link_libraries(\${EXECUTABLE} PRIVATE pspgu pspgum pspctrl pspge pspdisplay)\nendif()' "$SRC_CMAKE"
  fi
  grep -q 'pspgu' "$SRC_CMAKE"
  grep -q 'pspgum' "$SRC_CMAKE"
  grep -q 'pspctrl' "$SRC_CMAKE"
  grep -q 'pspge' "$SRC_CMAKE"
  grep -q 'pspdisplay' "$SRC_CMAKE"
fi

# PSP screen-droplet fallback: keep the effect disabled without pulling D3D/GL backends.
SD="upstream-revc/src/extras/screendroplets.cpp"
if [ -f "$SD" ] && ! grep -q 'PSP screen-droplet fallback' "$SD"; then
  cat >> "$SD" <<'EOF'
#ifdef RW_PSP
// PSP screen-droplet fallback
static void openim2d_uv2(void) {}
static void closeim2d_uv2(void) {}
static void RenderIndexedPrimitive_UV2(RwPrimitiveType, Im2DVertexUV2 *, RwInt32, RwImVertexIndex *, RwInt32) {}
#endif
EOF
  grep -q 'PSP screen-droplet fallback' "$SD"
fi
