#ifndef FBT_CLASS_H
#define FBT_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Frame Buffer Object Class
class FBT
{
public:
    unsigned int ID;
    GLuint unit;

    // Create the Frame Buffer Object
    FBT(const unsigned int width, const unsigned int height, GLuint slot);

    // Bind the Frame Buffer Texture
    void Bind();
};

#endif