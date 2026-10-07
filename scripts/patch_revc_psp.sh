#!/bin/sh
set -eu

RW="upstream-revc/vendor/librw/rw.h"
BASE="upstream-revc/vendor/librw/src/rwbase.h"
ENG="upstream-revc/vendor/librw/src/engine.cpp"
CMAKE="upstream-revc/vendor/librw/src/CMakeLists.txt"

# Remove desktop-only platform headers and keep the PSP backend as the
# concrete device interface for this probe.
sed -i \
  -e '/#include "src\/ps2\/rwps2.h"/d' \
  -e '/#include "src\/ps2\/rwps2plg.h"/d' \
  -e '/#include "src\/d3d\/rwxbox.h"/d' \
  -e '/#include "src\/d3d\/rwd3d.h"/d' \
  -e '/#include "src\/d3d\/rwd3d8.h"/d' \
  -e '/#include "src\/d3d\/rwd3d9.h"/d' \
  -e '/#include "src\/gl\/rwwdgl.h"/d' \
  -e '/#include "src\/gl\/rwgl3.h"/d' \
  -e '/#include "src\/gl\/rwgl3shader.h"/d' \
  -e '/#include "src\/gl\/rwgl3plg.h"/d' \
  "$RW"

# rw.h must expose the PSP device namespace to every game-side include,
# not just to librw's own compilation unit.
sed -i '/#include "src\/rwobjects.h"/a#include "src/psp/rwpsp.h"' "$RW"

if ! grep -q '#define RWDEVICE psp' "$BASE"; then
  sed -i '/#ifdef RW_GL3/i\\
#ifdef RW_PSP\\
#define RWDEVICE psp\\
#endif' "$BASE"
fi

# Restore librw engine dependency order after removing desktop device headers.
sed -i \
  -e '/#include "rwengine.h"/d' \
  -e '/#include "d3d\/rwxbox.h"/d' \
  -e '/#include "d3d\/rwd3d.h"/d' \
  -e '/#include "d3d\/rwd3d8.h"/d' \
  -e '/#include "d3d\/rwd3d9.h"/d' \
  -e '/#include "gl\/rwgl3.h"/d' \
  -e '/#include "gl\/rwwdgl.h"/d' \
  "$ENG"
sed -i '/#include "rwbase.h"/d; /#include "rwerror.h"/d; /#include "rwplg.h"/d; /#include "rwpipeline.h"/d; /#include "rwobjects.h"/d; /#include "rwengine.h"/d' "$ENG"
sed -i '1i#include "rwbase.h"\n#include "rwerror.h"\n#include "rwplg.h"\n#include "rwpipeline.h"\n#include "rwobjects.h"\n#include "rwengine.h"' "$ENG"
grep -q '#include "psp/rwpsp.h"' "$ENG" || sed -i '/#include "rwengine.h"/a#include "psp/rwpsp.h"' "$ENG"

# Keep PSP plugin/device registration only.
sed -i \
  -e 's/ps2::registerPlatformPlugins()/psp::registerPlatformPlugins()/g' \
  -e '/xbox::registerPlatformPlugins()/d' \
  -e '/d3d8::registerPlatformPlugins()/d' \
  -e '/d3d9::registerPlatformPlugins()/d' \
  -e '/wdgl::registerPlatformPlugins()/d' \
  -e '/gl3::registerPlatformPlugins()/d' \
  -e 's/ps2::renderdevice/psp::renderdevice/g' \
  -e '/xbox::renderdevice/d' \
  -e '/d3d8::renderdevice/d' \
  -e '/d3d9::renderdevice/d' \
  -e '/wdgl::renderdevice/d' \
  -e '/gl3::renderdevice/d' \
  -e '/d3d::nativeRasterOffset = 0;/d' \
  "$ENG"

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
  sed -i '/file(GLOB_RECURSE ${PROJECT}_SOURCES/a\
list(FILTER ${PROJECT}_SOURCES EXCLUDE REGEX "/audio/oal/|/audio/sampman_oal\\\\.cpp$")' "$SRC_CMAKE"
fi

# EAX is a desktop DirectSound/OpenAL compatibility layer and is not
# part of the PSP audio path. Exclude its sources from the PSP compile probe.
sed -i '/audio\\/eax\\//d' "$SRC_CMAKE"
# Select the PSP device in Engine::open.
if ! grep -q 'engine->device = psp::renderdevice' "$ENG"; then
  sed -i '/#ifdef RW_PS2/i\\
#ifdef RW_PSP\\
\\tengine->device = psp::renderdevice;\\
#elif defined(RW_PS2)' "$ENG"
fi

# Ensure the game-side fakerw wrapper sees the same concrete device.
# rw.h now includes rwpsp.h, so RWDEVICE psp resolves to rw::psp.
FAKERW="upstream-revc/src/fakerw/rwcore.h"
if [ -f "$FAKERW" ]; then
  sed -i '/^#ifdef RW_PSP$/,/^#endif$/d' "$FAKERW"
  sed -i '1i#ifdef RW_PSP\n#define RWDEVICE psp\n#endif' "$FAKERW"
fi

# Use the portable JoyState shape on PSP; actual sceCtrlReadBufferPositive
# population will be implemented in the input backend rather than discarded.
CTRL="upstream-revc/src/core/ControllerConfig.h"
CTRLC="upstream-revc/src/core/ControllerConfig.cpp"
if [ -f "$CTRL" ]; then
  sed -i 's/#ifdef RW_GL3/#if defined(RW_GL3) || defined(RW_PSP)/g; s/#if defined RW_GL3/#if defined(RW_GL3) || defined(RW_PSP)/g' "$CTRL"
  sed -i 's/#if defined RW_GL3/#if defined(RW_GL3) || defined(RW_PSP)/g' "$CTRL"
fi
if [ -f "$CTRLC" ]; then
  sed -i 's/#elif defined RW_GL3/#elif defined RW_GL3 || defined(RW_PSP)/g' "$CTRLC"
fi
