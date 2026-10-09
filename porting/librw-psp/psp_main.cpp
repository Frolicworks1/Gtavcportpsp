#include "skeleton.h"
#include <pspkernel.h>
#include <pspmoduleinfo.h>
#include <pspctrl.h>
#include <pspiofilemgr.h>
#include <string.h>

PSP_MODULE_INFO("reVC", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | PSP_THREAD_ATTR_VFPU);

static int gStartupLog = -1;
static char gStartupLogPath[256];

static int tryOpenLog(const char *path)
{
    if (!path || !path[0])
        return -1;
    return sceIoOpen(path, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_APPEND, 0777);
}

static void initStartupLogPath(int argc, char **argv)
{
    gStartupLogPath[0] = '\0';
    if (argc > 0 && argv && argv[0]) {
        const char *slash = strrchr(argv[0], '/');
        if (slash) {
            size_t n = (size_t)(slash - argv[0]);
            if (n > sizeof(gStartupLogPath) - sizeof("/psp_startup.log"))
                n = sizeof(gStartupLogPath) - sizeof("/psp_startup.log");
            memcpy(gStartupLogPath, argv[0], n);
            gStartupLogPath[n] = '\0';
            strncat(gStartupLogPath, "/psp_startup.log",
                    sizeof(gStartupLogPath) - strlen(gStartupLogPath) - 1);
        }
    }
}

extern "C" void pspStartupLog(const char *msg)
{
    if (gStartupLog < 0) {
        gStartupLog = tryOpenLog(gStartupLogPath);
        if (gStartupLog < 0)
            gStartupLog = tryOpenLog("ms0:/GAME/Grand Theft Auto Vice City/psp_startup.log");
        if (gStartupLog < 0)
            gStartupLog = tryOpenLog("ms0:/GAME/GTAVCPSP/psp_startup.log");
        if (gStartupLog < 0)
            gStartupLog = tryOpenLog("psp_startup.log");
    }
    if (gStartupLog >= 0 && msg) {
        sceIoWrite(gStartupLog, msg, strlen(msg));
        sceIoWrite(gStartupLog, "\n", 1);
    }
}

static void setupWorkingDirectory(int argc, char **argv)
{
    char path[256];
    path[0] = '\0';
    pspStartupLog("stage:main-enter");

    if (argc > 0 && argv && argv[0]) {
        const char *exe = argv[0];
        pspStartupLog("stage:argv-present");
        const char *slash = strrchr(exe, '/');
        if (slash) {
            size_t n = (size_t)(slash - exe);
            if (n >= sizeof(path)) n = sizeof(path) - 1;
            memcpy(path, exe, n);
            path[n] = '\0';
        }
    }

    if (path[0]) {
        pspStartupLog("stage:derived-path");
        if (sceIoChdir(path) >= 0) {
            pspStartupLog("stage:derived-chdir-ok");
            return;
        }
        pspStartupLog("stage:derived-chdir-failed");
    }

    pspStartupLog("stage:keep-loader-cwd");
}

static void probeRequiredData(void)
{
    SceUID fd = sceIoOpen("DATA/GTA_VC.DAT", PSP_O_RDONLY, 0);
    if (fd >= 0) {
        pspStartupLog("data:GTA_VC.DAT=present");
        sceIoClose(fd);
    } else {
        pspStartupLog("data:GTA_VC.DAT=missing");
    }

    SceUID dir = sceIoDopen("DATA");
    if (dir >= 0) {
        pspStartupLog("data:DATA-dir=present");
        sceIoDclose(dir);
    } else {
        pspStartupLog("data:DATA-dir=missing");
    }
}

extern "C" int main(int argc, char **argv)
{
    initStartupLogPath(argc, argv);
    pspStartupLog("stage:main-enter");
    setupWorkingDirectory(argc, argv);
    pspStartupLog("stage:cwd-setup-done");
    probeRequiredData();
    pspStartupLog("stage:controller-before");

    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
    pspStartupLog("stage:controller-after");

    pspStartupLog("stage:rsinitialize-before");
    if (RsEventHandler(rsINITIALIZE, NULL) == rsEVENTERROR) {
        pspStartupLog("stage:rsinitialize-error");
        return 1;
    }
    pspStartupLog("stage:rsinitialize-ok");

    pspStartupLog("stage:rsrwinitialize-before");
    if (RsEventHandler(rsRWINITIALIZE, NULL) == rsEVENTERROR) {
        pspStartupLog("stage:rsrwinitialize-error");
        RsEventHandler(rsTERMINATE, NULL);
        return 1;
    }
    pspStartupLog("stage:rsrwinitialize-ok");

    pspStartupLog("stage:idle-loop");
    while (!RsGlobal.quit) {
        RsEventHandler(rsIDLE, (void *)TRUE);
        sceKernelDelayThread(1000);
    }

    pspStartupLog("stage:terminate");
    RsEventHandler(rsTERMINATE, NULL);
    if (gStartupLog >= 0)
        sceIoClose(gStartupLog);
    return 0;
}
