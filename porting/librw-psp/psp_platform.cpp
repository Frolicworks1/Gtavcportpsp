#include "platform.h"
#include "skeleton.h"
#include <pspkernel.h>
#include <string.h>

extern "C" {
long _dwOperatingSystemVersion = 0;
int gGameState = 0;
}

extern "C" double psTimer(void)
{
    return (double)sceKernelGetSystemTimeWide() / 1000000.0;
}

extern "C" RwBool psInitialize(void) { return TRUE; }
extern "C" void psTerminate(void) {}
extern "C" void psCameraShowRaster(RwCamera*) {}
extern "C" RwBool psCameraBeginUpdate(RwCamera*) { return TRUE; }
extern "C" RwImage *psGrabScreen(RwCamera*) { return NULL; }
extern "C" void psMouseSetPos(RwV2d*) {}
extern "C" RwBool psSelectDevice(void) { return TRUE; }
extern "C" RwMemoryFunctions *psGetMemoryFunctions(void) { return NULL; }
extern "C" RwBool psInstallFileSystem(void) { return TRUE; }
extern "C" RwBool psNativeTextureSupport(void) { return TRUE; }

extern "C" void _InputTranslateShiftKeyUpDown(RsKeyCodes*) {}
extern "C" long _InputInitialiseMouse(bool) { return 0; }
extern "C" void _InputShutdownMouse(void) {}
extern "C" bool _InputMouseNeedsExclusive(void) { return false; }
extern "C" void _InputInitialiseJoys(void) {}
extern "C" void HandleExit(void) {}

extern "C" void _psSelectScreenVM(RwInt32) {}
extern "C" void InitialiseLanguage(void) {}
extern "C" RwBool _psSetVideoMode(RwInt32, RwInt32) { return TRUE; }

extern "C" RwChar **_psGetVideoModeList(void)
{
    static RwChar mode0[] = "480x272x32";
    static RwChar *modes[] = { mode0, NULL };
    return modes;
}

extern "C" RwInt32 _psGetNumVideModes(void) { return 1; }
