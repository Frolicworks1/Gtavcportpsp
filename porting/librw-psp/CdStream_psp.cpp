#include "common.h"
#include "CdStream.h"
#ifndef MAX_CDIMAGES
#define MAX_CDIMAGES 8
#endif

#include <pspiofilemgr.h>
#include <stdio.h>
#include <string.h>

static SceUID gPspImages[MAX_CDIMAGES];
static char gPspImageNames[MAX_CDIMAGES][256];
static int gPspNumImages = 0;
static int gPspNumChannels = 0;
static int gPspLastPosn = 0;

static void clearImageSlots(void)
{
    for(int i = 0; i < MAX_CDIMAGES; ++i)
        gPspImages[i] = -1;
}

void CdStreamInit(int numChannels)
{
    clearImageSlots();
    gPspNumChannels = numChannels;
    gPspLastPosn = 0;
}

int CdStreamRead(int channel, void *buffer, unsigned int offset, unsigned int size)
{
    if(channel < 0 || channel >= gPspNumChannels || !buffer)
        return STREAM_ERROR;

    const unsigned int image = _GET_INDEX(offset);
    const unsigned int sector = _GET_OFFSET(offset);

    if(image >= MAX_CDIMAGES || gPspImages[image] < 0)
        return STREAM_ERROR_OPENCD;

    const SceOff position = (SceOff)sector * CDSTREAM_SECTOR_SIZE;
    const int seekResult = (int)sceIoLseek(gPspImages[image], position, PSP_SEEK_SET);
    if(seekResult < 0)
        return STREAM_ERROR;

    const unsigned int bytes = size * CDSTREAM_SECTOR_SIZE;
    const int got = sceIoRead(gPspImages[image], buffer, bytes);
    gPspLastPosn = (int)(offset + size);

    return got == (int)bytes ? STREAM_SUCCESS : STREAM_ERROR;
}

int CdStreamGetStatus(int channel)
{
    return (channel >= 0 && channel < gPspNumChannels) ? STREAM_NONE : STREAM_ERROR;
}

int CdStreamSync(int channel)
{
    return CdStreamGetStatus(channel);
}

int CdStreamGetLastPosn(void)
{
    return gPspLastPosn;
}

void CdStreamShutdown(void)
{
    CdStreamRemoveImages();
}

bool CdStreamAddImage(char const *path)
{
    if(!path || gPspNumImages >= MAX_CDIMAGES)
        return false;

    SceUID fd = sceIoOpen(path, PSP_O_RDONLY, 0);
    if(fd < 0)
        return false;

    gPspImages[gPspNumImages] = fd;
    strncpy(gPspImageNames[gPspNumImages], path, sizeof(gPspImageNames[0]) - 1);
    gPspImageNames[gPspNumImages][sizeof(gPspImageNames[0]) - 1] = '\0';
    ++gPspNumImages;
    return true;
}

char *CdStreamGetImageName(int cd)
{
    if(cd < 0 || cd >= gPspNumImages || gPspImages[cd] < 0)
        return NULL;
    return gPspImageNames[cd];
}

void CdStreamRemoveImages(void)
{
    for(int i = 0; i < gPspNumImages; ++i) {
        if(gPspImages[i] >= 0)
            sceIoClose(gPspImages[i]);
        gPspImages[i] = -1;
        gPspImageNames[i][0] = '\0';
    }
    gPspNumImages = 0;
}

int32 CdStreamGetNumImages(void)
{
    return gPspNumImages;
}

uint32 GetGTA3ImgSize(void)
{
    if(gPspNumImages <= 0 || gPspImages[0] < 0)
        return 0;

    SceIoStat st;
    memset(&st, 0, sizeof(st));
    if(sceIoGetstat(gPspImageNames[0], &st) < 0)
        return 0;

    return (uint32)st.st_size;
}
