
#include <cmath>

#include "CameraAnim.h"


#ifndef MM_TO_INCH
#define MM_TO_INCH    0.03937
#endif



CameraAnim::CameraAnim() : frames(0), start(0), end(0)
{
}


CameraAnim::~CameraAnim()
{
    if (frames)
        delete [] frames;
}


Camera * CameraAnim::FindCamera(int frame)
{
    if (this->frames == 0)
        return 0;

    // check range
    if (frame < this->start)
        frame = this->start;
    else if (frame > this->end)
        frame = this->end;
    
    Camera *c = &this->frames[frame - this->start];
    return c;
}


Camera * CameraAnim::GetList()
{
    if (this->frames == 0)
        this->frames = new Camera[this->end - this->start + 1];
    return this->frames;
}


void CameraAnim::Print()
{
    int i;
    for (i = 0; i < (this->end - this->start + 1); i++)
    {
        Camera *cam = &this->frames[i];
        cam->Print();
    }
}


void Camera::Print()
{
    printf("camera\n");
    printf(" focal_length %g\n", focal_length);
    printf(" fstop %g\n", fstop);
    printf(" focus_distance %g\n", focus_distance);
    printf(" near_clip_plane %g\n", near_clip_plane);
    printf(" far_clip_plane %g\n", far_clip_plane);
}




