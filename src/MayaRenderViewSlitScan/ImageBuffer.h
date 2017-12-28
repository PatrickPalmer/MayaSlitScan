/*
 *  ImageBuffer.h
 *  SlitScan
 *
 *  Created by Patrick Palmer on 5/8/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _IMAGEBUFFER_H
#define _IMAGEBUFFER_H



#include <maya/MRenderView.h>
#include <maya/M3dView.h>
#include <maya/MRenderData.h>



class ImageBuffer
{
  public:
    ImageBuffer();
    ~ImageBuffer();
    
    void Create(const int, const int);

    void Default();
    int SlitWidth();
    void SetFrameRange(const double, const double);
    void Frame(const double);
    void ImageCopy(const MRenderData &);
    RV_PIXEL *PixelBuffer();
    
    
  protected:    
    RV_PIXEL *pixels;
    int width;
    int height;
    int offset;
    double frame;
    double start, end;
    
    int SlitOffset();

};


#endif
