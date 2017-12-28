


#include <maya/MFnPlugin.h>
#include <maya/MMessage.h>
#include <maya/MGlobal.h>


#include "InteractiveRender.h"


MStatus initializePlugin( MObject obj )            
{                                                            
    MFnPlugin    plugin( obj, "Patrick Palmer", "9.0" );    
    MStatus      stat;                                        
    stat = plugin.registerCommand(InteractiveRender::cmdName,
                                  InteractiveRender::creator,
                                  InteractiveRender::newSyntax);    
    if ( !stat )                                                
        stat.perror( "registerCommand" );                            
    return stat;                                                
}                                                                


MStatus uninitializePlugin( MObject obj )                        
{                                                                
    MFnPlugin    plugin( obj );                                    
    MStatus      stat;                                            
    stat = plugin.deregisterCommand( InteractiveRender::cmdName );
    if ( !stat )                                    
        stat.perror( "deregisterCommand" );            
    return stat;                                    
}



