#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "model.h"

class LightSource
{
public:
	LightSource();
    Shader Shader{ "shaders\\lightsourcevert.shader", "shaders\\lightsourcefrag.shader" };
    Model Model{ "models\\cube\\cube.gltf" };
private:
    //TODO: unhardcode this
};

#endif