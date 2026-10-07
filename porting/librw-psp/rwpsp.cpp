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
    if(!initialized) {
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
        sceGuFrontFace(GU_CW);
        sceGuShadeModel(GU_SMOOTH);
        sceGuFinish();
        sceGuSync(0, 0);
        sceGuDisplay(GU_TRUE);
        initialized = true;
    }
    // A new direct list is required for every rendered frame.
    sceGuStart(GU_DIRECT, (void*)0);
}
static void endUpdate(Camera*) {
    sceGuFinish();
    sceGuSync(0, 0);
    sceGuSwapBuffers();
}
static void clearCamera(Camera*, RGBA *color, uint32 flags) {
    if(color) {
        uint32 packed = ((uint32)color->red << 24) |
                        ((uint32)color->green << 16) |
                        ((uint32)color->blue << 8) |
                        (uint32)color->alpha;
        sceGuClearColor(packed);
    }
    sceGuClearDepth(0);
    uint32 clear = 0;
    if(flags & 1) clear |= GU_COLOR_BUFFER_BIT;
    if(flags & 2) clear |= GU_DEPTH_BUFFER_BIT;
    if(clear == 0) clear = GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT;
    sceGuClear(clear);
}
static void showRaster(Raster*, uint32) { }
static bool stateVertexAlpha = false;
static bool stateZTest = true;
static bool stateZWrite = true;
static int32 stateCull = CULLBACK;

static void setRenderState(int32 state, void *pvalue) {
    uint32 value = (uint32)(uintptr)pvalue;
    switch(state) {
    case VERTEXALPHA:
        stateVertexAlpha = value != 0;
        if(stateVertexAlpha) {
            sceGuEnable(GU_BLEND);
            sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
        } else {
            sceGuDisable(GU_BLEND);
        }
        break;
    case SRCBLEND:
    case DESTBLEND:
        // Keep the common alpha blend path enabled; exact blend mapping will
        // be completed with the PSP RenderWare state table.
        if(stateVertexAlpha) {
            sceGuEnable(GU_BLEND);
            sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
        }
        break;
    case ZTESTENABLE:
        stateZTest = value != 0;
        if(stateZTest) sceGuEnable(GU_DEPTH_TEST);
        else sceGuDisable(GU_DEPTH_TEST);
        break;
    case ZWRITEENABLE:
        stateZWrite = value != 0;
        if(stateZWrite) sceGuDepthMask(0);
        else sceGuDepthMask(1);
        break;
    case CULLMODE:
        stateCull = (int32)value;
        if(stateCull == CULLNONE) {
            sceGuDisable(GU_CULL_FACE);
        } else {
            sceGuEnable(GU_CULL_FACE);
            sceGuFrontFace(stateCull == CULLFRONT ? GU_CCW : GU_CW);
        }
        break;
    default:
        break;
    }
}

static void *getRenderState(int32 state) {
    switch(state) {
    case VERTEXALPHA: return (void*)(uintptr)stateVertexAlpha;
    case ZTESTENABLE: return (void*)(uintptr)stateZTest;
    case ZWRITEENABLE: return (void*)(uintptr)stateZWrite;
    case CULLMODE: return (void*)(uintptr)stateCull;
    default: return 0;
    }
}
static bool32 rasterRenderFast(Raster*, int32, int32) { return 0; }

struct GU2DVertex {
    float u, v;
    uint32 color;
    float x, y, z;
};

static int guPrimitive(PrimitiveType prim) {
    switch(prim) {
    case PRIMTYPELINELIST: return GU_LINES;
    case PRIMTYPEPOLYLINE: return GU_LINE_STRIP;
    case PRIMTYPETRILIST: return GU_TRIANGLES;
    case PRIMTYPETRISTRIP: return GU_TRIANGLE_STRIP;
    case PRIMTYPETRIFAN: return GU_TRIANGLE_FAN;
    case PRIMTYPEPOINTLIST: return GU_POINTS;
    default: return -1;
    }
}

