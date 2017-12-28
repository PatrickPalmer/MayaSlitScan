
import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
import maya.OpenMayaRender as OpenMayaRender
import maya.OpenMayaUI as OpenMayaUI

import math
import sys

kPluginNodeTypeName = "LensLogNode"

LensLogNodeId = OpenMaya.MTypeId(0x87010)
glRenderer = OpenMayaRender.MHardwareRenderer.theRenderer()
glFT = glRenderer.glFunctionTable()



class Ray:

    def __init__(self):
        self.origin = OpenMaya.MPoint(0,0,0,1)
        self.dir = OpenMaya.MVector(0,0,0)
        self.dist = 1.0

    def unserialize(self, str):
        list = str.split(",")
        if len(list) == 12:
            self.x = float(list[0])
            self.y = float(list[1])
            self.origin = OpenMaya.MPoint(float(list[2]), float(list[3]), float(list[4]), 1.0)
            self.dir = OpenMaya.MVector(float(list[5]), float(list[6]), float(list[7]))
            self.dist = float(list[8])
            self.color = OpenMaya.MColor(float(list[9]), float(list[10]), float(list[11]))

    def draw(self, column):
        render = False

        # requested column is -1, render all rays
        if column == -1.0:
            render = True
        elif column == self.x:
            render = True
        
        # ray has distance 
        if render and self.dist != 0.0:
            glFT.glBegin(OpenMayaRender.MGL_LINES)

            p1 = self.origin
            p2 = self.origin + (self.dir * self.dist)
    
            glFT.glVertex3f(p1.x, p1.y, p1.z)
            glFT.glVertex3f(p2.x, p2.y, p2.z) 

            glFT.glEnd()

    def boundingBox(self):
        p1 = self.origin
        p2 = self.origin + (self.dir * self.dist)
        bbox = OpenMaya.MBoundingBox( p1, p2 )
        return bbox





class LensLogNode(OpenMayaMPx.MPxLocatorNode):
    useInput = OpenMaya.MObject()
    input = OpenMaya.MObject()
    
    def __init__(self):
        OpenMayaMPx.MPxLocatorNode.__init__(self)
        self.rays = []
        self.readlog()
        
    def compute(self, plug, dataBlock):
        return OpenMaya.kUnknownParameter

    def draw(self, view, path, style, status):
        thisNode = self.thisMObject()

        # use input time
        useInput = OpenMaya.MPlug(thisNode, self.useInput).asBool()

        # input time
        plug = OpenMaya.MPlug(thisNode, self.input)
        inputVal = plug.asMTime()
        dtime = inputVal.value()

        # render which columns, default to all
        column = -1.0
        if useInput:
            column = dtime

        view.beginGL()

        if style == OpenMayaUI.M3dView.kFlatShaded or style == OpenMayaUI.M3dView.kGouraudShaded:
            glFT.glPushAttrib(OpenMayaRender.MGL_CURRENT_BIT)
            
            if status == OpenMayaUI.M3dView.kActive:
                view.setDrawColor( 13, OpenMayaUI.M3dView.kActiveColors )
            else:
                view.setDrawColor( 13, OpenMayaUI.M3dView.kDormantColors )

        # draw the rays
        for ray in self.rays:
            ray.draw(column)

        view.endGL()

    def isBounded(self):
        return True

    def boundingBox(self):
        return self.bbox

    def readlog(self):
        self.bbox = OpenMaya.MBoundingBox()
        file = "/Users/patrick/test.log"
        inf = open(file)
           for x in inf.readlines():
            if len(x) > 0:
                inst = Ray()
                inst.unserialize(x)
                self.bbox.expand(inst.boundingBox())
                self.rays.append(inst)
        inf.close()

# creator
def nodeCreator():
    return OpenMayaMPx.asMPxPtr( LensLogNode() )

# initializer
def nodeInitializer():
    unitFn = OpenMaya.MFnUnitAttribute()
    LensLogNode.input = unitFn.create("input", "in", OpenMaya.MFnUnitAttribute.kTime)
    unitFn.setDefault(0.0)
    LensLogNode.addAttribute( LensLogNode.input )

    numFn = OpenMaya.MFnNumericAttribute()
    LensLogNode.useInput = numFn.create("useInput", "uin", OpenMaya.MFnNumericData.kBoolean, False)
    LensLogNode.addAttribute( LensLogNode.useInput )




# initialize the script plug-in
def initializePlugin(mobject):
    mplugin = OpenMayaMPx.MFnPlugin(mobject)
    try:
        mplugin.registerNode( kPluginNodeTypeName, LensLogNodeId, nodeCreator, nodeInitializer, OpenMayaMPx.MPxNode.kLocatorNode )
    except:
        sys.stderr.write( "Failed to register node: %s" % kPluginNodeTypeName )
        raise

# uninitialize the script plug-in
def uninitializePlugin(mobject):
    mplugin = OpenMayaMPx.MFnPlugin(mobject)
    try:
        mplugin.deregisterNode( LensLogNodeId )
    except:
        sys.stderr.write( "Failed to deregister node: %s" % kPluginNodeTypeName )
        raise
    
