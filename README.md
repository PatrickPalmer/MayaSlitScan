# Slit Scan Rendering for Autodesk Maya and Mental Ray
Patrick Palmer
Version 1.1, July 1, 2008


## Introduction

Slit-Scan for Autodesk Maya is an implementation of the traditional film technique of slit scan photography.    

The basic concept of traditional slit-scan photography is that an open vertical slit is placed in front of the camera.   When the shutter is open, slit area allows light to bathe a small portion of a single frame of film.   Once the area has been exposed, the shutter is then closed.  The slit slides across the field of view, repeating the process of exposing different parts of the film.   By changing the scene between each slit exposure, extraordinary psychedelic images can be created.

This technique is possible because film is light sensitive from the time it is manufactured until it is developed.  Only areas of the film that are exposed to light register a latent image.    Unexposed areas stay unexposed for long periods of time. 

This solution implements the slit-scan technique by sliding the camera around while rendering a single frame within Mental Ray. 

## Implementation

The design approach was to optimize the rendering time of a single frame in Mental Ray with a static scene and a single animated camera.   This is different than scenes with animated objects with an animated camera.  Because of this focus, impact on rendering times is very small using this package.   Ray tracing is required for all scenes.


 
## Installation

Here are the installation instructions for the Slit-Scan plug-in and scripts for Autodesk Maya (tested with Maya 2008, 2009).  

### Apple OSX

The Mental Ray plug-in must be installed within the Maya application.  Here are the steps:

1. Open up the /Applications/Autodesk/maya2009 directory.
2. Right click on the Maya.app to bring up a contextual menu.
3. Select the "Show Package Contents" menu option.
4. Traverse into the Contents directory.
5. Place the render_SlitScanLens.xpm into the icons directory.
6. Traverse into the mentalray directory.
7. Place the slitscanMR.mi into the include directory.
8. Place the slitscanMR.so into the lib directory.

The slitscanCache.py should be placed into your ~/Library/Preferences/Autodesk/maya/2009/scripts directory.


### Microsoft Windows

To install the Mental Ray plug-in, traverse to the Autodesk Maya 2009 installation directory (usually C:\Program Files\Autodesk\Maya2009).  Install the slitscanMR.mi in the mentalray\include directory and the slitscanMR.dll into the mentalray\lib directory.  Make sure to select the correct dll to match the installed Maya architecture (32- or 64-bit).  Place the render_SlitScanLens.xpm into the icons directory.

Place the slitscanCache.py into My Documents\maya\2009\scripts.  You may need to add the scripts directory to the PYTHONPATH environment variable.  The Maya.env file is one place the PYTHONPATH environment variable can be defined.


 
## Usage

The Slit-Scan rendering is a two-stage process.  The first step is caching of the camera information in a data file.  Once the camera information is cached for the animation, the second stage is rendering with the Mental Ray Lens shader.

The first step is to animate the camera.  It is best to set the start frame to 1 and the end frame to be column with of the final image.   The step by frame count should be 1.  Below is an example of a camera animation with the start frame at 1 and the end frame at 1280.

![Camera Flight Path](/doc/camera-path.png)

Set the play back parameters to match the start and end frame number.

![Maya Timeline](/doc/timeline.png)
 	
Open up the script editor by pressing the button in the lower right hand corner or using the Window Menu.

![Maya Script Window Button](/doc/maya-script-button.png)
 
Once the script editor is open, set the language to Python.  The first command to run in the Python script editor is importing the function.  Then select the camera that will be cached.

![Maya Python Code](/doc/python-script.png)

Executing the function will build the Slit-Scan cache.  The playback time line will start to play back the animation from the start frame to the end frame.  The results of the cache will be written into the file specified in the Python function call.  The cache needs to be built each time the camera parameters or animation path changes.

Now that the cache has been built, attach Slit-Scan Mental Ray lens to a camera.  Select the camera to attach the lens and open up the Mental Ray frame layout within the camera shape attributes.

![Attribute Editor Camera](/doc/ae-camera.png) 

Attach a Mental Ray lens shader by pressing the Create button.  Select the "Slit Scan Lens" and the lens will be attached and selected.
 

The Slit-Scan Lens shader node has a single attribute which is the cache file name. 

![Slit Scan Shader Node](/doc/ae-slitscanlens.png) 

Now, everything is set up for rendering in Mental Ray.  Open up the render view, set the renderer to Mental Ray and render the camera that has the Slit-Scan Lens assigned.


## Technical Notes

The best results are usually created setting the camera start/stop frame range to be the same size as the resulting image width pixel count.  For example, if the resulting image resolution is 1280x720, set the start frame to 1 and the end frame to 1280.

The width of the slit is based on the ratio of cache start/stop frame range to render width size.   A 1:1 relationship will create a slit of 1 pixel wide.   For example, if the cache start/stop frame range is 640 and the image width is 1280, the slit width is 2 pixels wide.

Only the camera information is cached so nothing else needs to be in the scene.   Making all other objects in the scene invisible will increase the speed of the generation of the cache.

Cache is stored in little-endian and the plug-in currently only works on little-endian systems (Intel/AMD based machines).


