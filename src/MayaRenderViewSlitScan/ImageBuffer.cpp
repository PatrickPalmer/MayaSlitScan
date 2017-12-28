/*
 *  ImageBuffer.cpp
 *  SlitScan
 *
 *  Created by Patrick Palmer on 5/8/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <maya/MGlobal.h>

#include "ImageBuffer.h"


ImageBuffer::ImageBuffer() : pixels(0), offset(0)
{
}



ImageBuffer::~ImageBuffer()
{
    if (this->pixels)
        delete [] pixels;
}



RV_PIXEL *ImageBuffer::PixelBuffer()
{
    return this->pixels;
}


void ImageBuffer::Create(const int width, const int height)
{
    this->width = width;
    this->height = height;
    if (this->pixels)
        delete [] pixels;
    pixels = new RV_PIXEL[width*height];
    this->Default();
}


void ImageBuffer::Default()
{
    if (!this->pixels)
        return;
    
    RV_PIXEL white;
    white.r = 0.0f;
    white.g = 0.0f;
    white.b = 0.0f;
    white.a = 0.0f;
    
    for (int x = 0; x < this->width; x++)
        for (int y = 0; y < this->height; y++)
        {
            unsigned int index = x + (y*width);
            this->pixels[index] = white;
        }                      
}


void ImageBuffer::Frame(const double f)
{
    this->frame = f;
}

void ImageBuffer::SetFrameRange(const double s, const double e)
{
    this->start = s;
    this->end = e;
}


int ImageBuffer::SlitWidth()
{
    float w = this->width / (this->end - this->start + 1);
    
    return (int(w)+4);
}


int ImageBuffer::SlitOffset()
{
    return this->offset;
}


void ImageBuffer::ImageCopy(const MRenderData &data)
{
    short x, y;
    unsigned char *addr;
    RV_PIXEL *rvaddr;

if ( data.bytesPerChannel > 1 )
    MGlobal::displayWarning("bytes per channel > 1");
    
    // copy 
    for ( y = 0; y < data.ysize; y++)
        for (x = 0; x < data.xsize-2; x++)
        {
            addr = data.rgbaArr + ((data.xsize*y+x+2)*4*data.bytesPerChannel);
            rvaddr = this->pixels + (this->width * y + x + this->offset);
    
            if ( x == 0 || x == (data.xsize - 2 - 1) )
            {
                rvaddr->a = (rvaddr->a / 2) + (addr[0] / 2);
                rvaddr->b = (rvaddr->b / 2) + (addr[1] / 2);
                rvaddr->g = (rvaddr->g / 2) + (addr[2] / 2);
                rvaddr->r = (rvaddr->r / 2) + (addr[3] / 2);
            }
            else
            {
                rvaddr->a = addr[0];
                rvaddr->b = addr[1];
                rvaddr->g = addr[2];
                rvaddr->r = addr[3];
            }

        }
    
    // finished
    // add copied vertical line count to offset
    this->offset += this->SlitWidth() - 4;

}



