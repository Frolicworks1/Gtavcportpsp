#include "common.h"
#include "CdStream.h"
#include "crossplatform.h"
#include <stddef.h>

bool flushStream[5] = {false, false, false, false, false};

extern "C" size_t _dwMemAvailPhys = 0;

RwBool IsForegroundApp(void)
{
    return true;
}

void CapturePad(int)
{
}

const char* _psGetUserFilesFolder(void)
{
    static const char path[] = ".";
    return path;
}

void AddToQueue(Queue*, int)
{
}
