#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspiofilemgr.h>
#include <stdio.h>
#include <string.h>

PSP_MODULE_INFO("VC PSP GU Display Check", 0, 0, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(2048);

#define SCREEN_W 480
#define SCREEN_H 272
#define BUF_W 512

static unsigned int __attribute__((aligned(16))) list[262144];
static unsigned int __attribute__((aligned(16))) checkerTexture[64 * 64];

typedef struct {
    float u, v;
    unsigned int color;
    float x, y, z;
} TexturedVertex;

typedef struct {
    unsigned int color;
    float x, y, z;
} FlatVertex;

#define TV(c, x, y, z, u, v) { (u), (v), (c), (x), (y), (z) }
static const TexturedVertex cube[] __attribute__((aligned(16))) = {
    TV(0xFFFFFFFF,-1,-1,1, 0,64), TV(0xFFFFFFFF,1,-1,1, 64,64), TV(0xFFFFFFFF,1,1,1, 64,0),
    TV(0xFFFFFFFF,-1,-1,1, 0,64), TV(0xFFFFFFFF,1,1,1, 64,0), TV(0xFFFFFFFF,-1,1,1, 0,0),
    TV(0xFFFFFFFF,1,-1,-1, 0,64), TV(0xFFFFFFFF,-1,-1,-1, 64,64), TV(0xFFFFFFFF,-1,1,-1, 64,0),
    TV(0xFFFFFFFF,1,-1,-1, 0,64), TV(0xFFFFFFFF,-1,1,-1, 64,0), TV(0xFFFFFFFF,1,1,-1, 0,0),
    TV(0xFFFFFFFF,-1,-1,-1, 0,64), TV(0xFFFFFFFF,-1,-1,1, 64,64), TV(0xFFFFFFFF,-1,1,1, 64,0),
    TV(0xFFFFFFFF,-1,-1,-1, 0,64), TV(0xFFFFFFFF,-1,1,1, 64,0), TV(0xFFFFFFFF,-1,1,-1, 0,0),
    TV(0xFFFFFFFF,1,-1,1, 0,64), TV(0xFFFFFFFF,1,-1,-1, 64,64), TV(0xFFFFFFFF,1,1,-1, 64,0),
    TV(0xFFFFFFFF,1,-1,1, 0,64), TV(0xFFFFFFFF,1,1,-1, 64,0), TV(0xFFFFFFFF,1,1,1, 0,0),
    TV(0xFFFFFFFF,-1,1,1, 0,64), TV(0xFFFFFFFF,1,1,1, 64,64), TV(0xFFFFFFFF,1,1,-1, 64,0),
    TV(0xFFFFFFFF,-1,1,1, 0,64), TV(0xFFFFFFFF,1,1,-1, 64,0), TV(0xFFFFFFFF,-1,1,-1, 0,0),
    TV(0xFFFFFFFF,-1,-1,-1, 0,64), TV(0xFFFFFFFF,1,-1,-1, 64,64), TV(0xFFFFFFFF,1,-1,1, 64,0),
    TV(0xFFFFFFFF,-1,-1,-1, 0,64), TV(0xFFFFFFFF,1,-1,1, 64,0), TV(0xFFFFFFFF,-1,-1,1, 0,0)
};
#undef TV

/* Screen-space bars are deliberately independent of the 3D camera and texture.
   If these appear but the cube does not, display setup works and the 3D path
   is the next thing to debug. */
static const FlatVertex statusBars[] __attribute__((aligned(16))) = {
    {0xFF20D060, 16, 16, 0}, {0xFF20D060, 144, 16, 0}, {0xFF20D060, 144, 28, 0},
    {0xFF20D060, 16, 16, 0}, {0xFF20D060, 144, 28, 0}, {0xFF20D060, 16, 28, 0},
    {0xFF40A0FF, 16, 36, 0}, {0xFF40A0FF, 96, 36, 0}, {0xFF40A0FF, 96, 48, 0},
    {0xFF40A0FF, 16, 36, 0}, {0xFF40A0FF, 96, 48, 0}, {0xFF40A0FF, 16, 48, 0},
    {0xFFFFC040, 16, 56, 0}, {0xFFFFC040, 64, 56, 0}, {0xFFFFC040, 64, 68, 0},
    {0xFFFFC040, 16, 56, 0}, {0xFFFFC040, 64, 68, 0}, {0xFFFFC040, 16, 68, 0}
};

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

    if (fd < 0) return;

    snprintf(line, sizeof(line), "VC PSP display check\nFree user memory: %d bytes\n",
        sceKernelTotalFreeMemSize());
    sceIoWrite(fd, line, strlen(line));

    for (i = 0; i < sizeof(dataCandidates) / sizeof(dataCandidates[0]); ++i) {
        SceUID file = sceIoOpen(dataCandidates[i], PSP_O_RDONLY, 0);
        snprintf(line, sizeof(line), "%s: %s\n", dataCandidates[i],
            file >= 0 ? "FOUND" : "not found in current working directory");
        sceIoWrite(fd, line, strlen(line));
        if (file >= 0) sceIoClose(file);
    }
    sceIoClose(fd);
}

