
#ifndef _LOGFILE_H
#define _LOGFILE_H

#include <cstdio>

#include "shader.h"

class LogFile
{
  public:
    LogFile() : fp(0) { }

    ~LogFile() { Close(); }

    // log file close
    bool Close() { if (fp) fclose(fp); fp = 0; }

    // log file open
    bool Open(char *fname) { bool ret = true; Close(); if ((fp = fopen(fname, "w")) == 0) ret = false; return ret; }

    // log file write
    bool Write(miScalar x, miScalar y, miVector *org, miVector *dir, double dist, miColor *res) { if (fp) fprintf(fp, "%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g\n", x, y, org->x, org->y, org->z, dir->x, dir->y, dir->z, dist, res->r, res->g, res->b); return true; }
    bool Write(miScalar x, miScalar y, char *msg) { if (fp) fprintf(fp, "%g,%g,%s\n", x, y, msg); return true; }
 
  private:
    FILE *fp;
};


#endif

