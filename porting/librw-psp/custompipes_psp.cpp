#include "common.h"
#include <rw.h>
#include "custompipes.h"

namespace WorldRender {
int numBlendInsts[3] = {0,0,0};
void AtomicFirstPass(RpAtomic *atomic, int) { if(atomic) RpAtomicRender(atomic); }
void AtomicFullyTransparent(RpAtomic *atomic, int, int) { if(atomic) RpAtomicRender(atomic); }
void RenderBlendPass(int) {}
}

namespace CustomPipes {
void CustomPipeRegister(void) {}
void CustomPipeRegisterGL(void) {}
void CustomPipeInit(void) {}
void CustomPipeShutdown(void) {}
void SetTxdFindCallback(void) {}
void EnvMapRender(void) {}
void CreateVehiclePipe(void) {}
void DestroyVehiclePipe(void) {}
void AttachVehiclePipe(rw::Atomic*) {}
void AttachVehiclePipe(rw::Clump*) {}
void CreateWorldPipe(void) {}
void DestroyWorldPipe(void) {}
void AttachWorldPipe(rw::Atomic*) {}
void AttachWorldPipe(rw::Clump*) {}
void CreateGlossPipe(void) {}
void DestroyGlossPipe(void) {}
void AttachGlossPipe(rw::Atomic*) {}
void AttachGlossPipe(rw::Clump*) {}
rw::Texture *GetGlossTex(rw::Material*) { return nil; }
void CreateRimLightPipes(void) {}
void DestroyRimLightPipes(void) {}
void AttachRimPipe(rw::Atomic*) {}
void AttachRimPipe(rw::Clump*) {}
}
