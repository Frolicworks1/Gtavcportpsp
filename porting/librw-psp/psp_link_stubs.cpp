#include "common.h"\n#include "rwpsp.h"\n\nextern "C" double psTimer(){return 0.0;}\nextern "C" void psCameraShowRaster(void*){}\nextern "C" int psCameraBeginUpdate(void*){return 1;}\nextern "C" void* psGrabScreen(void*){return nullptr;}\nextern "C" void psMouseSetPos(void*){}\nextern "C" int psSelectDevice(){return 1;}\nextern "C" void psInstallFileSystem(){}\nextern "C" void psNativeTextureSupport(){}\nextern "C" void psTerminate(){}\nextern "C" int psInitialize(){return 1;}\nextern "C" void* psGetMemoryFunctions(){return nullptr;}\nextern "C" long psp_stub_541(...) __asm__("_ZN2rw3ps220getSizeNativeTextureEPNS_7TextureE"); extern "C" long psp_stub_541(...){return 0;}\nextern "C" long psp_stub_680(...) __asm__("_ZN2rw3ps218writeNativeTextureEPNS_7TextureEPNS_6StreamE"); extern "C" long psp_stub_680(...){return 0;}\nextern "C" long psp_stub_829(...) __asm__("_ZN2rw3ps217readNativeTextureEPNS_6StreamE"); extern "C" long psp_stub_829(...){return 0;}\nextern "C" long psp_stub_964(...) __asm__("_ZN2rw3ps217getSizeNativeDataEPvll"); extern "C" long psp_stub_964(...){return 0;}\nextern "C" long psp_stub_1091(...) __asm__("_ZN2rw3ps215writeNativeDataEPNS_6StreamElPvll"); extern "C" long psp_stub_1091(...){return 0;}\nextern "C" long psp_stub_1231(...) __asm__("_ZN2rw3ps214readNativeDataEPNS_6StreamElPvll"); extern "C" long psp_stub_1231(...){return 0;}\nextern "C" long psp_stub_1370(...) __asm__("_ZN2rw3ps217destroyNativeDataEPvll"); extern "C" long psp_stub_1370(...){return 0;}\nextern "C" long psp_stub_1499(...) __asm__("_ZN2rw3ps29initMatFXEv"); extern "C" long psp_stub_1499(...){return 0;}\nextern "C" long psp_stub_1616(...) __asm__("_ZN2rw3ps217getSizeNativeSkinEPvl"); extern "C" long psp_stub_1616(...){return 0;}\nextern "C" long psp_stub_1744(...) __asm__("_ZN2rw3ps215writeNativeSkinEPNS_6StreamElPvl"); extern "C" long psp_stub_1744(...){return 0;}\nextern "C" long psp_stub_1883(...) __asm__("_ZN2rw3ps28initSkinEv"); extern "C" long psp_stub_1883(...){return 0;}\nextern "C" long psp_stub_1999(...) __asm__("_ZN2rw3ps214readNativeSkinEPNS_6StreamElPvl"); extern "C" long psp_stub_1999(...){return 0;}\nextern "C" long other_stub_2137(...) __asm__("_Z15IsForegroundAppv"); extern "C" long other_stub_2137(...){return 0;}\nextern "C" long other_stub_2256(...) __asm__("_Z10CapturePadi"); extern "C" long other_stub_2256(...){return 0;}\nextern "C" long other_stub_2370(...) __asm__("_ZN11CustomPipes17CreateVehiclePipeEv"); extern "C" long other_stub_2370(...){return 0;}\nextern "C" long other_stub_2506(...) __asm__("_ZN11CustomPipes15CreateWorldPipeEv"); extern "C" long other_stub_2506(...){return 0;}\nextern "C" long other_stub_2640(...) __asm__("_ZN11CustomPipes15CreateGlossPipeEv"); extern "C" long other_stub_2640(...){return 0;}\nextern "C" long other_stub_2774(...) __asm__("_ZN11CustomPipes19CreateRimLightPipesEv"); extern "C" long other_stub_2774(...){return 0;}\nextern "C" long other_stub_2912(...) __asm__("_ZN11CustomPipes18DestroyVehiclePipeEv"); extern "C" long other_stub_2912(...){return 0;}\nextern "C" long other_stub_3049(...) __asm__("_ZN11CustomPipes16DestroyWorldPipeEv"); extern "C" long other_stub_3049(...){return 0;}\nextern "C" long other_stub_3184(...) __asm__("_ZN11CustomPipes16DestroyGlossPipeEv"); extern "C" long other_stub_3184(...){return 0;}\nextern "C" long other_stub_3319(...) __asm__("_ZN11CustomPipes20DestroyRimLightPipesEv"); extern "C" long other_stub_3319(...){return 0;}\nextern "C" long other_stub_3458(...) __asm__("_Z21_psGetUserFilesFolderv"); extern "C" long other_stub_3458(...){return 0;}\nextern "C" long other_stub_3583(...) __asm__("_ZN11WorldRender15AtomicFirstPassEPN2rw6AtomicEi"); extern "C" long other_stub_3583(...){return 0;}\nextern "C" long other_stub_3730(...) __asm__("_ZN11WorldRender22AtomicFullyTransparentEPN2rw6AtomicEii"); extern "C" long other_stub_3730(...){return 0;}\nextern "C" long other_stub_3885(...) __asm__("_ZN11WorldRender15RenderBlendPassEi"); extern "C" long other_stub_3885(...){return 0;}\nextern "C" long other_stub_4019(...) __asm__("_ZN11WorldRender13numBlendInstsE"); extern "C" long other_stub_4019(...){return 0;}\nextern "C" long other_stub_4150(...) __asm__("_ZN2rw7Charset4openEv"); extern "C" long other_stub_4150(...){return 0;}\nextern "C" long other_stub_4270(...) __asm__("_ZN2rw7Charset5closeEv"); extern "C" long other_stub_4270(...){return 0;}\nextern "C" long other_stub_4391(...) __asm__("_ZN2rw7Charset5printEPKclll"); extern "C" long other_stub_4391(...){return 0;}\nextern "C" long other_stub_4517(...) __asm__("_ZN2rw7Charset13printBufferedEPKclll"); extern "C" long other_stub_4517(...){return 0;}\nextern "C" long other_stub_4652(...) __asm__("_ZN2rw7Charset11flushBufferEv"); extern "C" long other_stub_4652(...){return 0;}\nextern "C" long other_stub_4780(...) __asm__("_ZN2rw7Charset9setColorsEPKNS_4RGBAES3_"); extern "C" long other_stub_4780(...){return 0;}\nextern "C" long other_stub_4918(...) __asm__("_ZN2rw7Charset6createEPKNS_4RGBAES3_"); extern "C" long other_stub_4918(...){return 0;}\nextern "C" long other_stub_5053(...) __asm__("_ZN2rw7Charset7destroyEv"); extern "C" long other_stub_5053(...){return 0;}\nextern "C" int gGameState __asm__("gGameState"); int gGameState=0;\nextern "C" int _dwOperatingSystemVersion __asm__("_dwOperatingSystemVersion"); int _dwOperatingSystemVersion=0;\nextern "C" int _dwMemAvailPhys __asm__("_dwMemAvailPhys"); int _dwMemAvailPhys=0;\nnamespace rw { namespace psp2 { Device renderdevice; } }\nextern "C" int main(){ return psInitialize(); }\n
extern "C" long psp_fixed_0(...) __asm__("_ZN2rw3ps220getSizeNativeTextureEPNS_7Texture");
extern "C" long psp_fixed_0(...){ return 0; }

