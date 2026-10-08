#include "skeleton.h"
#include <pspkernel.h>
#include <pspmoduleinfo.h>
#include <pspctrl.h>
#include <pspdebug.h>

PSP_MODULE_INFO("reVC", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | PSP_THREAD_ATTR_VFPU);

static int exitCallback(int, int, void *)
{
    RsGlobal.quit = TRUE;
    return 0;
}

static int callbackThread(unsigned int, void *)
{
    int cb = sceKernelCreateCallback("Exit Callback", exitCallback, NULL);
    if (cb >= 0)
        sceKernelRegisterExitCallback(cb);
    sceKernelSleepThreadCB();
    return 0;
}

static void installExitCallback(void)
{
    int thid = sceKernelCreateThread("Exit Callback Thread", callbackThread, 0x11, 0xFA0, 0, NULL);
    if (thid >= 0)
        sceKernelStartThread(thid, 0, NULL);
}

extern "C" int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    // Keep fatal PSP exceptions visible in PPSSPP instead of silently dropping the game.
    pspDebugScreenInit();
    pspDebugInstallErrorHandler(NULL);
    installExitCallback();

    // Initialise the real PSP controller path before the game starts polling it.
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    if (RsEventHandler(rsINITIALIZE, NULL) == rsEVENTERROR)
        return 1;

    if (RsEventHandler(rsRWINITIALIZE, NULL) == rsEVENTERROR) {
        RsEventHandler(rsTERMINATE, NULL);
        return 1;
    }

    while (!RsGlobal.quit) {
        RsEventHandler(rsIDLE, (void *)TRUE);
        sceKernelDelayThread(1000);
    }

    RsEventHandler(rsTERMINATE, NULL);
    return 0;
}
