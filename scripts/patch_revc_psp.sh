# Make Engine::open select the PSP device instead of falling back to null.
if ! grep -q 'engine->device = psp::renderdevice' "$ENG"; then
  sed -i '/#ifdef RW_PS2/i\
#ifdef RW_PSP\
\tengine->device = psp::renderdevice;\
#elif defined(RW_PS2)' "$ENG"
fi

# reVC's PC controller configuration uses DIJOYSTATE2 unless the GL3 path
# supplies its own JoyState. PSP also needs the portable JoyState shape; the
# actual PSP state population will be added in the input backend next.
CTRL="upstream-revc/src/core/ControllerConfig.h"
CTRLC="upstream-revc/src/core/ControllerConfig.cpp"

if [ -f "$CTRL" ]; then
  sed -i     -e 's/#ifdef RW_GL3/#if defined(RW_GL3) || defined(RW_PSP)/g'     "$CTRL"
fi

if [ -f "$CTRLC" ]; then
  sed -i     -e 's/#elif defined RW_GL3/#elif defined RW_GL3 || defined(RW_PSP)/g'     -e 's/#elif defined RW_GL3/#elif defined(RW_GL3) || defined(RW_PSP)/g'     "$CTRLC"
fi