extern "C" long psp_fixed_1(...) __asm__("_ZN2rw3ps218writeNativeTextureEPNS_7TextureEPNS_6Stream");
extern "C" long psp_fixed_1(...){ return 0; }

extern "C" long psp_fixed_2(...) __asm__("_ZN2rw3ps217readNativeTextureEPNS_6Stream");
extern "C" long psp_fixed_2(...){ return 0; }

extern "C" long psp_fixed_3(...) __asm__("_ZN2rw3ps217getSizeNativeDataEPvl");
extern "C" long psp_fixed_3(...){ return 0; }

extern "C" long psp_fixed_4(...) __asm__("_ZN2rw3ps215writeNativeDataEPNS_6StreamElPvl");
extern "C" long psp_fixed_4(...){ return 0; }

extern "C" long psp_fixed_5(...) __asm__("_ZN2rw3ps214readNativeDataEPNS_6StreamElPvl");
extern "C" long psp_fixed_5(...){ return 0; }

extern "C" long psp_fixed_6(...) __asm__("_ZN2rw3ps217destroyNativeDataEPvl");
extern "C" long psp_fixed_6(...){ return 0; }

extern "C" long psp_fixed_7(...) __asm__("_ZN2rw3ps29initMatFXE");
extern "C" long psp_fixed_7(...){ return 0; }

extern "C" long psp_fixed_8(...) __asm__("_ZN2rw3ps217getSizeNativeSkinEPv");
extern "C" long psp_fixed_8(...){ return 0; }

extern "C" long psp_fixed_9(...) __asm__("_ZN2rw3ps215writeNativeSkinEPNS_6StreamElPv");
extern "C" long psp_fixed_9(...){ return 0; }

