#!/bin/sh
set -eu

RW="upstream-revc/vendor/librw/rw.h"
BASE="upstream-revc/vendor/librw/src/rwbase.h"
ENG="upstream-revc/vendor/librw/src/engine.cpp"
CMAKE="upstream-revc/vendor/librw/src/CMakeLists.txt"
CTRL="upstream-revc/src/core/ControllerConfig.h"

# PSP controller path: reuse the GL3-style JoyState representation and disable
# the Windows DirectInput layout/size validation.
if [ -f "$CTRL" ]; then
  sed -i 's#^#if defined RW_GL3$#\#if defined RW_GL3 || defined RW_PSP#' "$CTRL"
  sed -i 's#^#ifdef RW_GL3$#\#if defined RW_GL3 || defined RW_PSP#' "$CTRL"
  sed -i 's#^#ifndef RW_GL3$#\#if !defined RW_GL3 \\&\\& !defined RW_PSP#' "$CTRL"

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

exit 0
