

#include <iostream>
#include <cmath>

#include "shader.h"
#include "CameraAnim.h"
#include "parse.h"



void Signature()
{
    std::cerr << "SlitScan Mental Ray Lens, (c) 2008 Patrick Palmer" << std::endl;
}


struct UserData
{
    CameraAnim ca;    
};


typedef struct mibSlitScan 
{
    //miScalar    slitWidth;
    miTag        filename;
} mibSlitScan_t;



extern "C" DLLEXPORT int SlitScanLens_version(void) 
{ 
    return 1; 
}



extern "C"
DLLEXPORT void SlitScanLens_init(
    miState * state,
    mibSlitScan_t * in_pParams,
    miBoolean *inst_req)
{
    if(!in_pParams) 
    {
        // Set this to miTRUE if you want instance specific initialization.
        *inst_req = miTRUE;
        /* add global init code here */
    } 
    else 
    {
        Signature();

        UserData *ud = new UserData;

        char *fname = (char *)mi_db_access( in_pParams->filename );
        ReadCameraAnim(&ud->ca, fname);
        mi_db_unpin( in_pParams->filename );

        void **userptr;
        mi_query(miQ_FUNC_USERPTR, state, 0, &userptr);
        *userptr = ud;
    }
}


extern "C" DLLEXPORT void SlitScanLens_exit(
    miState * state,
    mibSlitScan_t * in_pParams)
{
    if(!in_pParams) 
    {
        /* add global exit code here */
    } 
    else 
    {
        /* add instance-specific exit code here */
        void **userptr;
        mi_query( miQ_FUNC_USERPTR, state, 0, &userptr );

        // close file
        if (userptr)
        {
            if (*userptr)
            {
                UserData *ud = static_cast<UserData*>(*userptr);
                delete ud;
            }
        }

        *userptr = 0;
    }
}




extern "C" DLLEXPORT miBoolean SlitScanLens(
    miColor*    result,
    miState*    state,
    mibSlitScan_t*    paras)
{
    miBoolean     res;

    // get the user data
    void **userptr;
    mi_query( miQ_FUNC_USERPTR, state, 0, &userptr );
    UserData *ud = static_cast<UserData *>(*userptr);

    miVector cs_org;        // camera space origin
    miVector cs_dir;        // camera space direction vector

    mi_point_to_camera(state, &cs_org, &state->org);
    mi_vector_to_camera(state, &cs_dir, &state->dir);

    // width height
    int x_resolution = state->camera->x_resolution;
    int y_resolution = state->camera->y_resolution;

    // number of camera entries
    int camcount = ud->ca.end - ud->ca.start + 1;

    // ratio of image width to camera count
    double ratio = double(x_resolution) / double(camcount);
 
    //if (ratio != floor(ratio))
    //    ratio = floor(ratio) + 1.0;

    // raster_x is current column
    double offset = double(state->raster_x) / ratio;
    if (offset != floor(offset))
        offset = floor(offset + 0.5);
    if (offset > camcount)
        offset = camcount;

    // find camera
    Camera *cam = ud->ca.FindCamera(int(offset) + ud->ca.start);

    // if there isn't a camera, this is a problem
    if (cam == 0)
    {
printf("unable to read camera for slot %d (off %g x %d xres %d start %d ratio %g)\n", (int(offset)+ud->ca.start), offset, state->raster_x, x_resolution, ud->ca.start, ratio);

        result->r = 1.0;
        result->g = 0.0;
        result->b = 0.0;
        return true;
    }

    miMatrix cam_mat;
    int i;
    for (i = 0; i < 16; i++)
        cam_mat[i] = float(cam->mat[i]);

    miVector ncs_org;
    miVector ncs_dir;

    mi_point_transform(&ncs_org, &cs_org, cam_mat);
    mi_vector_transform(&ncs_dir, &cs_dir, cam_mat);
    mi_vector_norm(&ncs_dir);

    miVector is_org;
    miVector is_dir;

    mi_point_from_world(state, &is_org, &ncs_org);
    mi_vector_from_world(state, &is_dir, &ncs_dir);

    res = mi_trace_eye(result, state, &is_org, &is_dir);

    return res;
}

