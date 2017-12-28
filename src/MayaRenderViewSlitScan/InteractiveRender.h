
#ifndef _INTERACTIVERENDER_H
#define _INTERACTIVERENDER_H


#include <maya/MPxCommand.h>
#include <maya/MRenderView.h>
#include <maya/M3dView.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>

#include "MayaRendererAccess.h"
#include "ImageBuffer.h"



class InteractiveRender : public MPxCommand 
{                            
public:                    
    InteractiveRender();
    ~InteractiveRender();

    virtual MStatus    doIt ( const MArgList& );
    
    static void*    creator();
    
    static MSyntax    newSyntax();
    MStatus parseSyntax (MArgDatabase &argData);

    static const char * cmdName;

    private:

    RV_PIXEL evaluate(int x, int y) const;

    bool fullRefresh;
    bool immediateRefresh;
    bool doNotClearBackground;
    bool verbose;
    double radius;
    unsigned int size[2];
    unsigned int tileSize[2];
    unsigned int numberLoops;
    RV_PIXEL color1;
    RV_PIXEL color2;
    
    MayaRendererAccess *mrAccess;
    ImageBuffer *buffer;
    
    void Print();
};


#endif




