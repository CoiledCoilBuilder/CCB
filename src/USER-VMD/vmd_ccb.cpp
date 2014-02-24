// -*-c++-*-

// *hd +------------------------------------------------------------------------------------+
// *hd |  This file is part of Coiled-Coil Builder.                                         |
// *hd |                                                                                    |
// *hd |  Coiled-Coil Builder is free software: you can redistribute it and/or modify       |
// *hd |  it under the terms of the GNU General Public License as published by              |
// *hd |  the Free Software Foundation, either version 3 of the License, or                 |
// *hd |  (at your option) any later version.                                               |
// *hd |                                                                                    |
// *hd |  Coiled-Coil Builder is distributed in the hope that it will be useful,            |
// *hd |  but WITHOUT ANY WARRANTY without even the implied warranty of                     |
// *hd |  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                     |
// *hd |  GNU General Public License for more details.                                      |
// *hd |                                                                                    |
// *hd |  You should have received a copy of the GNU General Public License                 |
// *hd |  along with Coiled-Coil Builder.  If not, see <http:www.gnu.org/licenses/>.        |
// *hd +------------------------------------------------------------------------------------+

// *hd | If you intend to use this software for your research, please cite:
// *hd | and inform Chris MacDermaid <chris.macdermaid@gmail.com> of any pending publications.

// *hd | Copyright (c) 2012,2013,2014 by Chris M. MacDermaid <chris.macdermaid@gmail.com>
// *hd | and Jeffery G. Saven <saven@sas.upenn.edu>


/**
 * @file   vmd_ccb.cpp
 * @author Chris MacDermaid <macdercm@MinasTirith>
 * @date   Thu Apr 25 21:31:50 2013
 *
 * @brief  VMD selection interface for CCB.
 *
 * Unlike the TCL version, the VMD version accepts and manipulates the
 * atomselect DIRECTLY, saving time converting to and from TCL lists.
 * of course all the other options are still available. If you're using
 * this in VMD, use this.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tcl.h>

#include "ccb.h"
#include "error.h"
#include "backbonehandler.h"
#include "backbone.h"
#include "ccbio.h"
#include "output.h"
#include "domain.h"
#include "site.h"
#include "group.h"
#include "atom.h"

#include "AtomSel.h"
#include "MoleculeList.h"
#include "TclCommands.h"
#include "VMDApp.h"

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

using namespace CCB_NS;

/**
 * @brief parse commands
 *
 * @param ccb ccb pointer
 * @param interp tcl interp pointer
 * @param objc number of tcl objects passed
 * @param objv object array
 *
 * @return TCL_OK/TCL_ERROR
 */

/* Tcl plugin wrapper. it parses the arguments, calls the
   real code and then passes the result to the interpreter. */
