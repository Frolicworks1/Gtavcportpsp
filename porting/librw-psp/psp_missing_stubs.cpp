#include "common.h"
#include "CdStream.h"
#include "crossplatform.h"
#include "rwcharset.h"
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

namespace rw {
bool32 Charset::open(void) { return 1; }
void Charset::close(void) {}
Charset *Charset::create(const RGBA *, const RGBA *) { return new Charset(); }
void Charset::destroy(void) { delete this; }
Charset *Charset::setColors(const RGBA *, const RGBA *) { return this; }
void Charset::print(const char *, int32, int32, bool32) {}
void Charset::printBuffered(const char *, int32, int32, bool32) {}
void Charset::flushBuffer(void) {}
}
