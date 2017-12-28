
#include <stdlib.h> 

#include <maya/MGlobal.h>
#include <maya/MIOStream.h> 
#include <maya/MPlug.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>

#include <maya/MFnNumericAttribute.h>

#include <maya/MFloatPoint.h>

#include <maya/MRenderData.h>
#include <maya/MRenderShadowData.h>
#include <maya/MRenderUtil.h>
#include <maya/MRenderCallback.h>

#include "MayaRendererAccess.h"


MayaRendererAccess::MayaRendererAccess()
{
}


MayaRendererAccess::~MayaRendererAccess()
{
}



void MayaRendererAccess::install()
{
    MRenderCallback::addCallback( this, 255 );
}


void MayaRendererAccess::remove()
{
    MRenderCallback::removeCallback( this );
}


void MayaRendererAccess::SetImageBuffer(ImageBuffer *buf)
{
    this->buffer = buf;
}


// To get 3 float values from the node attribute
//
static void getFloat3(MObject node, MObject attr, MFloatPoint & val)
{
    // Get the attr to use
    //
    MObject object;
    MStatus status;
    MPlug    plug(node, attr);

    CHECK_MSTATUS(plug.getValue(object));

    MFnNumericData data(object, &status);
    CHECK_MSTATUS(status);

    CHECK_MSTATUS(data.getData(val[0], val[1], val[2]));
}


bool MayaRendererAccess::postProcessCallback( const MRenderData &data ) 
{
    this->buffer->ImageCopy(data);
    
    return 0;
}