extern "C" long psp_fixed_10(...) __asm__("_ZN2rw3ps28initSkinE");
extern "C" long psp_fixed_10(...){ return 0; }

extern "C" long psp_fixed_11(...) __asm__("_ZN2rw3ps214readNativeSkinEPNS_6StreamElPv");
extern "C" long psp_fixed_11(...){ return 0; }

extern "C" long psp_fixed_12(...) __asm__("_Z15IsForegroundApp");
extern "C" long psp_fixed_12(...){ return 0; }

extern "C" long psp_fixed_13(...) __asm__("_Z10CapturePad");
extern "C" long psp_fixed_13(...){ return 0; }

extern "C" long psp_fixed_14(...) __asm__("_ZN11CustomPipes17CreateVehiclePipeE");
extern "C" long psp_fixed_14(...){ return 0; }

extern "C" long psp_fixed_15(...) __asm__("_ZN11CustomPipes15CreateWorldPipeE");
extern "C" long psp_fixed_15(...){ return 0; }

extern "C" long psp_fixed_16(...) __asm__("_ZN11CustomPipes15CreateGlossPipeE");
extern "C" long psp_fixed_16(...){ return 0; }

extern "C" long psp_fixed_17(...) __asm__("_ZN11CustomPipes19CreateRimLightPipesE");
extern "C" long psp_fixed_17(...){ return 0; }

extern "C" long psp_fixed_18(...) __asm__("_ZN11CustomPipes18DestroyVehiclePipeE");
extern "C" long psp_fixed_18(...){ return 0; }

extern "C" long psp_fixed_19(...) __asm__("_ZN11CustomPipes16DestroyWorldPipeE");
extern "C" long psp_fixed_19(...){ return 0; }

extern "C" long psp_fixed_20(...) __asm__("_ZN11CustomPipes16DestroyGlossPipeE");
extern "C" long psp_fixed_20(...){ return 0; }

extern "C" long psp_fixed_21(...) __asm__("_ZN11CustomPipes20DestroyRimLightPipesE");
extern "C" long psp_fixed_21(...){ return 0; }

extern "C" long psp_fixed_22(...) __asm__("_Z21_psGetUserFilesFolder");
extern "C" long psp_fixed_22(...){ return 0; }

extern "C" long psp_fixed_23(...) __asm__("_ZN11WorldRender15AtomicFirstPassEPN2rw6AtomicE");
extern "C" long psp_fixed_23(...){ return 0; }

extern "C" long psp_fixed_24(...) __asm__("_ZN11WorldRender22AtomicFullyTransparentEPN2rw6AtomicEi");
extern "C" long psp_fixed_24(...){ return 0; }

extern "C" long psp_fixed_25(...) __asm__("_ZN11WorldRender15RenderBlendPassE");
extern "C" long psp_fixed_25(...){ return 0; }

extern "C" long psp_fixed_26(...) __asm__("_ZN11WorldRender13numBlendInsts");
extern "C" long psp_fixed_26(...){ return 0; }

extern "C" long psp_fixed_27(...) __asm__("_ZN2rw7Charset4openE");
extern "C" long psp_fixed_27(...){ return 0; }

extern "C" long psp_fixed_28(...) __asm__("_ZN2rw7Charset5closeE");
extern "C" long psp_fixed_28(...){ return 0; }

extern "C" long psp_fixed_29(...) __asm__("_ZN2rw7Charset5printEPKcll");
extern "C" long psp_fixed_29(...){ return 0; }

extern "C" long psp_fixed_30(...) __asm__("_ZN2rw7Charset13printBufferedEPKcll");
extern "C" long psp_fixed_30(...){ return 0; }

extern "C" long psp_fixed_31(...) __asm__("_ZN2rw7Charset11flushBufferE");
extern "C" long psp_fixed_31(...){ return 0; }

extern "C" long psp_fixed_32(...) __asm__("_ZN2rw7Charset9setColorsEPKNS_4RGBAES3");
extern "C" long psp_fixed_32(...){ return 0; }

extern "C" long psp_fixed_33(...) __asm__("_ZN2rw7Charset6createEPKNS_4RGBAES3");
extern "C" long psp_fixed_33(...){ return 0; }

extern "C" long psp_fixed_34(...) __asm__("_ZN2rw7Charset7destroyE");
extern "C" long psp_fixed_34(...){ return 0; }

extern "C" long psp_fixed_35(...) __asm__("gGameStat");
extern "C" long psp_fixed_35(...){ return 0; }

extern "C" long psp_fixed_36(...) __asm__("_dwOperatingSystemVersio");
extern "C" long psp_fixed_36(...){ return 0; }

extern "C" long psp_fixed_37(...) __asm__("_dwMemAvailPhy");
extern "C" long psp_fixed_37(...){ return 0; }
