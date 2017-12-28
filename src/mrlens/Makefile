

MAYABASE = /Applications/Autodesk/maya2009

# OSX Mental Ray build

MR_OPTIONS = -flat_namespace -undefined suppress -dynamiclib 
EXPAT_OBJS = ../expat/lib/xmlparse.o ../expat/lib/xmltok.o ../expat/lib/xmlrole.o


slitscanMR.so: slitscanMR.o parse.o CameraAnim.o
	g++ $(MR_OPTIONS) -o slitscanMR.so slitscanMR.o parse.o CameraAnim.o

slitscanMR.o: slitscanMR.cpp CameraAnim.h parse.h
	g++ -c -O3 -mtune=pentiumpro -fexpensive-optimizations -fforce-mem -finline-functions -funroll-loops -fomit-frame-pointer -frerun-cse-after-loop -fstrength-reduce -fforce-addr -fPIC -std=c++98 -dynamic -fno-common -DQMC -DMI_MODULE= -DMI_PRODUCT_RAY -DMACOSX -D_REENTRANT -DEVIL_ENDIAN -DX86 -DHYPERTHREAD -I$(MAYABASE)/devkit/mentalray/include slitscanMR.cpp -o slitscanMR.o

parse.o: parse.cpp CameraAnim.h
	g++ -c -O3 -mtune=pentiumpro -fexpensive-optimizations -fforce-mem -finline-functions -funroll-loops -fomit-frame-pointer -frerun-cse-after-loop -fstrength-reduce -fforce-addr -fPIC -std=c++98 -dynamic -fno-common -DQMC -DMI_MODULE= -DMI_PRODUCT_RAY -DMACOSX -D_REENTRANT -DEVIL_ENDIAN -DX86 -DHYPERTHREAD -I$(MAYABASE)/devkit/mentalray/include parse.cpp -o parse.o

CameraAnim.o: CameraAnim.cpp CameraAnim.h
	g++ -c -O3 -mtune=pentiumpro -fexpensive-optimizations -fforce-mem -finline-functions -funroll-loops -fomit-frame-pointer -frerun-cse-after-loop -fstrength-reduce -fforce-addr -fPIC -std=c++98 -dynamic -fno-common -DQMC -DMI_MODULE= -DMI_PRODUCT_RAY -DMACOSX -D_REENTRANT -DEVIL_ENDIAN -DX86 -DHYPERTHREAD -I$(MAYABASE)/devkit/mentalray/include CameraAnim.cpp -o CameraAnim.o

install:
	cp slitscanMR.mi $(MAYABASE)/Maya.app/Contents/mentalray/include/.
	cp slitscanMR.so $(MAYABASE)/Maya.app/Contents/mentalray/lib/.
	cp slitscanCache.py ~/Library/Preferences/Autodesk/maya/scripts/.

clean: 
	rm -f slitscanMR.o parse.o CameraAnim.o slitscanMR.so

