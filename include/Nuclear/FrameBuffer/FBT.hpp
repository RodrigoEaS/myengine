#ifndef FBT_CLASS_HPP
#define FBT_CLASS_HPP

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