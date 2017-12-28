
#include <math.h>

#include <maya/MGlobal.h>
#include <maya/MIOStream.h>
#include <maya/MRenderView.h>
#include <maya/M3dView.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MAnimControl.h>

#include "InteractiveRender.h"
#include "ImageBuffer.h"



static const char * kVerbose                    = "-v";
static const char * kVerboseLong                = "-verbose";
static const char * kDoNotClearBackground        = "-b";
static const char * kDoNotClearBackgroundLong    = "-background";
static const char * kRadius                        = "-r";
static const char * kRadiusLong                    = "-radius";
static const char * kSizeX                        = "-sx";
static const char * kSizeXLong                    = "-sizeX";
static const char * kSizeY                        = "-sy";
static const char * kSizeYLong                    = "-sizeY";
static const char * kSizeTileX                    = "-tx";
static const char * kSizeTileXLong                = "-sizeTileX";
static const char * kSizeTileY                    = "-ty";
static const char * kSizeTileYLong                = "-sizeTileY";
static const char * kNumberLoops                = "-nl";
static const char * kNumberLoopsLong            = "-numberLoops";
static const char * kImmediateRefresh            = "-ir";
static const char * kImmediateRefreshLong        = "-immediateRefresh";
static const char * kFullRefresh                = "-fr";
static const char * kFullRefreshLong            = "-fullRefresh";

const char * InteractiveRender::cmdName = "SlitScanInteractiveRenderView";


InteractiveRender::InteractiveRender()
{
    this->mrAccess = new MayaRendererAccess;
    this->buffer = new ImageBuffer;
}


InteractiveRender::~InteractiveRender()
{
    delete this->mrAccess;
    delete this->buffer;
}




void* InteractiveRender::creator()                    
{
    return new InteractiveRender;
}                                                    


MSyntax InteractiveRender::newSyntax()
{
    MStatus status;
    MSyntax syntax;

    syntax.addFlag(kDoNotClearBackground, kDoNotClearBackgroundLong);
    CHECK_MSTATUS_AND_RETURN(status, syntax);

    syntax.addFlag(kVerbose, kVerboseLong);
    CHECK_MSTATUS_AND_RETURN(status, syntax);

    syntax.addFlag(kImmediateRefresh, kImmediateRefreshLong);
    CHECK_MSTATUS_AND_RETURN(status, syntax);

    syntax.addFlag(kFullRefresh, kFullRefreshLong);
    CHECK_MSTATUS_AND_RETURN(status, syntax);

    syntax.addFlag(kRadius, kRadiusLong, MSyntax::kDouble);
    CHECK_MSTATUS_AND_RETURN(status, syntax);

    syntax.addFlag(kSizeX, kSizeXLong, MSyntax::kLong);
    CHECK_MSTATUS_AND_RETURN(status, syntax);

    syntax.addFlag(kSizeY, kSizeYLong, MSyntax::kLong);
    CHECK_MSTATUS_AND_RETURN(status, syntax);

    syntax.addFlag(kSizeTileX, kSizeTileXLong, MSyntax::kLong);
    CHECK_MSTATUS_AND_RETURN(status, syntax);

    syntax.addFlag(kSizeTileY, kSizeTileYLong, MSyntax::kLong);
    CHECK_MSTATUS_AND_RETURN(status, syntax);

    syntax.addFlag(kNumberLoops, kNumberLoopsLong, MSyntax::kLong);
    CHECK_MSTATUS_AND_RETURN(status, syntax);

    return syntax;
}

//
// Description:
//        Read the values of the additionnal flags for this command.
//
MStatus InteractiveRender::parseSyntax (MArgDatabase &argData)
{
    // Get the flag values, otherwise the default values are used.
    doNotClearBackground = argData.isFlagSet( kDoNotClearBackground );
    verbose = argData.isFlagSet( kVerbose );
    fullRefresh = argData.isFlagSet( kFullRefresh );
    immediateRefresh = argData.isFlagSet( kImmediateRefresh );

    radius = 50.;                            // pattern frequency, in pixels
    if (argData.isFlagSet( kRadius ))
        argData.getFlagArgument(kRadius, 0, radius);

    size[0] = 640;
    size[1] = 480;
    if (argData.isFlagSet( kSizeX ))
        argData.getFlagArgument(kSizeX, 0, size[0]);
    if (argData.isFlagSet( kSizeY ))
        argData.getFlagArgument(kSizeY, 0, size[1]);

    tileSize[0] = 16;
    tileSize[1] = 16;
    if (argData.isFlagSet( kSizeTileX ))
        argData.getFlagArgument(kSizeTileX, 0, tileSize[0]);
    if (argData.isFlagSet( kSizeTileY ))
        argData.getFlagArgument(kSizeTileY, 0, tileSize[1]);

    numberLoops = 10;
    if (argData.isFlagSet( kNumberLoops ))
        argData.getFlagArgument(kNumberLoops, 0, numberLoops);

    return MS::kSuccess;
}



