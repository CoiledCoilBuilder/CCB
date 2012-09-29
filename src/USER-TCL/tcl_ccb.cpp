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
#include "error.h"
#include "backbonehandler.h"
#include "backbone.h"
#include "scadsio.h"
#include "output.h"
#include "domain.h"
#include "site.h"
#include "group.h"
#include "atom.h"

/**
 * @def BLEN
 * @brief Buffer Size
 */

#define BLEN 64

/**
 * @def MAX_ARGS
 * @brief Maximim number of argumets to parse
 */

#define MAX_ARGS 64

using namespace SCADS_NS;

/**
 * @brief parse commands
 *
 * @param scads scads pointer
 * @param interp tcl interp pointer
 * @param objc number of tcl objects passed
 * @param objv object array
 *
 * @return TCL_OK/TCL_ERROR
 */

/* Tcl plugin wrapper. it parses the arguments, calls the
   real code and then passes the result to the interpreter. */
int tcl_ccb(ClientData /**/, Tcl_Interp *interp,
    int objc, Tcl_Obj *const objv[])
{

    //Fire up a ccb  instance
    SCADS *scads = new SCADS(objc, NULL);

    const char **newarg = new const char*[5];

    /// Add the coiled-coil plugin
    newarg[0] = (char *) "backbone";
    newarg[1] = (char *) "add";
    newarg[2] = (char *) "coiledcoil";
    newarg[3] = (char *) "bbcc1";
    scads->backbone->add_backbone(4,newarg);
    scads->backbone->init_backbone(newarg[3]);

    // Parse the commandline options
    int argc = 0;

    const char **argv = new const char*[MAX_ARGS];

    // Look for and expand lists to pass to interface
    Tcl_Obj **list = NULL;
    int len = 0;

    // flags
    bool pdb = 0;
    const char *outfile;
    bool vmd = 0;

    // Parse Arguments
    for (int i = 1; i < objc; ++i) {
        // check for a sublist, converts to list if not, expensive?
        if (Tcl_ListObjGetElements(interp, objv[i], &len, &list) != TCL_OK)
            return TCL_ERROR;

        for (int j = 0; j < len; j++) {

            // is this OK to do?
            argv[argc] = Tcl_GetString(list[j]);

            // Check for PDB
            if (strcmp("-pdb", argv[argc]) == 0) {

                 if (i + 1 == objc) {
                      Tcl_AppendResult(interp, "Missing argument to -pdb\n", NULL);
                      return TCL_ERROR;
                 }

                pdb = 1;
                outfile = Tcl_GetString(objv[++i]);

                // Verbosity
            } else if (strcmp("-v", argv[argc]) == 0) {
                int v = 0;
                
                if (i + 1 == objc) {
                     Tcl_AppendResult(interp, "Missing argument to -v\n", NULL);
                     return TCL_ERROR;
                }

                if(Tcl_GetIntFromObj(interp,objv[++i], &v) != TCL_OK)
                    return TCL_ERROR;
                scads->error->verbosity_level = v;

            // VMD
            } else if (strcmp("-vmd", argv[argc]) == 0) {
                vmd = 1;

            } else {

                argc++;
            }
        }
    }

    //Pass parsed commands to the style
    scads->backbone->update_backbone(newarg[3],argc,argv,0);

    // Generate the coordinates
    scads->backbone->generate_backbone(newarg[3]);

    /// Write out the coordinates to a pdb file
    if (pdb) {
        newarg[0] = (char *) "output";
        newarg[1] = (char *) "pdb";
        newarg[2] = (char *) "pdb1";
        newarg[3] = outfile;
        newarg[4] = (char *) "all";
        scads->scadsio->add_output(5, newarg);
        scads->scadsio->init_output(newarg[2]);
        scads->scadsio->write_output(newarg[2]);
        scads->scadsio->delete_output(newarg[2]);
    }

    /// Create TCL object and return coordinates if requested
    if (vmd) {

        Tcl_Obj *resultPtr;
        resultPtr = Tcl_NewListObj(0,NULL);

        for (int i = 0; i < scads->domain->nsite; i++)
            for (int j = 0; j < scads->domain->site[i]->fixed_atoms->natom; j++) {

                Tcl_Obj *xyz;

                double coords[3] = { 0.0 };
                scads->domain->site[i]->fixed_atoms->atom[j]->get_xyz(coords);

                xyz = Tcl_NewListObj(0,NULL);

                for (int k = 0; k < 3; k++)
                    Tcl_ListObjAppendElement(interp,xyz,Tcl_NewDoubleObj(coords[k]));

                Tcl_ListObjAppendElement(interp,resultPtr,xyz);

                fprintf(stdout, "TEST");
                fflush(stdout);  
            }

        Tcl_SetObjResult(interp, resultPtr);
    }

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

#if defined(USE_TCL_STUBS)
       if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL)
                    return TCL_ERROR;
       if (Tcl_PkgRequire(interp, "Tcl", TCL_VERSION, 0) == NULL)
                    return TCL_ERROR;
#endif

       if (Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK)
            return TCL_ERROR;

        Tcl_CreateObjCommand(interp,"ccb",tcl_ccb,
            (ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);

        return TCL_OK;
    }

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

    EXTERN int Ccb_SafeInit(Tcl_Interp *interp)

#else

    int Ccb_SafeInit(Tcl_Interp *interp)

#endif
    {
        return TCL_OK;
    }

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

    EXTERN int Ccb_SafeUnload(Tcl_Interp *interp)

#else

    int Ccb_SafeUnload(Tcl_Interp *interp)

#endif
    {
        return TCL_OK;
    }

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

    EXTERN int Ccb_Unload(Tcl_Interp *interp)

#else

    int Ccb_Unload(Tcl_Interp *interp)

#endif
    {
        return TCL_OK;
    }
}
