
"""

Slit-Scan for Autodesk Maya and Mental Ray
Patrick Andrew Palmer
Version 1.1

usage in the Maya Python Scripts Panel:

    import slitscanCache
    slitscanCache.build('/Users/patrick/cameraAnim.bin')

"""



import maya.cmds as mc
import struct


def build(filename):

    # get the currently selected list
    sel = mc.ls(selection=True)
    if len(sel) == 0:
        print "please select a camera first"
        return False

    obj = sel[0]

    # check if camera
    if not mc.objectType(obj, isType="camera"):
        children = mc.listRelatives(obj)
        if len(children) == 0:
            print "please select a camera first"
            return False
        obj = children[0]
        if not mc.objectType(obj, isType="camera"):
            print "please select a camera first"
            return False

    # we now know that 'obj' is a cameraShape
    parents = mc.listRelatives(obj, parent=True)
    parent = parents[0]

    # get start/end time
    start = int(mc.playbackOptions(query=True, minTime=True))
    end = int(mc.playbackOptions(query=True, maxTime=True))

    try:
        # open the file
        outf = open(filename, "wb")

        # magic, version
        outf.write(struct.pack("<2i", 226372, 1))
        
        # write start and end frames
        outf.write(struct.pack("<2i", start, end))

        # save the current time
        ctime = mc.currentTime(query=True)

        # go through each frame
        for frame in range(start, (end+1)):

            mc.currentTime(frame)

            # write camera information in specific order
            outf.write(struct.pack("<d", (mc.getAttr(obj+'.focalLength'))))
            outf.write(struct.pack("<d", (mc.getAttr(obj+'.fStop'))))
            outf.write(struct.pack("<d", (mc.getAttr(obj+'.fd'))))
            outf.write(struct.pack("<d", (mc.getAttr(obj+'.ncp'))))
            outf.write(struct.pack("<d", (mc.getAttr(obj+'.fcp'))))
            outf.write(struct.pack("<i", (mc.getAttr(obj+'.orthographic'))))
            outf.write(struct.pack("<d", (mc.getAttr(obj+'.orthographicWidth'))))
            outf.write(struct.pack("<d", (mc.getAttr(obj+'.hfa'))))
            outf.write(struct.pack("<d", (mc.getAttr(obj+'.vfa'))))
            outf.write(struct.pack("<d", (mc.getAttr(obj+'.hfo'))))
            outf.write(struct.pack("<d", (mc.getAttr(obj+'.vfo'))))
            outf.write(struct.pack("<d", (mc.getAttr(obj+'.lsr'))))
            outf.write(struct.pack("<i", (mc.getAttr(obj+'.ff'))))
            outf.write(struct.pack("<d", (mc.getAttr(obj+'.ffo'))))
            outf.write(struct.pack("<d", (mc.getAttr(obj+'.cs'))))

            # eye
            pos = mc.xform(parent, query=True, ws=True, translation=True)
            outf.write(struct.pack("<3d", pos[0], pos[1], pos[2]))

            # center of interest    
            coi = mc.camera(obj, query=True, wci=True);
            outf.write(struct.pack("<3d", coi[0], coi[1], coi[2]))

            # up    
            up = mc.camera(obj, query=True, wup=True);
            outf.write(struct.pack("<3d", up[0], up[1], up[2]))

            # matrix
            mat = mc.xform(parent, query=True, ws=True, m=True)
            for i in range(0, 16):
                outf.write(struct.pack("<d", mat[i]))
    
        # restore current time
        mc.currentTime(ctime)

        # close the file
        outf.close()

    except:
        print "unable to open file"

    return True