static uint32 packColor(const Im2DVertex& v) {
    return ((uint32)v.r << 24) | ((uint32)v.g << 16) |
           ((uint32)v.b << 8) | (uint32)v.a;
}

static void draw2D(PrimitiveType prim, const Im2DVertex *src, int32 count) {
    if(!src || count <= 0) return;
    int p = guPrimitive(prim);
    if(p < 0) return;

    GU2DVertex *dst = (GU2DVertex*)sceGuGetMemory(sizeof(GU2DVertex) * count);
    for(int32 i = 0; i < count; i++) {
        dst[i].u = src[i].u;
        dst[i].v = src[i].v;
        dst[i].color = packColor(src[i]);
        dst[i].x = src[i].x;
        dst[i].y = src[i].y;
        dst[i].z = src[i].z;
    }
    sceGuDrawArray(p, GU_TEXTURE_32BITF | GU_COLOR_8888 |
        GU_VERTEX_32BITF | GU_TRANSFORM_2D, count, 0, dst);
}

static void im2DRenderLine(void *vertices, int32 a, int32 b, int32) {
    const Im2DVertex *v = (const Im2DVertex*)vertices;
    if(v) {
        GU2DVertex *dst = (GU2DVertex*)sceGuGetMemory(sizeof(GU2DVertex) * 2);
        const Im2DVertex *s = &v[a];
        const Im2DVertex *t = &v[b];
        dst[0].u=s->u; dst[0].v=s->v; dst[0].color=packColor(*s);
        dst[0].x=s->x; dst[0].y=s->y; dst[0].z=s->z;
        dst[1].u=t->u; dst[1].v=t->v; dst[1].color=packColor(*t);
        dst[1].x=t->x; dst[1].y=t->y; dst[1].z=t->z;
        sceGuDrawArray(GU_LINES, GU_TEXTURE_32BITF | GU_COLOR_8888 |
            GU_VERTEX_32BITF | GU_TRANSFORM_2D, 2, 0, dst);
    }
}
static void im2DRenderTriangle(void *vertices, int32 a, int32 b, int32 cidx, int32 d) {
    const Im2DVertex *v = (const Im2DVertex*)vertices;
    if(!v) return;
    GU2DVertex *dst = (GU2DVertex*)sceGuGetMemory(sizeof(GU2DVertex) * 3);
    const Im2DVertex *s[3] = { &v[a], &v[b], &v[cidx] };
    for(int i=0;i<3;i++) {
        dst[i].u=s[i]->u; dst[i].v=s[i]->v; dst[i].color=packColor(*s[i]);
        dst[i].x=s[i]->x; dst[i].y=s[i]->y; dst[i].z=s[i]->z;
    }
    sceGuDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_COLOR_8888 |
        GU_VERTEX_32BITF | GU_TRANSFORM_2D, 3, 0, dst);
}
static void im2DRenderPrimitive(PrimitiveType prim, void *vertices, int32 count) {
    draw2D(prim, (const Im2DVertex*)vertices, count);
}
static void im2DRenderIndexedPrimitive(PrimitiveType prim, void *vertices, int32 count, void *indices, int32 indexCount) {
    const Im2DVertex *src=(const Im2DVertex*)vertices;
    const uint16 *idx=(const uint16*)indices;
    if(!src || !idx || indexCount <= 0) return;
    int p=guPrimitive(prim);
    if(p < 0) return;
    GU2DVertex *dst=(GU2DVertex*)sceGuGetMemory(sizeof(GU2DVertex) * indexCount);
    for(int32 i=0;i<indexCount;i++) {
        const Im2DVertex &s=src[idx[i]];
        dst[i].u=s.u; dst[i].v=s.v; dst[i].color=packColor(s);
        dst[i].x=s.x; dst[i].y=s.y; dst[i].z=s.z;
    }
    sceGuDrawArray(p, GU_TEXTURE_32BITF | GU_COLOR_8888 |
        GU_VERTEX_32BITF | GU_TRANSFORM_2D, indexCount, 0, dst);
}

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