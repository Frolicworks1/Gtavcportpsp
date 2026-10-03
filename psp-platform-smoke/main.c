#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspiofilemgr.h>
#include <stdio.h>
#include <string.h>

PSP_MODULE_INFO("VC PSP GU Texture Probe", 0, 0, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(2048);

static unsigned int __attribute__((aligned(16))) list[262144];
static unsigned int __attribute__((aligned(16))) checkerTexture[64 * 64];

typedef struct {
    float u, v;
    unsigned int color;
    float x, y, z;
} Vertex;

#define V(c, x, y, z, u, v) { (u), (v), (c), (x), (y), (z) }
static const Vertex cube[] __attribute__((aligned(16))) = {
    /* front */
    V(0xFFFFFFFF,-1,-1,1, 0,64), V(0xFFFFFFFF,1,-1,1, 64,64), V(0xFFFFFFFF,1,1,1, 64,0),
    V(0xFFFFFFFF,-1,-1,1, 0,64), V(0xFFFFFFFF,1,1,1, 64,0), V(0xFFFFFFFF,-1,1,1, 0,0),
    /* back */
    V(0xFFFFFFFF,1,-1,-1, 0,64), V(0xFFFFFFFF,-1,-1,-1, 64,64), V(0xFFFFFFFF,-1,1,-1, 64,0),
    V(0xFFFFFFFF,1,-1,-1, 0,64), V(0xFFFFFFFF,-1,1,-1, 64,0), V(0xFFFFFFFF,1,1,-1, 0,0),
    /* left */
    V(0xFFFFFFFF,-1,-1,-1, 0,64), V(0xFFFFFFFF,-1,-1,1, 64,64), V(0xFFFFFFFF,-1,1,1, 64,0),
    V(0xFFFFFFFF,-1,-1,-1, 0,64), V(0xFFFFFFFF,-1,1,1, 64,0), V(0xFFFFFFFF,-1,1,-1, 0,0),
    /* right */
    V(0xFFFFFFFF,1,-1,1, 0,64), V(0xFFFFFFFF,1,-1,-1, 64,64), V(0xFFFFFFFF,1,1,-1, 64,0),
    V(0xFFFFFFFF,1,-1,1, 0,64), V(0xFFFFFFFF,1,1,-1, 64,0), V(0xFFFFFFFF,1,1,1, 0,0),
    /* top */
    V(0xFFFFFFFF,-1,1,1, 0,64), V(0xFFFFFFFF,1,1,1, 64,64), V(0xFFFFFFFF,1,1,-1, 64,0),
    V(0xFFFFFFFF,-1,1,1, 0,64), V(0xFFFFFFFF,1,1,-1, 64,0), V(0xFFFFFFFF,-1,1,-1, 0,0),
    /* bottom */
    V(0xFFFFFFFF,-1,-1,-1, 0,64), V(0xFFFFFFFF,1,-1,-1, 64,64), V(0xFFFFFFFF,1,-1,1, 64,0),
    V(0xFFFFFFFF,-1,-1,-1, 0,64), V(0xFFFFFFFF,1,-1,1, 64,0), V(0xFFFFFFFF,-1,-1,1, 0,0)
};
#undef V

static const char *dataCandidates[] = {
    "DATA/GTA_VC.DAT",
    "DATA/GTA3.DAT",
    "DATA/MAIN.SCM",
    "MODELS/GTA3.IMG",
    "MODELS/GTA_VC.IMG"
};

static void write_diagnostics(void)
{
    static const char logPath[] = "ms0:/PSP/GAME/VCPSP/VCPSP_DIAGNOSTICS.TXT";
    SceUID fd = sceIoOpen(logPath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
    char line[192];
    unsigned int i;

    if (fd < 0)
        return;

    snprintf(line, sizeof(line), "VC PSP port probe\nBuild: GU textured cube + platform diagnostics\nFree user memory: %d bytes\n",
        sceKernelTotalFreeMemSize());
    sceIoWrite(fd, line, strlen(line));

    for (i = 0; i < sizeof(dataCandidates) / sizeof(dataCandidates[0]); ++i) {
        SceUID file = sceIoOpen(dataCandidates[i], PSP_O_RDONLY, 0);
        snprintf(line, sizeof(line), "%s: %s\n", dataCandidates[i],
            file >= 0 ? "FOUND" : "not found in current working directory");
        sceIoWrite(fd, line, strlen(line));
        if (file >= 0)
            sceIoClose(file);
    }

    sceIoClose(fd);
}

static void build_checker_texture(void)
{
    int x, y;
    for (y = 0; y < 64; ++y) {
        for (x = 0; x < 64; ++x) {
            const int checker = ((x / 8) ^ (y / 8)) & 1;
            checkerTexture[y * 64 + x] = checker ? 0xFFFFC040 : 0xFF204080;
        }
    }
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    write_diagnostics();
    build_checker_texture();
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

        sceGuTexMode(GU_PSM_8888, 0, 0, GU_FALSE);
        sceGuTexImage(0, 64, 64, 64, checkerTexture);
        sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
        sceGuTexFilter(GU_LINEAR, GU_LINEAR);
        sceGuTexWrap(GU_REPEAT, GU_REPEAT);
        sceGuEnable(GU_TEXTURE_2D);
        sceGuDisable(GU_CULL_FACE);
        sceGuColor(0xFFFFFFFF);
        sceGuDrawArray(GU_TRIANGLES,
            GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D,
            sizeof(cube) / sizeof(cube[0]), 0, cube);

        sceGuDisable(GU_TEXTURE_2D);
        sceGuFinish();
        sceGuSync(0, 0);
        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();
    }

    return 0;
}
