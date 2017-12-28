

#include <fstream>
#include <iostream>


#include "CameraAnim.h"
#include "parse.h"




bool ReadCameraAnim(CameraAnim *ca, const char *filename)
{
    bool done = false;

    std::ifstream ifs;
    ifs.open(filename, std::ifstream::in | std::ifstream::binary);

    if (!ifs.good())
    {
        std::cerr << "unable to open " << filename << std::endl;
        return false;
    }
    else
    {
        Header h;
        ifs.read((char*)&h, sizeof(Header));
        
        if (!h.Match())
        {
            std::cerr << "wrong file  " << filename << std::endl;
            return false;
        }

        ca->start = h.start;
        ca->end = h.end;
    }

    if (ifs.fail())
    {
        std::cerr << "error reading header in file " << filename << "." << std::endl;
        return false;
    }
    else
    {
        Camera *list = ca->GetList();
        ifs.read((char*)list, (ca->end - ca->start + 1) * sizeof(Camera));
    }
    
    if (ifs.fail())
    {
        std::cerr << "error reading camera anim in file " << filename << "." << std::endl;
        return false;
    }

    ifs.close();

    return true;
}




#ifdef TEST_PARSE
int main()
{
    char *filename = "/Users/patrick/cameraCache.bin";
    CameraAnim *ca = new CameraAnim;

    if (ReadCameraAnim(ca, filename))
        ca->Print();

    delete ca;
}
#endif

