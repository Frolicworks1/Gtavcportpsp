#include "common.h"
#include "CdStream.h"
#include "crossplatform.h"
#include <stddef.h>

extern "C" size_t _dwMemAvailPhys = 0;

bool IsForegroundApp(void)
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
