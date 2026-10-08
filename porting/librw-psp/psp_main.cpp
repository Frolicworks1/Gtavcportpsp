#include "skeleton.h"
#include <pspkernel.h>

extern "C" int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

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
