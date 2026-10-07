#include "common.h"
#include "Pad.h"
#include <pspctrl.h>

void CapturePad(RwInt32 padID)
{
    if (padID < 0 || padID >= MAX_PADS)
        return;

    SceCtrlData pad;
    sceCtrlPeekBufferPositive(&pad, 0, 1);

    CControllerState &s = GetPad(padID)->PCTempJoyState;
    s.Clear();
    s.DPadUp = !!(pad.Buttons & PSP_CTRL_UP);
    s.DPadDown = !!(pad.Buttons & PSP_CTRL_DOWN);
    s.DPadLeft = !!(pad.Buttons & PSP_CTRL_LEFT);
    s.DPadRight = !!(pad.Buttons & PSP_CTRL_RIGHT);
    s.Triangle = !!(pad.Buttons & PSP_CTRL_TRIANGLE);
    s.Circle = !!(pad.Buttons & PSP_CTRL_CIRCLE);
    s.Cross = !!(pad.Buttons & PSP_CTRL_CROSS);
    s.Square = !!(pad.Buttons & PSP_CTRL_SQUARE);
    s.Start = !!(pad.Buttons & PSP_CTRL_START);
    s.Select = !!(pad.Buttons & PSP_CTRL_SELECT);
    s.LeftShoulder1 = !!(pad.Buttons & PSP_CTRL_LTRIGGER);
    s.RightShoulder1 = !!(pad.Buttons & PSP_CTRL_RTRIGGER);
}
