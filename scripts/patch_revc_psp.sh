#!/bin/sh
set -eu

RW="upstream-revc/vendor/librw/rw.h"
BASE="upstream-revc/vendor/librw/src/rwbase.h"
ENG="upstream-revc/vendor/librw/src/engine.cpp"
CMAKE="upstream-revc/vendor/librw/src/CMakeLists.txt"

# Remove desktop-only platform headers and keep the PSP backend as the
# concrete device interface for this probe.
awk '!/^[[:space:]]*#include "src\\/(ps2\\/(rwps2\\.h|rwps2plg\\.h)|d3d\\/(rwxbox|rwd3d|rwd3d8|rwd3d9)\\.h|gl\\/(rwwdgl|rwgl3|rwgl3shader|rwgl3plg)\\.h)"/ { print }' "$RW" > "$RW.tmp"
mv "$RW.tmp" "$RW"

# rw.h must expose the PSP device namespace to every game-side include,
# not just to librw's own compilation unit.
if ! grep -q '#include "src/psp/rwpsp.h"' "$RW"; then
  awk 'BEGIN { added=0 } { print; if (!added && $0 == "#include \"src/rwobjects.h\"") { print "#include \"src/psp/rwpsp.h\""; added=1 } }' "$RW" > "$RW.tmp"
  mv "$RW.tmp" "$RW"
fi

if ! grep -q '#define RWDEVICE psp' "$BASE"; then
  awk 'BEGIN { added=0 } { if (!added && $0 == "#ifdef RW_GL3") { print "#ifdef RW_PSP"; print "#define RWDEVICE psp"; print "#endif"; added=1 } print }' "$BASE" > "$BASE.tmp"
  mv "$BASE.tmp" "$BASE"
fi

# Restore librw engine dependency order after removing desktop device headers.
awk '!/^[[:space:]]*#include "rwengine\\.h"/ && !/^[[:space:]]*#include "(d3d\\/(rwxbox|rwd3d|rwd3d8|rwd3d9)|gl\\/(rwgl3|rwwdgl))\\.h"/ { print }' "$ENG" > "$ENG.tmp"
mv "$ENG.tmp" "$ENG"
awk 'BEGIN { print "#include \"rwbase.h\""; print "#include \"rwerror.h\""; print "#include \"rwplg.h\""; print "#include \"rwpipeline.h\""; print "#include \"rwobjects.h\""; print "#include \"rwengine.h\"" } { print }' "$ENG" > "$ENG.tmp"
mv "$ENG.tmp" "$ENG"
if ! grep -q '#include "psp/rwpsp.h"' "$ENG"; then
  awk 'BEGIN { added=0 } { print; if (!added && $0 == "#include \"rwengine.h\"") { print "#include \"psp/rwpsp.h\""; added=1 } }' "$ENG" > "$ENG.tmp"
  mv "$ENG.tmp" "$ENG"
fi

# Keep PSP plugin/device registration only.
awk '{
  gsub(/ps2::registerPlatformPlugins\(\)/, "psp::registerPlatformPlugins()");
  gsub(/ps2::renderdevice/, "psp::renderdevice");
  if ($0 ~ /xbox::registerPlatformPlugins\(\)|d3d8::registerPlatformPlugins\(\)|d3d9::registerPlatformPlugins\(\)|wdgl::registerPlatformPlugins\(\)|gl3::registerPlatformPlugins\(\)/) next;
  if ($0 ~ /xbox::renderdevice|d3d8::renderdevice|d3d9::renderdevice|wdgl::renderdevice|gl3::renderdevice/) next;
  if ($0 ~ /d3d::nativeRasterOffset = 0;/) next;
  print;
}' "$ENG" > "$ENG.tmp"
mv "$ENG.tmp" "$ENG"

if ! grep -q 'psp/rwpsp.cpp' "$CMAKE"; then
  awk '
    /ps2\/rwps2plg\\.h/ && !done {
      print
      print "    psp/rwpsp.cpp"
      print "    psp/rwpsp.h"
      done=1
      next
    }
    { print }
  ' "$CMAKE" > "$CMAKE.tmp"
  mv "$CMAKE.tmp" "$CMAKE"
fi

if grep -q '^add_library(librw' "$CMAKE" && ! grep -q 'pspgu pspgum pspge pspdisplay' "$CMAKE"; then
  cat >> "$CMAKE" <<'EOF'