int tcl_ccb(ClientData UNUSED(clientdata), Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[])
{

    //Fire up a ccb  instance
    CCB *ccb = new CCB(objc, NULL);

    const char **newarg = new const char*[5];

    /// Add the coiled-coil plugin
    newarg[0] = (char *) "backbone";
    newarg[1] = (char *) "add";
    newarg[2] = (char *) "coiledcoil";
    newarg[3] = (char *) "bbcc1";

    if (ccb->backbone->add_backbone(4,newarg) != CCB_OK ||
        ccb->backbone->init_backbone(newarg[3]) != CCB_OK) {
        delete ccb;
        return TCL_ERROR;
    }

    // Parse the commandline options
    int argc = 0;

    const char **argv = new const char*[MAX_ARGS];

    // Look for and expand lists to pass to interface
    Tcl_Obj **list = NULL;
    int len = 0;

    // flags
    bool pdb_flag = 0;
    const char *outfile = NULL;

    // VMD
    bool vmd_flag = 0;
    VMDApp *vmd = NULL;
    AtomSel *sel = NULL;
    Molecule *mol = NULL;
    MoleculeList *mol_list = NULL;

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
                    Tcl_AppendResult(interp, "CCB: Missing argument to -pdb\n", NULL);
                    return TCL_ERROR;
                }

                pdb_flag = 1;
                outfile = Tcl_GetString(objv[++i]);

                // Atom Selection
            } else if (strcmp("-sel", argv[argc]) == 0) {

                if (i + 1 == objc) {
                    Tcl_AppendResult(interp, "CCB: Missing argument to -sel\n", NULL);
                    return TCL_ERROR;
                }

                // Get the VMD handle
                vmd = (VMDApp *)Tcl_GetAssocData(interp, "VMDApp", NULL);
                if (!vmd) {
                    Tcl_AppendResult(interp, "CCB: Unable to find VMD Instance\n", NULL);
                    return TCL_ERROR;
                }

                // Get the selction
                sel = tcl_commands_get_sel(interp, Tcl_GetStringFromObj(objv[++i],NULL));
                if (!sel) {
                    Tcl_AppendResult(interp, "CCB: Invalid atom selection", NULL);
                    return TCL_ERROR;
                }

                if (sel->num_atoms < 1) {
                    Tcl_AppendResult(interp, "CCB: Atomselection contains no atoms", NULL);
                    return TCL_ERROR;
                }

                // Get the Molecule List
                mol_list = vmd->moleculeList;
                if (!mol_list) {
                    Tcl_AppendResult(interp, "CCB: Null Molecule List", NULL);
                    return TCL_ERROR;
                }

                // Get the associated mol
                mol = mol_list->mol_from_id(sel->molid());
                if (!mol) {
                    Tcl_AppendResult(interp, "CCB: Mol associated with selection not found. Deleted?", NULL);
                    return TCL_ERROR;
                }

                // Verbosity
            } else if (strcmp("-v", argv[argc]) == 0) {
                int v = 0;

                if (i + 1 == objc) {
                    Tcl_AppendResult(interp, "CCB: Missing argument to -v\n", NULL);
                    return TCL_ERROR;
                }

                if(Tcl_GetIntFromObj(interp,objv[++i], &v) != TCL_OK)
                    return TCL_ERROR;
                ccb->error->verbosity_level = v;

                // Return to VMD as a TCL list (backward compatability)
            } else if (strcmp("-vmd", argv[argc]) == 0) {
                vmd_flag = 1;

            } else {

                argc++;
            }
        }
    }

    //Pass parsed commands to the style
    if (ccb->backbone->update_backbone(newarg[3],argc,argv,0) != CCB_OK ||
        ccb->backbone->generate_backbone(newarg[3]) != CCB_OK) {
        delete ccb;
        return TCL_ERROR;
    }

    /// Write out the coordinates to a pdb file
    if (pdb_flag) {
        newarg[0] = (char *) "output";
        newarg[1] = (char *) "pdb";
        newarg[2] = (char *) "pdb1";
        newarg[3] = outfile;
        newarg[4] = (char *) "all";

        if (ccb->ccbio->add_output(5, newarg) != CCB_OK ||
            ccb->ccbio->init_output(newarg[2]) != CCB_OK ||
            ccb->ccbio->write_output(newarg[2]) != CCB_OK) {
            delete ccb;
            return TCL_ERROR;
        }
    }

    // Populate the coordinates in the selection DIRECTLY if the user passed it.
    // This should be faster than passing things back and forth via TCL lists.

    if (sel) {

        // Get a pointer to the coordinates for the mol/frame/selection
        float *vmdcoords = sel->coordinates(mol_list);
        int k = 0;
        for (int i = 0; i < ccb->domain->nsite; i++)
            for (int j = 0; j < ccb->domain->site[i]->fixed_atoms->natom; j++) {

                double x[3] = { 0.0 };
                ccb->domain->site[i]->fixed_atoms->atom[j]->get_xyz(x);

                // Only populate if the atom is on;
                if (sel->on[k]) {
                    vmdcoords[0] = x[0];
                    vmdcoords[1] = x[1];
                    vmdcoords[2] = x[2];
                }

                vmdcoords += 3;
                k++;
            }

        // Force gui redraw
        mol->force_recalc(DrawMolItem::MOL_REGEN);

    }

    /// Create TCL object and return coordinates if requested
    if (vmd_flag) {

        Tcl_Obj *resultPtr;
        resultPtr = Tcl_NewListObj(0,NULL);

        for (int i = 0; i < ccb->domain->nsite; i++)
            for (int j = 0; j < ccb->domain->site[i]->fixed_atoms->natom; j++) {

                Tcl_Obj *xyz;

                double coords[3] = { 0.0 };
                ccb->domain->site[i]->fixed_atoms->atom[j]->get_xyz(coords);

                xyz = Tcl_NewListObj(0,NULL);

                for (int k = 0; k < 3; k++)
                    Tcl_ListObjAppendElement(interp,xyz,Tcl_NewDoubleObj(coords[k]));

                Tcl_ListObjAppendElement(interp,resultPtr,xyz);
            }

        Tcl_SetObjResult(interp, resultPtr);
    }

    // Delete argv
    delete [] argv;

    // Delete ccb instance
    delete ccb;

    return TCL_OK;
}

/**
 * Register the plugin with the TCL interpreter
 *
 */

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

    EXTERN int Ccb_SafeInit(Tcl_Interp *)

#else

            int Ccb_SafeInit(Tcl_Interp *)

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

    EXTERN int Ccb_SafeUnload(Tcl_Interp *)

#else

            int Ccb_SafeUnload(Tcl_Interp *)

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

    EXTERN int Ccb_Unload(Tcl_Interp *)

#else

            int Ccb_Unload(Tcl_Interp *)

#endif
    {
        return TCL_OK;
    }
}
