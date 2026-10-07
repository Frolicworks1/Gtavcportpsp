#include "../rwbase.h"
#include "../rwerror.h"
#include "../rwplg.h"
#include "../rwpipeline.h"
#include "../rwobjects.h"
#include "../rwengine.h"
#include "rwpsp.h"
#include <pspgu.h>
#include <pspgum.h>

namespace rw { namespace psp {

static void beginUpdate(Camera*) {
    static bool initialized = false;
    if(initialized) return;
    sceGuInit();
    sceGuStart(GU_DIRECT, (void*)0);
    sceGuDrawBuffer(GU_PSM_8888, (void*)0, 512);
    sceGuDispBuffer(480, 272, (void*)0x88000, 512);
    sceGuDepthBuffer((void*)0x110000, 512);
    sceGuOffset(2048 - 240, 2048 - 136);
    sceGuViewport(2048, 2048, 480, 272);
    sceGuDepthRange(65535, 0);
    sceGuScissor(0, 0, 480, 272);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuEnable(GU_DEPTH_TEST);
    sceGuDepthFunc(GU_GEQUAL);
    sceGuFinish();
    sceGuSync(0, 0);
    sceGuDisplay(GU_TRUE);
    initialized = true;
}
static void endUpdate(Camera*) { }
static void clearCamera(Camera*, RGBA*, uint32) { }
static void showRaster(Raster*, uint32) { }
static void setRenderState(int32, void*) { }
static void *getRenderState(int32) { return 0; }
static bool32 rasterRenderFast(Raster*, int32, int32) { return 0; }

static void im2DRenderLine(void*, int32, int32, int32) { }
static void im2DRenderTriangle(void*, int32, int32, int32, int32) { }
static void im2DRenderPrimitive(PrimitiveType, void*, int32) { }
static void im2DRenderIndexedPrimitive(PrimitiveType, void*, int32, void*, int32) { }

static void im3DTransform(void*, int32, Matrix*, uint32) { }
static void im3DRenderPrimitive(PrimitiveType) { }
static void im3DRenderIndexedPrimitive(PrimitiveType, void*, int32) { }
static void im3DEnd(void) { }

static int deviceSystem(DeviceReq req, void *arg, int32 n)
{
    switch(req) {
    case DEVICEGETNUMSUBSYSTEMS: return 1;
    case DEVICEGETCURRENTSUBSYSTEM: return 0;
    case DEVICEGETSUBSSYSTEMINFO:
        if(arg) {
            SubSystemInfo *info = (SubSystemInfo*)arg;
            info->name[0]='P'; info->name[1]='S'; info->name[2]='P';
            info->name[3]='\0';
            return 1;
        }
        return 0;
    case DEVICEGETNUMVIDEOMODES: return 1;
    case DEVICEGETCURRENTVIDEOMODE: return 0;
    case DEVICEGETVIDEOMODEINFO:
        if(arg) {
            VideoMode *mode=(VideoMode*)arg;
            mode->width=480; mode->height=272; mode->depth=32; mode->flags=0;
            return 1;
        }
        return 0;
    default: return 1;
    }
}

Device renderdevice = {
    0.1f, 1000.0f,
    beginUpdate, endUpdate, clearCamera, showRaster, rasterRenderFast,
    setRenderState, getRenderState,
    im2DRenderLine, im2DRenderTriangle, im2DRenderPrimitive, im2DRenderIndexedPrimitive,
    im3DTransform, im3DRenderPrimitive, im3DRenderIndexedPrimitive, im3DEnd,
    deviceSystem
};

void registerPlatformPlugins(void) { }

} }