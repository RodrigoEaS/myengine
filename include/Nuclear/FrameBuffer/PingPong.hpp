#define PING_PONG_CLASS_HPP
#ifdef PING_PONG_CLASS_HPP

#include"Nuclear/FrameBuffer/FBO.hpp"
#include"Nuclear/FrameBuffer/FBT.hpp"

class PingPongBuffer
{
public:
    FBO FBO();
    FBT FBT();

    PingPongBuffer();
};

#endif