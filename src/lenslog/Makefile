

MAYABASE = /Applications/Autodesk/maya2008

# OSX Mental Ray build

MR_OPTIONS = -flat_namespace -undefined suppress -dynamiclib 


LensLogMR.so: LensLogMR.o
	g++ $(MR_OPTIONS) -o LensLogMR.so LensLogMR.o

LensLogMR.o: LensLogMR.cpp
	g++ -c -O3 -mtune=pentiumpro -fexpensive-optimizations -fforce-mem -finline-functions -funroll-loops -fomit-frame-pointer -frerun-cse-after-loop -fstrength-reduce -fforce-addr -fPIC -std=c++98 -dynamic -fno-common -DQMC -DMI_MODULE= -DMI_PRODUCT_RAY -DMACOSX -D_REENTRANT -DEVIL_ENDIAN -DX86 -DHYPERTHREAD -I$(MAYABASE)/devkit/mentalray/include LensLogMR.cpp -o LensLogMR.o

install:
	cp LensLogMR.mi $(MAYABASE)/Maya.app/Contents/mentalray/include/.
	cp LensLogMR.so $(MAYABASE)/Maya.app/Contents/mentalray/lib/.
	cp LensLogNode.py ~/Library/Preferences/Autodesk/maya/plug-ins/.

clean: 
	rm -f LensLogMR.o

