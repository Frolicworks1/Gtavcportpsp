#include "../rw.h"
#include "rwpsp.h"

namespace rw { namespace psp {

int32 nativeRasterOffset = 0;

void *destroyNativeData(void *object, int32, int32) { return object; }
Stream *readNativeData(Stream *stream, int32, void *, int32, int32) { return stream; }
Stream *writeNativeData(Stream *stream, int32, void *, int32, int32) { return stream; }
int32 getSizeNativeData(void *, int32, int32) { return 0; }

Texture *readNativeTexture(Stream *) { return nil; }
void writeNativeTexture(Texture *, Stream *) {}
int32 getSizeNativeTexture(Texture *) { return 0; }

Stream *readNativeSkin(Stream *stream, int32, void *, int32) { return stream; }
Stream *writeNativeSkin(Stream *stream, int32, void *, int32) { return stream; }
int32 getSizeNativeSkin(void *, int32) { return 0; }

void initMatFX(void) {}
void initSkin(void) {}

} }

// The generic librw serializers reference every desktop backend. On PSP these
// formats are intentionally decoded through the common path, so the unused
// backend hooks resolve to harmless no-ops instead of pulling desktop code.
#define PSP_BACKEND_ALIAS(NS) \
namespace rw { namespace NS { \
int32 nativeRasterOffset = 0; \
void *destroyNativeData(void *object, int32, int32) { return object; } \
Stream *readNativeData(Stream *stream, int32, void *, int32, int32) { return stream; } \
Stream *writeNativeData(Stream *stream, int32, void *, int32, int32) { return stream; } \
int32 getSizeNativeData(void *, int32, int32) { return 0; } \
Texture *readNativeTexture(Stream *) { return nil; } \
void writeNativeTexture(Texture *, Stream *) {} \
int32 getSizeNativeTexture(Texture *) { return 0; } \
Stream *readNativeSkin(Stream *stream, int32, void *, int32) { return stream; } \
Stream *writeNativeSkin(Stream *stream, int32, void *, int32) { return stream; } \
int32 getSizeNativeSkin(void *, int32) { return 0; } \
void initMatFX(void) {} \
void initSkin(void) {} \
} }

PSP_BACKEND_ALIAS(ps2)
PSP_BACKEND_ALIAS(xbox)
PSP_BACKEND_ALIAS(d3d8)
PSP_BACKEND_ALIAS(d3d9)
PSP_BACKEND_ALIAS(wdgl)
PSP_BACKEND_ALIAS(gl3)

namespace rw { namespace d3d {
int32 nativeRasterOffset = 0;
void *allocateDXT(Raster *, long, long, long) { return nil; }
void setTexels(Raster *, void *, long) {}
} }
