#ifndef FBO_CLASS_H
#define FBO_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Frame Buffer Object Class
class FBO
{   
public:
    // Reference of the Frame Buffer Object
    unsigned int ID;
    
    // Create the Frame Buffer Object
    FBO();
    // Bind the Frame Buffer Object
    void Bind();
    // Delete the Frame Buffer Object
    void Delete();
};

#endif