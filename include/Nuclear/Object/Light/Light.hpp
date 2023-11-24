/*#ifndef LIGHT_CLASS_H
#define LIGHT_CLASS_H

#include"Object.h"

class pointLight: public Object{
public:
    pointLight(glm::vec3 position, glm::vec4 color);
};

class direcLight: public Object{
public:
    direcLight(glm::vec3 position, glm::vec3 rotation, glm::vec4 color);
};

class spotLight: public Object{
public:
    spotLight(glm::vec3 position, glm::vec3 rotation, glm::vec4 color, float outerCone, float innerCone);
};

#endif*/