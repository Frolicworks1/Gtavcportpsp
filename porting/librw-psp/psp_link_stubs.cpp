#include "common.h"
#include "../../vendor/librw/src/psp/rwpsp.h"
#include <stdint.h>
extern "C" double psTimer(){return 0.0;}
extern "C" void psCameraShowRaster(void*){}
extern "C" int psCameraBeginUpdate(void*){return 1;}
extern "C" void* psGrabScreen(void*){return 0;}
extern "C" void psMouseSetPos(void*){}
extern "C" int psSelectDevice(){return 1;}
extern "C" void psInstallFileSystem(){}
extern "C" void psNativeTextureSupport(){}
extern "C" void psTerminate(){}
extern "C" int psInitialize(){return 1;}
extern "C" void* psGetMemoryFunctions(){return 0;}
extern "C" long psp_stub_0(...) __asm__("_ZN2rw4wdgl17getSizeNativeDataEPvll");\nextern "C" long psp_stub_0(...){return 0;}\nextern "C" long psp_stub_1(...) __asm__("_ZN2rw3ps217getSizeNativeDataEPvll");\nextern "C" long psp_stub_1(...){return 0;}\nextern "C" long psp_stub_2(...) __asm__("_ZN2rw4xbox17getSizeNativeDataEPvll");\nextern "C" long psp_stub_2(...){return 0;}\nextern "C" long psp_stub_3(...) __asm__("_ZN2rw4d3d817getSizeNativeDataEPvll");\nextern "C" long psp_stub_3(...){return 0;}\nextern "C" long psp_stub_4(...) __asm__("_ZN2rw4d3d917getSizeNativeDataEPvll");\nextern "C" long psp_stub_4(...){return 0;}\nextern "C" long psp_stub_5(...) __asm__("_ZN2rw4wdgl15writeNativeDataEPNS_6StreamElPvll");\nextern "C" long psp_stub_5(...){return 0;}\nextern "C" long psp_stub_6(...) __asm__("_ZN2rw3ps215writeNativeDataEPNS_6StreamElPvll");\nextern "C" long psp_stub_6(...){return 0;}\nextern "C" long psp_stub_7(...) __asm__("_ZN2rw4xbox15writeNativeDataEPNS_6StreamElPvll");\nextern "C" long psp_stub_7(...){return 0;}\nextern "C" long psp_stub_8(...) __asm__("_ZN2rw4d3d815writeNativeDataEPNS_6StreamElPvll");\nextern "C" long psp_stub_8(...){return 0;}\nextern "C" long psp_stub_9(...) __asm__("_ZN2rw4d3d915writeNativeDataEPNS_6StreamElPvll");\nextern "C" long psp_stub_9(...){return 0;}\nextern "C" long psp_stub_10(...) __asm__("_ZN2rw4wdgl14readNativeDataEPNS_6StreamElPvll");\nextern "C" long psp_stub_10(...){return 0;}\nextern "C" long psp_stub_11(...) __asm__("_ZN2rw4xbox14readNativeDataEPNS_6StreamElPvll");\nextern "C" long psp_stub_11(...){return 0;}\nextern "C" long psp_stub_12(...) __asm__("_ZN2rw3ps214readNativeDataEPNS_6StreamElPvll");\nextern "C" long psp_stub_12(...){return 0;}\nextern "C" long psp_stub_13(...) __asm__("_ZN2rw4d3d914readNativeDataEPNS_6StreamElPvll");\nextern "C" long psp_stub_13(...){return 0;}\nextern "C" long psp_stub_14(...) __asm__("_ZN2rw4d3d814readNativeDataEPNS_6StreamElPvll");\nextern "C" long psp_stub_14(...){return 0;}\nextern "C" long psp_stub_15(...) __asm__("_ZN2rw3gl317destroyNativeDataEPvll");\nextern "C" long psp_stub_15(...){return 0;}\nextern "C" long psp_stub_16(...) __asm__("_ZN2rw3ps217destroyNativeDataEPvll");\nextern "C" long psp_stub_16(...){return 0;}\nextern "C" long psp_stub_17(...) __asm__("_ZN2rw4xbox17destroyNativeDataEPvll");\nextern "C" long psp_stub_17(...){return 0;}\nextern "C" long psp_stub_18(...) __asm__("_ZN2rw4d3d817destroyNativeDataEPvll");\nextern "C" long psp_stub_18(...){return 0;}\nextern "C" long psp_stub_19(...) __asm__("_ZN2rw4d3d917destroyNativeDataEPvll");\nextern "C" long psp_stub_19(...){return 0;}\nextern "C" long psp_stub_20(...) __asm__("_ZN2rw4wdgl17destroyNativeDataEPvll");\nextern "C" long psp_stub_20(...){return 0;}\nextern "C" long psp_stub_21(...) __asm__("_ZN2rw3ps29initMatFXEv");\nextern "C" long psp_stub_21(...){return 0;}\nextern "C" long psp_stub_22(...) __asm__("_ZN2rw4xbox9initMatFXEv");\nextern "C" long psp_stub_22(...){return 0;}\nextern "C" long psp_stub_23(...) __asm__("_ZN2rw4d3d89initMatFXEv");\nextern "C" long psp_stub_23(...){return 0;}\nextern "C" long psp_stub_24(...) __asm__("_ZN2rw4d3d99initMatFXEv");\nextern "C" long psp_stub_24(...){return 0;}\nextern "C" long psp_stub_25(...) __asm__("_ZN2rw4wdgl9initMatFXEv");\nextern "C" long psp_stub_25(...){return 0;}\nextern "C" long psp_stub_26(...) __asm__("_ZN2rw3gl39initMatFXEv");\nextern "C" long psp_stub_26(...){return 0;}\nextern "C" long psp_stub_27(...) __asm__("_ZN2rw3d3d11allocateDXTEPNS_6RasterElll");\nextern "C" long psp_stub_27(...){return 0;}\nextern "C" long psp_stub_28(...) __asm__("_ZN2rw3d3d9setTexelsEPNS_6RasterEPvl");\nextern "C" long psp_stub_28(...){return 0;}\nextern "C" long psp_stub_29(...) __asm__("_ZN2rw4xbox17getSizeNativeSkinEPvl");\nextern "C" long psp_stub_29(...){return 0;}\nextern "C" long psp_stub_30(...) __asm__("_ZN2rw3ps217getSizeNativeSkinEPvl");\nextern "C" long psp_stub_30(...){return 0;}\nextern "C" long psp_stub_31(...) __asm__("_ZN2rw4wdgl17getSizeNativeSkinEPvl");\nextern "C" long psp_stub_31(...){return 0;}\nextern "C" long psp_stub_32(...) __asm__("_ZN2rw4xbox15writeNativeSkinEPNS_6StreamElPvl");\nextern "C" long psp_stub_32(...){return 0;}\nextern "C" long psp_stub_33(...) __asm__("_ZN2rw4wdgl15writeNativeSkinEPNS_6StreamElPvl");\nextern "C" long psp_stub_33(...){return 0;}\nextern "C" long psp_stub_34(...) __asm__("_ZN2rw3ps215writeNativeSkinEPNS_6StreamElPvl");\nextern "C" long psp_stub_34(...){return 0;}\nextern "C" long psp_stub_35(...) __asm__("_ZN2rw3ps28initSkinEv");\nextern "C" long psp_stub_35(...){return 0;}\nextern "C" long psp_stub_36(...) __asm__("_ZN2rw4xbox8initSkinEv");\nextern "C" long psp_stub_36(...){return 0;}\nextern "C" long psp_stub_37(...) __asm__("_ZN2rw4d3d88initSkinEv");\nextern "C" long psp_stub_37(...){return 0;}\nextern "C" long psp_stub_38(...) __asm__("_ZN2rw4d3d98initSkinEv");\nextern "C" long psp_stub_38(...){return 0;}\nextern "C" long psp_stub_39(...) __asm__("_ZN2rw4wdgl8initSkinEv");\nextern "C" long psp_stub_39(...){return 0;}\nextern "C" long psp_stub_40(...) __asm__("_ZN2rw3gl38initSkinEv");\nextern "C" long psp_stub_40(...){return 0;}\nextern "C" long psp_stub_41(...) __asm__("_ZN2rw4xbox14readNativeSkinEPNS_6StreamElPvl");\nextern "C" long psp_stub_41(...){return 0;}\nextern "C" long psp_stub_42(...) __asm__("_ZN2rw4wdgl14readNativeSkinEPNS_6StreamElPvl");\nextern "C" long psp_stub_42(...){return 0;}\nextern "C" long psp_stub_43(...) __asm__("_ZN2rw3ps214readNativeSkinEPNS_6StreamElPvl");\nextern "C" long psp_stub_43(...){return 0;}\nextern "C" long psp_stub_44(...) __asm__("_ZN11CustomPipes17CreateVehiclePipeEv");\nextern "C" long psp_stub_44(...){return 0;}\nextern "C" long psp_stub_45(...) __asm__("_ZN11CustomPipes15CreateWorldPipeEv");\nextern "C" long psp_stub_45(...){return 0;}\nextern "C" long psp_stub_46(...) __asm__("_ZN11CustomPipes15CreateGlossPipeEv");\nextern "C" long psp_stub_46(...){return 0;}\nextern "C" long psp_stub_47(...) __asm__("_ZN11CustomPipes19CreateRimLightPipesEv");\nextern "C" long psp_stub_47(...){return 0;}\nextern "C" long psp_stub_48(...) __asm__("_ZN11CustomPipes18DestroyVehiclePipeEv");\nextern "C" long psp_stub_48(...){return 0;}\nextern "C" long psp_stub_49(...) __asm__("_ZN11CustomPipes16DestroyWorldPipeEv");\nextern "C" long psp_stub_49(...){return 0;}\nextern "C" long psp_stub_50(...) __asm__("_ZN11CustomPipes16DestroyGlossPipeEv");\nextern "C" long psp_stub_50(...){return 0;}\nextern "C" long psp_stub_51(...) __asm__("_ZN11CustomPipes20DestroyRimLightPipesEv");\nextern "C" long psp_stub_51(...){return 0;}\nextern "C" long psp_stub_52(...) __asm__("_ZN11WorldRender15AtomicFirstPassEPN2rw6AtomicEi");\nextern "C" long psp_stub_52(...){return 0;}\nextern "C" long psp_stub_53(...) __asm__("_ZN11WorldRender22AtomicFullyTransparentEPN2rw6AtomicEii");\nextern "C" long psp_stub_53(...){return 0;}\nextern "C" long psp_stub_54(...) __asm__("_ZN11WorldRender15RenderBlendPassEi");\nextern "C" long psp_stub_54(...){return 0;}\nextern "C" long psp_stub_55(...) __asm__("_ZN2rw7Charset4openEv");\nextern "C" long psp_stub_55(...){return 0;}\nextern "C" long psp_stub_56(...) __asm__("_ZN2rw7Charset5closeEv");\nextern "C" long psp_stub_56(...){return 0;}\nextern "C" long psp_stub_57(...) __asm__("_ZN2rw7Charset5printEPKclll");\nextern "C" long psp_stub_57(...){return 0;}\nextern "C" long psp_stub_58(...) __asm__("_ZN2rw7Charset13printBufferedEPKclll");\nextern "C" long psp_stub_58(...){return 0;}\nextern "C" long psp_stub_59(...) __asm__("_ZN2rw7Charset11flushBufferEv");\nextern "C" long psp_stub_59(...){return 0;}\nextern "C" long psp_stub_60(...) __asm__("_ZN2rw7Charset9setColorsEPKNS_4RGBAES3_");\nextern "C" long psp_stub_60(...){return 0;}\nextern "C" long psp_stub_61(...) __asm__("_ZN2rw7Charset6createEPKNS_4RGBAES3_");\nextern "C" long psp_stub_61(...){return 0;}\nextern "C" long psp_stub_62(...) __asm__("_ZN2rw7Charset7destroyEv");\nextern "C" long psp_stub_62(...){return 0;}\nextern "C" long psp_stub_63(...) __asm__("_Z15IsForegroundAppv");\nextern "C" long psp_stub_63(...){return 0;}\nextern "C" long psp_stub_64(...) __asm__("_Z10CapturePadi");\nextern "C" long psp_stub_64(...){return 0;}\nextern "C" long psp_stub_65(...) __asm__("_Z21_psGetUserFilesFolderv");\nextern "C" long psp_stub_65(...){return 0;}\nextern "C" long psp_stub_66(...) __asm__("_ZL12openim2d_uv2v");\nextern "C" long psp_stub_66(...){return 0;}\nextern "C" long psp_stub_67(...) __asm__("_ZL13closeim2d_uv2v");\nextern "C" long psp_stub_67(...){return 0;}\nextern "C" long psp_stub_68(...) __asm__("_ZL26RenderIndexedPrimitive_UV215RwPrimitiveTypeP13Im2DVertexUV2lPtl");\nextern "C" long psp_stub_68(...){return 0;}\nextern "C" int psp_game_state __asm__("gGameState"); int psp_game_state=0;\nextern "C" int psp_osver __asm__("_dwOperatingSystemVersion"); int psp_osver=0;\nextern "C" int psp_memavail __asm__("_dwMemAvailPhys"); int psp_memavail=0;\nnamespace rw { namespace psp2 { Device renderdevice; } }\n\n
extern "C" long extra_stub_1000(...) __asm__("_InputTranslateShiftKeyUpDown");
extern "C" long extra_stub_1000(...){return 0;}

