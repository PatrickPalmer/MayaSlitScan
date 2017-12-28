
#include <cstdio>

#include "shader.h"
#include "LogFile.h"



/*
 * This lens shader maps color components lying in the interval
 * [floor, ceiling] to the interval [0, 1]. Values below
 * and above those limits are clamped to 0 and 1, respectively.
 * This shader may be used to as a "poor man's tonemapping".
 * Default values are floor = 0 and ceiling = 1.
 */
typedef struct mibLog {
    miScalar    floor;        /* color interval low bound */
    miScalar    ceiling;    /* color interval high bound */
    miBoolean    luminance;    /* clamp color or luminance */
    miColor        floor_color;    /* return color if lum is below floor*/
    miColor        ceil_color;    /* return color if lum is above ceil*/
} mibLog_t;


extern "C" DLLEXPORT int lens_log_version(void) { return 1; }


extern "C" DLLEXPORT void lens_log_init(
    miState * state,
    mibLog_t * in_pParams,
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
        LogFile *log = new LogFile;
        log->Open("/Users/patrick/test.log");
        void **userptr;
        mi_query(miQ_FUNC_USERPTR, state, 0, &userptr);
        *userptr = log;
    }

}


extern "C" DLLEXPORT void lens_log_exit(
    miState * state,
    mibLog_t * in_pParams)
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
                LogFile *log = static_cast<LogFile*>(*userptr);
                delete log;
            }
        }

        *userptr = 0;
    }
}



extern "C" DLLEXPORT miBoolean lens_log(
    miColor*    result,
    miState*    state,
    mibLog_t*    paras)
{
    miBoolean res;

    // get the user data
    void **userptr;
    mi_query( miQ_FUNC_USERPTR, state, 0, &userptr );
    LogFile *log = static_cast<LogFile *>(*userptr);

    res = mi_trace_eye(result, state, &state->org, &state->dir);

    miLock *lock;
    mi_query(miQ_FUNC_LOCK, state, miNULLTAG, &lock);
    mi_lock(*lock);

    log->Write(state->raster_x, state->raster_y, &state->org, &state->dir, state->dist, result);
     
    mi_unlock(*lock);

    return res;
}

