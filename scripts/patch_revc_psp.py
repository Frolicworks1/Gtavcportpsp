from pathlib import Path

ROOT = Path("upstream-revc/vendor/librw")
rw = ROOT / "rw.h"
base = ROOT / "src/rwbase.h"
eng = ROOT / "src/engine.cpp"
cmake = ROOT / "src/CMakeLists.txt"

def replace_once(path, old, new):
    s = path.read_text()
    if old not in s:
        raise SystemExit(f"patch target not found: {path}: {old!r}")
    path.write_text(s.replace(old, new, 1))

s = rw.read_text()
for inc in [
    '#include "src/ps2/rwps2.h"\n',
    '#include "src/ps2/rwps2plg.h"\n',
    '#include "src/d3d/rwxbox.h"\n',
    '#include "src/d3d/rwd3d.h"\n',
    '#include "src/d3d/rwd3d8.h"\n',
    '#include "src/d3d/rwd3d9.h"\n',
    '#include "src/gl/rwwdgl.h"\n',
    '#include "src/gl/rwgl3.h"\n',
    '#include "src/gl/rwgl3shader.h"\n',
    '#include "src/gl/rwgl3plg.h"\n',
]:
    s = s.replace(inc, "")
if '#include "src/psp/rwpsp.h"\n' not in s:
    s += '#include "src/psp/rwpsp.h"\n'
rw.write_text(s)

s = base.read_text()
if "#ifdef RW_PSP\n#define RWDEVICE psp\n" not in s:
    marker = "#ifdef RW_GL3\n"
    if marker not in s:
        raise SystemExit("rwbase.h: RW_GL3 marker not found")
    s = s.replace(marker, "#ifdef RW_PSP\n#define RWDEVICE psp\n#elif defined(RW_GL3)\n", 1)
base.write_text(s)

s = eng.read_text()
for inc in [
    '#include "ps2/rwps2.h"\n',
    '#include "d3d/rwxbox.h"\n',
    '#include "d3d/rwd3d.h"\n',
    '#include "d3d/rwd3d8.h"\n',
    '#include "d3d/rwd3d9.h"\n',
    '#include "gl/rwgl3.h"\n',
    '#include "gl/rwwdgl.h"\n',
]:
    s = s.replace(inc, "")
if '#include "psp/rwpsp.h"\n' not in s:
    s = s.replace('#include "rwengine.h"\n', '#include "rwengine.h"\n#include "psp/rwpsp.h"\n', 1)
s = s.replace('ps2::registerPlatformPlugins();', 'psp::registerPlatformPlugins();')
s = s.replace('engine->device = ps2::renderdevice;', 'engine->device = psp::renderdevice;')
s = s.replace('d3d::nativeRasterOffset = 0;', '')
eng.write_text(s)

s = cmake.read_text()
if 'psp/rwpsp.cpp' not in s:
    marker = '    ps2/rwps2plg.h\n)'
    if marker not in s:
        raise SystemExit("librw CMake source-list marker not found")
    s = s.replace(marker, '    ps2/rwps2plg.h\n\n    psp/rwpsp.cpp\n    psp/rwpsp.h\n)', 1)
if 'if(RW_PSP)' not in s:
    s += '\nif(RW_PSP)\n  target_link_libraries(librw PRIVATE pspgu pspgum pspge pspdisplay)\nendif()\n'
cmake.write_text(s)
