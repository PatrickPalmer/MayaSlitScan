
#ifndef _MAYARENDERACCESS_H
#define _MAYARENDERACCESS_H


#include <maya/MRenderData.h>
#include <maya/MRenderShadowData.h>
#include <maya/MRenderUtil.h>
#include <maya/MRenderCallback.h>

#include "ImageBuffer.h"



class MayaRendererAccess : public MRenderCallback 
{
    public:
    MayaRendererAccess();
    ~MayaRendererAccess();
    
    void install();
    void remove();
    
    void setObj(MObject &obj) { nodeObj = obj;}
    void SetImageBuffer(ImageBuffer *);
    
    virtual bool postProcessCallback( const MRenderData &data );

    private:
    MObject nodeObj;
    ImageBuffer *buffer;
        
};


#endif

