# Upstream engine source dependency

## Primary engine candidate

- Repository: https://github.com/mrxenginner/reVC
- Branch used by the existing host baseline: `miami`
- Purpose: reverse-engineered Grand Theft Auto: Vice City engine source.
- The Linux host baseline has built successfully in [GitHub Actions run 37145378347](https://github.com/Frolicworks1/Gtavcportpsp/actions/runs/37145378347).
- **That success is host-only.** It does not mean the engine builds for PSP, and no upstream engine code is currently linked into the PSP EBOOT.
- Before redistributing source, review the exact revision's license, submodule licenses, and any required notices. Keep all proprietary game data out of this repository.

## Secondary reference, not a PSP target

- Repository: https://github.com/re3fork/re3-miami-vita
- Audited revision: `5de9de0d8072c3644f6cb961160b0fd7bfb067fb`
- This is a PlayStation Vita adaptation, not a PSP build. Its `PSP2` defines, `arm-vita-eabi` toolchain, `vita2d`/Vita GL libraries, Vita system stubs, and Vita audio/controller APIs are not usable on PSP MIPS Allegrex.
- It may provide architectural reference for game initialization and engine/platform separation, but must not be copied into a PSP EBOOT unchanged.

## Current PSP implementation status

The checked-in `GTAVC-WIP9-cloud-build.zip` contains a small PSP bootstrap/platform scaffold (file probing, IMG archive indexing, basic GU drawing, input/audio/platform stubs). Its `gtavc_revc_psp_frame()` explicitly draws a bootstrap frame; it does **not** call the actual Vice City game loop. The separate `psp-platform-smoke/main.c` is a renderer diagnostic.

## Required integration work

1. Pin an exact revision of the primary reVC source and record its license/submodule requirements.
2. Add the engine source as a source dependency (not as an executable copied from an EBOOT).
3. Cross-compile the engine with PSP MIPS Allegrex tools and systematically port compile blockers; a host build is not evidence of PSP compatibility.
4. Replace or port the renderer backend to PSP GU/GE, including RenderWare device, raster, texture, camera, geometry, and state handling.
5. Port OS/platform seams for timing, filesystem, memory, threading, controller input, audio, save data, and streaming.
6. Supply legally obtained Vice City game data separately and verify archive/model/texture loading.
7. Test an actual Vice City scene in PPSSPP or on PSP with logs and screenshots before calling the build playable.

## GTA III PSP EBOOT reference

The user-provided GTA III EBOOT was inspected statically. Strings suggested a PSP-native RenderWare/GU backend and PSP-specific streaming/input/save code. Those strings are architectural clues only; they do not establish runtime behavior or compatibility with Vice City. Do not copy executable code or proprietary assets from the EBOOT.