RV_PIXEL InteractiveRender::evaluate(int x, int y) const
//
//    Description:
//        Generates a simple procedural circular pattern to be sent to the 
//        Render View.
//
//    Arguments:
//        x, y - coordinates in the current tile (the pattern is centred 
//               around (0,0) ).
//
//    Return Value:
//        An RV_PIXEL structure containing the colour of pixel (x,y).
//
{
    double distance = sqrt(double((x*x) + (y*y))) / radius;
    float percent = (float)(cos(distance*2*3.1415927)/2.+.5);

    RV_PIXEL pixel;
    pixel.r = color1.r * percent + color2.r * (1-percent);
    pixel.g = color1.g * percent + color2.g * (1-percent);
    pixel.b = color1.b * percent + color2.b * (1-percent);
    pixel.a = 255.0f;

    return pixel;
}

MStatus InteractiveRender::doIt( const MArgList& args )
//
//    Description:
//        Implements the MEL InteractiveRender command. This command
//        Draws a 640x480 tiled pattern of circles into Maya's Render
//        View window.
//
//    Arguments:
//        args - The argument list that was passed to the command from MEL.
//
//    Return Value:
//        MS::kSuccess - command succeeded
//        MS::kFailure - command failed (returning this value will cause the 
//                     MEL script that is being run to terminate unless the
//                     error is caught using a "catch" statement.
//
{
    MStatus stat = MS::kSuccess;

    // Check if the render view exists. It should always exist, unless
    // Maya is running in batch mode.
    //
    if (!MRenderView::doesRenderEditorExist())
    {
        displayError( 
            "Cannot InteractiveRender in batch render mode.\n"
            "Run in interactive mode, so that the render editor exists." );
        return MS::kFailure;
    }
    
    // get optional flags
    MArgDatabase argData( syntax(), args );
    parseSyntax( argData );

    double start = MAnimControl::minTime().value();
    double end = MAnimControl::maxTime().value();
    double frame;
    
    this->mrAccess->install();
    this->buffer->Create(size[0], size[1]);
    this->buffer->SetFrameRange(start, end);
    this->mrAccess->SetImageBuffer(this->buffer);
    
    MTime ctime = MAnimControl::currentTime();
    
    // step through each frame
    for (frame = start; frame <= end; frame++)
    {
        Print();
        this->buffer->Frame(frame);
        int rw = this->buffer->SlitWidth();
        MAnimControl::setCurrentTime(MTime(frame));
        MString cmd = MString("render -x ") + rw + " -y " + size[1] + " testCam";
        MString res = MGlobal::executeCommandStringResult(cmd);
        MGlobal::displayInfo(MString("slitscan: (")+frame+") "+cmd+" "+res);
    }
    
    MAnimControl::setCurrentTime(ctime);
    
    this->mrAccess->remove();
        
    // We'll render a 640x480 image.  Tell the Render View to get ready
    // to receive 640x480 pixels of data.
    //
    unsigned int image_width = size[0], image_height = size[1];
    if (MRenderView::startRender( image_width, image_height, 
                                  doNotClearBackground, 
                                  immediateRefresh) != MS::kSuccess)
    {
        displayError("InteractiveRender: error occured in startRender.");
        return MS::kFailure;
    }

    unsigned int min_x = 0;
    unsigned int max_x = size[0]-1;
    unsigned int min_y = 0;
    unsigned int max_y = size[1]-1;
    
    RV_PIXEL *pixels = this->buffer->PixelBuffer();
    
    if (MRenderView::updatePixels(min_x, max_x, min_y, max_y, pixels) != MS::kSuccess)
    {
        displayError( "InteractiveRender: error occured in updatePixels." );
        delete [] pixels;
        return MS::kFailure;
    }

    // Inform the Render View that we have completed rendering the entire image.
    //
    if (MRenderView::endRender() != MS::kSuccess)
    {
        displayError( "InteractiveRender: error occured in endRender." );
        return MS::kFailure;
    }

    displayError( "InteractiveRender completed." );
    return stat;
}


void InteractiveRender::Print() 
{
    MGlobal::displayInfo(MString("slitscan: size ")+size[0]+" "+size[1]);
    MGlobal::displayInfo(MString("slitscan: tilesize ")+tileSize[0]+" "+tileSize[1]);
    MGlobal::displayInfo(MString("slitscan: doNotClearBackground ")+doNotClearBackground);
    MGlobal::displayInfo("slitscan: full refresh "+fullRefresh);

}


