#!/bin/sh
set -eu

RW="upstream-revc/vendor/librw/rw.h"
BASE="upstream-revc/vendor/librw/src/rwbase.h"
ENG="upstream-revc/vendor/librw/src/engine.cpp"
CMAKE="upstream-revc/vendor/librw/src/CMakeLists.txt"

sed -i   -e '/#include "src\/ps2\/rwps2.h"/d'   -e '/#include "src\/ps2\/rwps2plg.h"/d'   -e '/#include "src\/d3d\/rwxbox.h"/d'   -e '/#include "src\/d3d\/rwd3d.h"/d'   -e '/#include "src\/d3d\/rwd3d8.h"/d'   -e '/#include "src\/d3d\/rwd3d9.h"/d'   -e '/#include "src\/gl\/rwwdgl.h"/d'   -e '/#include "src\/gl\/rwgl3.h"/d'   -e '/#include "src\/gl\/rwgl3shader.h"/d'   -e '/#include "src\/gl\/rwgl3plg.h"/d'   "$RW"

grep -q '#include "src/psp/rwpsp.h"' "$RW" || printf '\n#include "src/psp/rwpsp.h"\n' >> "$RW"

if ! grep -q '#define RWDEVICE psp' "$BASE"; then
  sed -i '/#ifdef RW_GL3/i\
#ifdef RW_PSP\
#define RWDEVICE psp\
#endif' "$BASE"
fi

sed -i   -e '/#include "rwengine.h"/d'   -e '/#include "d3d\/rwxbox.h"/d'   -e '/#include "d3d\/rwd3d.h"/d'   -e '/#include "d3d\/rwd3d8.h"/d'   -e '/#include "d3d\/rwd3d9.h"/d'   -e '/#include "gl\/rwgl3.h"/d'   -e '/#include "gl\/rwwdgl.h"/d'   "$ENG"

grep -q '#include "psp/rwpsp.h"' "$ENG" || sed -i '/#include "rwengine.h"/a\
#include "psp/rwpsp.h"' "$ENG"

# Select the PSP device in Engine::open and keep only the PSP platform plugin registration.\nsed -i \
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
    /ps2\/rwps2plg\.h/ && !done {
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


# Make Engine::open select the PSP device instead of falling back to null.
if ! grep -q 'RW_PSP.*psp::renderdevice' "$ENG"; then
  sed -i '/#ifdef RW_PS2/i\#ifdef RW_PSP\
\tengine->device = psp::renderdevice;\
#elif defined(RW_PS2)' "$ENG"
fi