if(RW_PSP)
  target_link_libraries(librw PRIVATE pspgu pspgum pspge pspdisplay)
endif()
EOF
fi

# The pinned reVC CMake project only exposes OpenAL as its non-Windows
# audio backend. PSP has no OpenAL implementation in this port yet. For this
# compile probe, remove the desktop OAL/MSS configuration block and its OAL
# source files; a real PSP audio backend will be added separately.
SRC_CMAKE="upstream-revc/src/CMakeLists.txt"
if [ -f "$SRC_CMAKE" ]; then
  awk '
    /^if\(\$\{PROJECT\}_AUDIO STREQUAL "OAL"\)/ { skip=1; depth=1; next }
    skip && /^if\(/ { depth++; next }
    skip && /^endif\(\)/ { depth--; if(depth==0) skip=0; next }
    !skip { print }
  ' "$SRC_CMAKE" > "$SRC_CMAKE.tmp"
  mv "$SRC_CMAKE.tmp" "$SRC_CMAKE"
  # The source list is created near the top of this file, so filter OAL
  # sources immediately after the GLOB. Filtering after add_executable()
  # would be too late to affect the target.
  awk 'BEGIN { added=0 } { print; if (!added && index($0, "file(GLOB_RECURSE") != 0) { print "list(FILTER ${PROJECT}_SOURCES EXCLUDE REGEX \\\"/audio/oal/|/audio/sampman_oal.cpp$\\\")"; added=1 } }' "$SRC_CMAKE" > "$SRC_CMAKE.tmp"
  mv "$SRC_CMAKE.tmp" "$SRC_CMAKE"
fi

# EAX is a desktop DirectSound/OpenAL compatibility layer and is not
# part of the PSP audio path. Exclude its sources from the PSP compile probe.
awk '!/audio\\/eax\\//' "$SRC_CMAKE" > "$SRC_CMAKE.tmp"
mv "$SRC_CMAKE.tmp" "$SRC_CMAKE"
# Select the PSP device in Engine::open.
if ! grep -q 'engine->device = psp::renderdevice' "$ENG"; then
  awk 'BEGIN { added=0 } { if (!added && $0 == "#ifdef RW_PS2") { print "#ifdef RW_PSP"; print "\tengine->device = psp::renderdevice;"; print "#elif defined(RW_PS2)"; added=1 } else print }' "$ENG" > "$ENG.tmp"
  mv "$ENG.tmp" "$ENG"
fi

# Ensure the game-side fakerw wrapper sees the same concrete device.
# rw.h now includes rwpsp.h, so RWDEVICE psp resolves to rw::psp.
FAKERW="upstream-revc/src/fakerw/rwcore.h"
if [ -f "$FAKERW" ]; then
  awk '
  /^#ifdef RW_PSP$/ { skip=1; next }
  skip && /^#endif$/ { skip=0; next }
  !skip { print }
' "$FAKERW" > "$FAKERW.tmp"
mv "$FAKERW.tmp" "$FAKERW"
awk 'BEGIN { print "#ifdef RW_PSP"; print "#define RWDEVICE psp"; print "#endif" } { print }' "$FAKERW" > "$FAKERW.tmp"
  mv "$FAKERW.tmp" "$FAKERW"
fi

# Use the portable JoyState shape on PSP; actual sceCtrlReadBufferPositive
# population will be implemented in the input backend rather than discarded.
CTRL="upstream-revc/src/core/ControllerConfig.h"
CTRLC="upstream-revc/src/core/ControllerConfig.cpp"
if [ -f "$CTRL" ]; then
  awk '{ gsub(/#ifdef RW_GL3/, "#if defined(RW_GL3) || defined(RW_PSP)"); gsub(/#if defined RW_GL3/, "#if defined(RW_GL3) || defined(RW_PSP)"); print }' "$CTRL" > "$CTRL.tmp"
  mv "$CTRL.tmp" "$CTRL"
fi
if [ -f "$CTRLC" ]; then
  awk '{ gsub(/#elif defined RW_GL3/, "#elif defined RW_GL3 || defined(RW_PSP)"); print }' "$CTRLC" > "$CTRLC.tmp"
  mv "$CTRLC.tmp" "$CTRLC"
fi
