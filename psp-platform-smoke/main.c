#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspgu.h>

PSP_MODULE_INFO("VC PSP Port Foundation", 0, 0, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(2048);

static unsigned int __attribute__((aligned(16))) list[262144];

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
    sceGuDisable(GU_DEPTH_TEST);
    sceGuFinish();
    sceGuSync(0, 0);
    sceDisplayWaitVblankStart();
    sceGuDisplay(GU_TRUE);

    /* This diagnostic intentionally never exits on a controller press.
       START toggles the screen colour so input can be tested without
       triggering the previously crashing exit path. */
    SceCtrlData pad;
    unsigned int previousButtons = 0;
    unsigned int screenColor = 0xFF20D020;
    for (;;) {
        sceCtrlReadBufferPositive(&pad, 1);
        if ((pad.Buttons & PSP_CTRL_START) &&
            !(previousButtons & PSP_CTRL_START)) {
            screenColor = (screenColor == 0xFF20D020)
                ? 0xFF2020D0 : 0xFF20D020;
        }
        previousButtons = pad.Buttons;

        sceGuStart(GU_DIRECT, list);
        sceGuClearColor(screenColor);
        sceGuClear(GU_COLOR_BUFFER_BIT);
        sceGuFinish();
        sceGuSync(0, 0);
        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();
    }

    return 0;
}