static void build_checker_texture(void)
{
    int x, y;
    for (y = 0; y < 64; ++y)
        for (x = 0; x < 64; ++x) {
            int checker = ((x / 8) ^ (y / 8)) & 1;
            checkerTexture[y * 64 + x] = checker ? 0xFFFFC040 : 0xFF204080;
        }
}

int main(int argc, char *argv[])
{
    SceCtrlData pad;
    float angle = 0.0f;
    (void)argc;
    (void)argv;

    write_diagnostics();
    build_checker_texture();
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    sceGuInit();
    sceGuStart(GU_DIRECT, list);
    sceGuDrawBuffer(GU_PSM_8888, (void *)0, BUF_W);
    sceGuDispBuffer(SCREEN_W, SCREEN_H, (void *)0x88000, BUF_W);
    sceGuDepthBuffer((void *)0x110000, BUF_W);
    sceGuOffset(2048 - (SCREEN_W / 2), 2048 - (SCREEN_H / 2));
    sceGuViewport(2048, 2048, SCREEN_W, SCREEN_H);
    sceGuScissor(0, 0, SCREEN_W, SCREEN_H);
    sceGuEnable(GU_SCISSOR_TEST);
    sceGuDepthRange(65535, 0);
    sceGuDepthFunc(GU_GEQUAL);
    sceGuEnable(GU_DEPTH_TEST);
    sceGuShadeModel(GU_SMOOTH);
    sceGuFinish();
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuDisplay(GU_TRUE);

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
        sceGumPerspective(60.0f, (float)SCREEN_W / (float)SCREEN_H, 0.5f, 100.0f);
        sceGumMatrixMode(GU_VIEW);
        sceGumLoadIdentity();
        sceGumMatrixMode(GU_MODEL);
        sceGumLoadIdentity();
        {
            ScePspFVector3 position = {0.0f, 0.0f, -4.0f};
            ScePspFVector3 rotation = {angle * 0.73f, angle, angle * 0.37f};
            sceGumTranslate(&position);
            sceGumRotateXYZ(&rotation);
        }

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

        /* Draw a guaranteed, untextured 2D colour marker after the 3D pass. */
        sceGuDisable(GU_TEXTURE_2D);
        sceGuDisable(GU_DEPTH_TEST);
        sceGumMatrixMode(GU_PROJECTION);
        sceGumLoadIdentity();
        sceGumMatrixMode(GU_VIEW);
        sceGumLoadIdentity();
        sceGumMatrixMode(GU_MODEL);
        sceGumLoadIdentity();
        sceGuDrawArray(GU_TRIANGLES,
            GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_2D,
            sizeof(statusBars) / sizeof(statusBars[0]), 0, statusBars);

        sceGuFinish();
        sceGuSync(0, 0);
        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();
    }
    return 0;
}
