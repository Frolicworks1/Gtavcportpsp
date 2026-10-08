#include "skeleton.h"
#include <pspkernel.h>
#include <pspmoduleinfo.h>
#include <pspctrl.h>
#include <pspiofilemgr.h>
#include <string.h>

PSP_MODULE_INFO("reVC", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | PSP_THREAD_ATTR_VFPU);


static void setupWorkingDirectory(int argc, char **argv)
{
    char path[256];
    path[0] = '\0';

    // The working GTA III PSP build uses the PSP ms0: filesystem and keeps
    // DATA/MODELS relative to the game directory. Recreate that invariant
    // before RenderWare/game initialization instead of relying on the host CWD.
    if (argc > 0 && argv && argv[0]) {
        const char *exe = argv[0];
        const char *slash = strrchr(exe, '/');
        if (slash) {
            size_t n = (size_t)(slash - exe);
            if (n >= sizeof(path)) n = sizeof(path) - 1;
            memcpy(path, exe, n);
            path[n] = '\0';
        }
    }

    if (path[0] && sceIoChdir(path) >= 0)
        return;

    // Fallback for PPSSPP launches that do not provide a full argv[0].
    // The final package is installed under this PSP GAME directory.
    sceIoChdir("ms0:/PSP/GAME/GTAVCPSP");
}

extern "C" int main(int argc, char **argv)
{
    setupWorkingDirectory(argc, argv);

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