extern "C" long extra_stub_1001(...) __asm__("_InputMouseNeedsExclusive");
extern "C" long extra_stub_1001(...){return 0;}

extern "C" long extra_stub_1002(...) __asm__("_InputShutdownMouse");
extern "C" long extra_stub_1002(...){return 0;}

extern "C" long extra_stub_1003(...) __asm__("_InputInitialiseMouse");
extern "C" long extra_stub_1003(...){return 0;}

extern "C" long extra_stub_1004(...) __asm__("HandleExit");
extern "C" long extra_stub_1004(...){return 0;}

extern "C" long extra_stub_1005(...) __asm__("_psGetVideoModeList");
extern "C" long extra_stub_1005(...){return 0;}

extern "C" long extra_stub_1006(...) __asm__("_psGetNumVideModes");
extern "C" long extra_stub_1006(...){return 0;}

extern "C" long extra_stub_1007(...) __asm__("_psSelectScreenVM");
extern "C" long extra_stub_1007(...){return 0;}

extern "C" long extra_stub_1008(...) __asm__("AddToQueue");
extern "C" long extra_stub_1008(...){return 0;}

extern "C" long extra_stub_1009(...) __asm__("CdStreamSync");
extern "C" long extra_stub_1009(...){return 0;}

extern "C" long extra_stub_1010(...) __asm__("CdStreamInit");
extern "C" long extra_stub_1010(...){return 0;}

extern "C" long extra_stub_1011(...) __asm__("CdStreamGetImageName");
extern "C" long extra_stub_1011(...){return 0;}

extern "C" long extra_stub_1012(...) __asm__("CdStreamRead");
extern "C" long extra_stub_1012(...){return 0;}

extern "C" long extra_stub_1013(...) __asm__("CdStreamGetStatus");
extern "C" long extra_stub_1013(...){return 0;}


extern "C" int psp_native_raster_offset __asm__("_ZN2rw4xbox18nativeRasterOffsetE"); int psp_native_raster_offset=0;
extern "C" int psp_num_blend_insts __asm__("_ZN11WorldRender13numBlendInstsE"); int psp_num_blend_insts=0;
