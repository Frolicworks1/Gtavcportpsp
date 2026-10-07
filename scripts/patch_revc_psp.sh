#!/bin/sh
set -eu

RW="upstream-revc/vendor/librw/rw.h"
BASE="upstream-revc/vendor/librw/src/rwbase.h"
ENG="upstream-revc/vendor/librw/src/engine.cpp"
CMAKE="upstream-revc/vendor/librw/src/CMakeLists.txt"

# Remove desktop platform headers from the umbrella header.
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

grep -q '#include "src/psp/rwpsp.h"' "$RW" || printf '\n#include "src/psp/rwpsp.h"\n' >> "$RW"

# Give fakerw/reVC a concrete PSP RWDEVICE.
grep -q '#define RWDEVICE psp' "$BASE" ||   sed -i '/#ifdef RW_GL3/i #ifdef RW_PSP\n#define RWDEVICE psp\n#elif' "$BASE"

# Replace desktop device includes/selection with PSP.
sed -i \
  -e '/#include "ps2\/rwps2.h"/d' \
  -e '/#include "d3d\/rwxbox.h"/d' \
  -e '/#include "d3d\/rwd3d.h"/d' \
  -e '/#include "d3d\/rwd3d8.h"/d' \
  -e '/#include "d3d\/rwd3d9.h"/d' \
  -e '/#include "gl\/rwgl3.h"/d' \
  -e '/#include "gl\/rwwdgl.h"/d' \
  "$ENG"

grep -q '#include "psp/rwpsp.h"' "$ENG" ||   sed -i '/#include "rwengine.h"/a #include "psp/rwpsp.h"' "$ENG"

sed -i 's/ps2::registerPlatformPlugins()/psp::registerPlatformPlugins()/g' "$ENG"
sed -i 's/ps2::renderdevice/psp::renderdevice/g' "$ENG"
sed -i '/d3d::nativeRasterOffset = 0;/d' "$ENG"

# Add PSP sources to librw's source list if not already present.
grep -q 'psp/rwpsp.cpp' "$CMAKE" || sed -i '/ps2\/rwps2plg.h/a\\
    psp/rwpsp.cpp\\
    psp/rwpsp.h' "$CMAKE"

grep -q 'pspgu pspgum pspge pspdisplay' "$CMAKE" || cat >> "$CMAKE" <<'EOF'

if(RW_PSP)
  target_link_libraries(librw PRIVATE pspgu pspgum pspge pspdisplay)
endif()
EOF
