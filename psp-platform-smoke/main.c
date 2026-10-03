#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <pspgum.h>

PSP_MODULE_INFO("VC PSP GU Renderer Probe", 0, 0, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(2048);

static unsigned int __attribute__((aligned(16))) list[262144];

typedef struct {
    unsigned int color;
    float x, y, z;
} Vertex;

#define V(c, x, y, z) { (c), (x), (y), (z) }
static const Vertex cube[] __attribute__((aligned(16))) = {
    /* front */
    V(0xFFFF4040,-1,-1,1), V(0xFFFF4040,1,-1,1), V(0xFFFF4040,1,1,1),
    V(0xFFFF4040,-1,-1,1), V(0xFFFF4040,1,1,1), V(0xFFFF4040,-1,1,1),
    /* back */
    V(0xFF40FF40,1,-1,-1), V(0xFF40FF40,-1,-1,-1), V(0xFF40FF40,-1,1,-1),
    V(0xFF40FF40,1,-1,-1), V(0xFF40FF40,-1,1,-1), V(0xFF40FF40,1,1,-1),
    /* left */
    V(0xFF4040FF,-1,-1,-1), V(0xFF4040FF,-1,-1,1), V(0xFF4040FF,-1,1,1),
    V(0xFF4040FF,-1,-1,-1), V(0xFF4040FF,-1,1,1), V(0xFF4040FF,-1,1,-1),
    /* right */
    V(0xFFFFFF40,1,-1,1), V(0xFFFFFF40,1,-1,-1), V(0xFFFFFF40,1,1,-1),
    V(0xFFFFFF40,1,-1,1), V(0xFFFFFF40,1,1,-1), V(0xFFFFFF40,1,1,1),
    /* top */
    V(0xFFFF40FF,-1,1,1), V(0xFFFF40FF,1,1,1), V(0xFFFF40FF,1,1,-1),
    V(0xFFFF40FF,-1,1,1), V(0xFFFF40FF,1,1,-1), V(0xFFFF40FF,-1,1,-1),
    /* bottom */
    V(0xFF40FFFF,-1,-1,-1), V(0xFF40FFFF,1,-1,-1), V(0xFF40FFFF,1,-1,1),
    V(0xFF40FFFF,-1,-1,-1), V(0xFF40FFFF,1,-1,1), V(0xFF40FFFF,-1,-1,1)
};
#undef V

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    sceGuInit();
    sceGuStart(GU_DIRECT, list);
    sceGuDrawBuffer(GU_PSM_8888, (void *)0, 512);
    sceGuDispBuffer(480, 272, (void *)0x88000, 512);
    sceGuDepthBuffer((void *)0x110000, 512);
    sceGuOffset(2048 - (480 / 2), 2048 - (272 / 2));
    sceGuViewport(2048, 2048, 480, 272);
    sceGuScissor(0, 0, 480, 272);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuDepthRange(65535, 0);
    sceGuDepthFunc(GU_GEQUAL);
    sceGuEnable(GU_DEPTH_TEST);
    sceGuShadeModel(GU_SMOOTH);
    sceGuFinish();
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuDisplay(GU_TRUE);

    SceCtrlData pad;
    float angle = 0.0f;
    for (;;) {
        sceCtrlReadBufferPositive(&pad, 1);
        if (pad.Buttons & PSP_CTRL_LEFT) angle -= 0.035f;
        if (pad.Buttons & PSP_CTRL_RIGHT) angle += 0.035f;
        if (!(pad.Buttons & (PSP_CTRL_LEFT | PSP_CTRL_RIGHT))) angle += 0.012f;

        sceGuStart(GU_DIRECT, list);
        sceGuClearColor(0xFF101820);
        sceGuClearDepth(0);
        sceGuClear(GU_COLOR_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

        sceGumMatrixMode(GU_PROJECTION);
        sceGumLoadIdentity();
        sceGumPerspective(60.0f, 480.0f / 272.0f, 0.5f, 100.0f);

        sceGumMatrixMode(GU_VIEW);
        sceGumLoadIdentity();

        sceGumMatrixMode(GU_MODEL);
        sceGumLoadIdentity();
        ScePspFVector3 position = { 0.0f, 0.0f, -4.0f };
        sceGumTranslate(&position);
        ScePspFVector3 rotation = { angle * 0.73f, angle, angle * 0.37f };
        sceGumRotateXYZ(&rotation);

        sceGuDisable(GU_TEXTURE_2D);
        sceGuDisable(GU_CULL_FACE);
        sceGuDrawArray(GU_TRIANGLES,
            GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D,
            sizeof(cube) / sizeof(cube[0]), 0, cube);

        sceGuFinish();
        sceGuSync(0, 0);
        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();
    }

    return 0;
}
