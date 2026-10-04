# PSP MIPS Allegrex cross-toolchain for reVC porting work.
# Compiler selection only: this does not make upstream reVC PSP-compatible.
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR mips)

if(NOT DEFINED ENV{PSPDEV})
  message(FATAL_ERROR "PSPDEV must point to the installed PSPDEV toolchain")
endif()

set(PSPDEV "$ENV{PSPDEV}")
set(CMAKE_C_COMPILER   "${PSPDEV}/bin/psp-gcc")
set(CMAKE_CXX_COMPILER "${PSPDEV}/bin/psp-g++")
set(CMAKE_AR           "${PSPDEV}/bin/psp-ar")
set(CMAKE_RANLIB       "${PSPDEV}/bin/psp-ranlib")
set(CMAKE_STRIP        "${PSPDEV}/bin/psp-strip")

set(CMAKE_FIND_ROOT_PATH "${PSPDEV}/psp")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
