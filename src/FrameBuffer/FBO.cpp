#include "Nuclear/FrameBuffer/FBO.hpp"

// Frame Buffer Object Class
FBO::FBO()
{
    // Create the Frame Buffer Object
    glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

// Bind the Frame Buffer Object
void FBO::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

// Delete the Frame Buffer Object
void FBO::Delete()
{
    glDeleteFramebuffers(1, &ID);
}
