/**
 * @file   scads.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Tue Jun 21 09:29:05 2011
 * 
 * @brief  Main SCADS header, define namespace, fundamental and top level classes.
 */

#ifndef SCADS_SCADS_H
#define SCADS_SCADS_H

#include "mpi.h"
#include "stdio.h"
#include "tcl.h"


/**
 * @namespace SCADS_NS
 *
 * The scads namespace. This is the top-level namespace 
 * all classes should belong to this namespace if they're
 * inherited from the scads super-class.
 */


namespace SCADS_NS {

	/**
	 * SCADS main class
	 *
	 * All fundamental and top level classes initialize under the SCADS class.
	 *
	 * This class is initialized in the main.cpp file. All fundamental and
	 * top-level classes should be public and listed below. Note that the top level
	 * and fundamental class names have a capital beginning letter, their pointers do not.
	 */

	class SCADS {

     public:

          // Pointers to fundamental SCADS classes

          class Memory *memory; /**< Memory Allocation Routines: memory.cpp/memory.h, taken from LAMMPS */
          class Error *error; /**< Error handling: error.cpp/error.h,	taken from LAMMPS */
          class Universe *universe; /**< Universe of Processors: universe.cpp/universe.h, taken from LAMMPS */
          class Scadsio *scadsio; /**< Input and output styles for reading and outputting files */
          class Opthandler *opthandler; /**< manages optimizers */

          // Pointers to top-level SCADS classes

          class Comm *comm; /**< Custom MPI communication routines: comm.cpp/comm.h */
          class Compute *compute; /**< Any quantity that needs to be computed internally: potentials, center of mass, RMSD, supports styles: compute.cpp/compute.h */
          class Domain *domain; /**< All atomistic/coarse-grain propertities and quantities related to the system: domain.cpp/domain.h */
          class AnalysisHandler *analysis; /**< Analysis utilities for post-processing results, suports styles: analysis.cpp/analysis.h */
          class Opt *opt; /**< Optimization routines, linear/non-linear solvers, entropy calculation: opt.cpp/opt.h */
          class Timer *timer; /**< Timing routines timing.cpp/timing.h */
          class Bitmask *bitmask; /**< Dynamic collections of atoms... */
          class AtomSelectHandler *atomselect; /** <manages atomselections */
          class BackboneHandler *backbone; /** <manages backbone modeler styles */
          class Tables *tables; /**< Manages Lookup Tables */

          // Output and Communication
          FILE *screen; /**< Output to Screen, "what am I doing at this very moment?" */
          MPI_Comm world; /**< MPI Communicator */

          // Constructor and Destructors
          SCADS(int, char**, MPI_Comm);
          ~SCADS();

          // Functions
          void create(); /**< Creates instances of the top level classes */
          void init(); /**< Initializes & sets up the top level classes, Input->init() */
          void destroy(); /**< Destroys the top level classes */
          void run(); /**< The main running routine, this is called after everything is setup */

          void cmdline(int argc, char **argv); /**< Parses the command line */
          void read_ctl_file(); /**< Reads the control file */
          void writeOutput();

          Tcl_Interp *tcl_interp; /** < The TCL interpretor */

     private:
          char *ctlfile; /**< The Input Control File */
          void sorttest(SCADS *scads);
	};

}

#endif
