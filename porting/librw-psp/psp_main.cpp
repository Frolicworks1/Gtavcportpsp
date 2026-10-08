#include "skeleton.h"
#include <pspkernel.h>
#include <pspmoduleinfo.h>
#include <pspctrl.h>

PSP_MODULE_INFO("reVC", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | PSP_THREAD_ATTR_VFPU);


extern "C" int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

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
