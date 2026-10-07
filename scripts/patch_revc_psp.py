from pathlib import Path

rw=Path("upstream-revc/vendor/librw/rw.h")
s=rw.read_text()
for x in [
    '#include "src/ps2/rwps2.h"\n','#include "src/ps2/rwps2plg.h"\n',
    '#include "src/d3d/rwxbox.h"\n','#include "src/d3d/rwd3d.h"\n',
    '#include "src/d3d/rwd3d8.h"\n','#include "src/d3d/rwd3d9.h"\n',
    '#include "src/gl/rwwdgl.h"\n','#include "src/gl/rwgl3.h"\n',
    '#include "src/gl/rwgl3shader.h"\n','#include "src/gl/rwgl3plg.h"\n']:
    s=s.replace(x,"")
s += '\n#include "src/psp/rwpsp.h"\n'
rw.write_text(s)

p=Path("upstream-revc/vendor/librw/src/rwbase.h")
s=p.read_text().replace("#ifdef RW_GL3\n","#ifdef RW_PSP\n#define RWDEVICE psp\n#elif defined(RW_GL3)\n",1)
p.write_text(s)

p=Path("upstream-revc/vendor/librw/src/engine.cpp")
s=p.read_text()
for x in [
    '#include "ps2/rwps2.h"\n','#include "d3d/rwxbox.h"\n','#include "d3d/rwd3d.h"\n',
    '#include "d3d/rwd3d8.h"\n','#include "d3d/rwd3d9.h"\n',
    '#include "gl/rwgl3.h"\n','#include "gl/rwwdgl.h"\n']:
    s=s.replace(x,"")
s=s.replace('#include "rwengine.h"\n','#include "rwengine.h"\n#include "psp/rwpsp.h"\n')
s=s.replace('ps2::registerPlatformPlugins();','psp::registerPlatformPlugins();')
s=s.replace('engine->device = ps2::renderdevice;','engine->device = psp::renderdevice;')
s=s.replace('#ifdef RW_PS2\n\tengine->device = psp::renderdevice;','#ifdef RW_PSP\n\tengine->device = psp::renderdevice;')
s=s.replace('d3d::nativeRasterOffset = 0;','')
p.write_text(s)

p=Path("upstream-revc/vendor/librw/src/CMakeLists.txt")
s=p.read_text().replace("    ps2/rwps2plg.h\n)","    ps2/rwps2plg.h\n\n    psp/rwpsp.cpp\n    psp/rwpsp.h\n)")
s += "\nif(RW_PSP)\n  target_link_libraries(librw PRIVATE pspgu pspgum pspge pspdisplay)\nendif()\n"
p.write_text(s)
