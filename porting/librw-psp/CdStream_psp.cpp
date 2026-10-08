#include "common.h"
#include "CdStream.h"
#ifndef MAX_CDIMAGES
#define MAX_CDIMAGES 8
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *gPspImages[MAX_CDIMAGES];
static char gPspImageNames[MAX_CDIMAGES][64];
static int gPspNumImages = 0;
static int gPspNumChannels = 0;
static int gPspLastPosn = 0;

void CdStreamInit(int numChannels)
{
    gPspNumChannels = numChannels;
    gPspLastPosn = 0;
}

int CdStreamRead(int channel, void *buffer, unsigned int offset, unsigned int size)
{
    if(channel < 0 || channel >= gPspNumChannels || !buffer) return STREAM_ERROR;
    const unsigned int image = _GET_INDEX(offset);
    const unsigned int sector = _GET_OFFSET(offset);
    if(image >= MAX_CDIMAGES || !gPspImages[image]) return STREAM_ERROR_OPENCD;
    FILE *f = gPspImages[image];
    if(fseek(f, (long)sector * CDSTREAM_SECTOR_SIZE, SEEK_SET) != 0) return STREAM_ERROR;
    const size_t bytes = (size_t)size * CDSTREAM_SECTOR_SIZE;
    const size_t got = fread(buffer, 1, bytes, f);
    gPspLastPosn = (int)(offset + size);
    return got == bytes ? STREAM_SUCCESS : STREAM_ERROR;
}

int CdStreamGetStatus(int channel)
{
    return (channel >= 0 && channel < gPspNumChannels) ? STREAM_NONE : STREAM_ERROR;
}

int CdStreamSync(int channel)
{
    return CdStreamGetStatus(channel);
}

int CdStreamGetLastPosn(void) { return gPspLastPosn; }

void CdStreamShutdown(void) { CdStreamRemoveImages(); }

bool CdStreamAddImage(char const *path)
{
    if(!path || gPspNumImages >= MAX_CDIMAGES) return false;
    FILE *f = fopen(path, "rb");
    if(!f) return false;
    gPspImages[gPspNumImages] = f;
    strncpy(gPspImageNames[gPspNumImages], path, sizeof(gPspImageNames[0]) - 1);
    gPspImageNames[gPspNumImages][sizeof(gPspImageNames[0]) - 1] = 0;
    ++gPspNumImages;
    return true;
}

char *CdStreamGetImageName(int cd)
{
    if(cd < 0 || cd >= gPspNumImages || !gPspImages[cd]) return NULL;
    return gPspImageNames[cd];
}

void CdStreamRemoveImages(void)
{
    for(int i=0;i<gPspNumImages;i++) {
        if(gPspImages[i]) fclose(gPspImages[i]);
        gPspImages[i] = NULL;
        gPspImageNames[i][0] = 0;
    }
    gPspNumImages = 0;
}

int CdStreamGetNumImages(void) { return gPspNumImages; }

uint32 GetGTA3ImgSize(void)
{
    if(gPspNumImages <= 0 || !gPspImages[0]) return 0;
    FILE *f = gPspImages[0];
    long old = ftell(f);
    if(fseek(f, 0, SEEK_END) != 0) return 0;
    long size = ftell(f);
    fseek(f, old, SEEK_SET);
    return size > 0 ? (uint32)size : 0;
}
