
#ifndef _CAMERAANIM_H
#define _CAMERAANIM_H


#include <cstdio>
#include <cmath>
#include <list>


struct Header
{
    int magic;
    int version;
    int start;
    int end;

    inline bool Match() const;
};


struct Camera
{
    double focal_length;
    double fstop;
    double focus_distance;
    double near_clip_plane;
    double far_clip_plane;
    int orthographic;
    double orthographic_width;
    double vertical_film_aperture;
    double horizontal_film_aperture;
    double vertical_film_offset;
    double horizontal_film_offset;
    double squeeze_ratio;
    int film_fit;
    double film_fit_offset;
    double camera_scale;
    double eye[3];
    double coi[3];
    double up[3];
    double mat[16];

    void Print();
};


inline bool Header::Match() const
{
    return (this->magic == 226372);
}


class CameraAnim
{
  public:
    int start;
    int end;

    Camera *frames;

    CameraAnim();
    ~CameraAnim();
    Camera * FindCamera(int frame);
    Camera * GetList();
    void Print();
};



#endif



