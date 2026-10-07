#pragma once

#include "../rwengine.h"

namespace rw { namespace psp {

struct Im2DVertex {
    float32 x, y, z, w;
    uint8 r, g, b, a;
    float32 u, v;
    void setScreenX(float32 v){ x=v; }
    void setScreenY(float32 v){ y=v; }
    void setScreenZ(float32 v){ z=v; }
    void setCameraZ(float32 v){ w=v; }
    void setRecipCameraZ(float32 v){ w=1.0f/v; }
    void setColor(uint8 R,uint8 G,uint8 B,uint8 A){ r=R;g=G;b=B;a=A; }
    void setU(float32 V,float){ u=V; }
    void setV(float32 V,float){ v=V; }
    float getScreenX(){return x;} float getScreenY(){return y;}
    float getScreenZ(){return z;} float getCameraZ(){return w;}
    float getRecipCameraZ(){return 1.0f/w;}
    RGBA getColor(){return makeRGBA(r,g,b,a);}
    float getU(){return u;} float getV(){return v;}
};

struct Im3DVertex {
    V3d position;
    V3d normal;
    uint8 r,g,b,a;
    float32 u,v;
    void setX(float32 x){position.x=x;} void setY(float32 y){position.y=y;}
    void setZ(float32 z){position.z=z;}
    void setNormalX(float32 x){normal.x=x;} void setNormalY(float32 y){normal.y=y;}
    void setNormalZ(float32 z){normal.z=z;}
    void setColor(uint8 R,uint8 G,uint8 B,uint8 A){r=R;g=G;b=B;a=A;}
    void setU(float32 U){u=U;} void setV(float32 V){v=V;}
    float getX(){return position.x;} float getY(){return position.y;} float getZ(){return position.z;}
    float getNormalX(){return normal.x;} float getNormalY(){return normal.y;} float getNormalZ(){return normal.z;}
    RGBA getColor(){return makeRGBA(r,g,b,a);}
    float getU(){return u;} float getV(){return v;}
};

extern Device renderdevice;
void registerPlatformPlugins(void);

} }
