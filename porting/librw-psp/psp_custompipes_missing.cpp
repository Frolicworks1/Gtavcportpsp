#include <rw.h>

namespace WorldRender {
int numBlendInsts[3] = {0, 0, 0};
void AtomicFirstPass(rw::Atomic *, int) {}
void AtomicFullyTransparent(rw::Atomic *, int, int) {}
void RenderBlendPass(int) {}
}

namespace CustomPipes {
void CreateVehiclePipe(void) {}
void DestroyVehiclePipe(void) {}
void CreateWorldPipe(void) {}
void DestroyWorldPipe(void) {}
void CreateGlossPipe(void) {}
void DestroyGlossPipe(void) {}
void CreateRimLightPipes(void) {}
void DestroyRimLightPipes(void) {}
}
