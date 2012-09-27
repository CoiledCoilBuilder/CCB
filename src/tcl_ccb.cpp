/**
 * @file   tcl_ccb.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Wed Sep 26 15:26:05 2012
 *
 * @brief  Tcl interface for ccb plugin
 *
 * Load into tcl like:
 * 
 * %load libccb.so
 * then call it using the "ccb" command
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tcl.h>

#include "scads.h"

using namespace SCADS_NS;

/* Tcl plugin wrapper. it parses the arguments, calls the
   real code and then passes the result to the interpreter. */
int tcl_ccb(ClientData nodata, Tcl_Interp *interp,
    int objc, Tcl_Obj *const objv[])
{

    //Fire up a ccb  instance
    SCADS *scads = new SCADS(objc, NULL);

    // Delete ccb instance
    delete scads;

    return TCL_OK;
}

extern "C" {

    /* register the plugin with the tcl interpreters */
#if defined(CCBTCLDLL_EXPORTS) && defined(_WIN32)
#  undef TCL_STORAGE_CLASS
#  define TCL_STORAGE_CLASS DLLEXPORT

#define WIN32_LEAN_AND_MEAN /* Exclude rarely-used stuff from Windows headers */
#include <windows.h>

    BOOL APIENTRY DllMain( HANDLE hModule,
        DWORD  ul_reason_for_call,
        LPVOID lpReserved )
    {
        return TRUE;
    }

    EXTERN int Ccb_Init(Tcl_Interp *interp)

#else

        int Ccb_Init(Tcl_Interp *interp)

#endif
    {
        Tcl_CreateObjCommand(interp,"ccb",tcl_ccb,
            (ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

        Tcl_PkgProvide(interp, "ccb", "1.0");

        return TCL_OK;
    }
}
