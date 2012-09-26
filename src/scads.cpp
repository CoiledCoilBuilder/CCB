/**
 * @file   scads.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Tue Jun 21 12:04:55 2011
 * 
 * @brief  Main routine file, contains the top level SCADS Class.
 * 
 * This is where the meat of the program is.
 */

#include "math.h"
#include "mpi.h"
#include "string.h"
#include "scads.h"
#include "stdlib.h"
#include "memory.h"
#include "universe.h"
#include "error.h"
#include "scadsio.h"
#include "timer.h"
#include "input.h"
#include "output.h"
#include "rotamer_library.h"
#include "type.h"
#include "bitmask.h"
#include "domain.h"
#include "opt.h"
#include "opthandler.h"
#include "atomselecthandler.h"
#include "backbonehandler.h"
#include "analysishandler.h"
#include "tables.h"

#include "tcl.h"
#include "tcl_commands.h"

#define BLEN 200

using namespace SCADS_NS;

/** 
 *  Start an instance for SCADS and allocate fundamental classes memory, error, scadsio and universe,
 *  
 *
 * @param narg number of arguments passed
 * @param arg argument array
 * @param communicator the MPI communicator
 */

SCADS::SCADS(int narg, char **arg, MPI_Comm communicator) {

	/// Initialize fundamental classes
	memory = new Memory(this);
	error = new Error(this);
	universe = new Universe(this, communicator);
	scadsio = new Scadsio(this);
	opthandler = new Opthandler(this);

	/// Set screen to stdout
	screen = stdout;

	/// Set world comm.
	world = communicator;

	/// Create and initialize the top level classes
	create();
	init();

	// create a tcl interpretor instance
	tcl_interp = Tcl_CreateInterp();

	// Associate SCADS ptr with interpreter keyword SCADS
	Tcl_SetAssocData(tcl_interp, "SCADS", NULL, this);

	// initialize custom commands (atomselect, rotamer, run ...)
	init_tcl_commands(this, tcl_interp);

	timer->barrier_start(TIME_INIT);

	if (universe->me == 0) {
		fprintf(screen, "------------------------------------------\n");
		fprintf(screen, "| Computational Optimization of Molecular |\n");
		fprintf(screen, "| Probabilities to Aid Sequence Selection |\n");
		fprintf(screen, "|   COMPASS    Version %-18s |\n", universe->version);
                fprintf(screen, "|  Compiled on: %9s at %9s  |\n", __DATE__,__TIME__); 
		fprintf(screen, "------------------------------------------\n\n");
		fprintf(screen, "Running with %d MPI tasks\n", universe->nprocs);
		fprintf(screen, "Running with %d OMP threads\n\n", universe->nthreads);
	}

	/// Parse command line
	cmdline(narg, arg);

	/// Read the control file using TCL
	read_ctl_file();

	timer->barrier_stop(TIME_INIT);

	if (universe->me == 0)
		fprintf(screen, "Total time to exit: %f min\n", timer->array[TIME_INIT] / 60.0);
}

/**
 * SCADS top-level destructor
 * Kill the top-level classes, and then the fundamental classes
 */

SCADS::~SCADS() {

	// Delete the TCL interpretor
	Tcl_DeleteInterp(tcl_interp);

	// Kill top level classes
	destroy();

	// Delete fundamental classes
	delete opthandler;
	delete universe;
	delete error;
	delete memory;
	delete scadsio;

	fprintf(screen, "-----Normal Termination of COMPASS-----\n");
}

/**
 * Create the top level classes
 */

void SCADS::create() {

	domain = new Domain(this);
	backbone = new BackboneHandler(this);
	atomselect = new AtomSelectHandler(this);
	analysis = new AnalysisHandler(this);
	bitmask = new Bitmask(this);
     tables = new Tables(this);
	timer = new Timer(this);
	opt = NULL;
}

/**
 * initialize the top level classes 
 */
void SCADS::init() {

}

/**
 * destroy the top level classes
 */
void SCADS::destroy() {

	delete timer;
     delete tables;
	delete bitmask;
	delete analysis;
	delete atomselect;
	delete backbone;
	delete domain;

}

/**
 * Run the calculation after system setup
 * 
 */
void SCADS::run() {

	// Check to see that the user specified an optimizaton problem

	// Memory usage pre opt
	scadsio->memory_usage();

	//Solve thermodynamic optimization problem...
	if (opt) {
		opthandler->init_opt();
		opt->optimize();
		opthandler->delete_opt();
	} else
		error->warning(FLERR, "RUN: No optimization problem specified\n");

	// Memory usage post opt 
	//scadsio->memory_usage();

}

void SCADS::read_ctl_file() {

	/// Read the control file using the tcl interpretor
	int code = Tcl_EvalFile(tcl_interp, ctlfile);

	/*
	 * If we wern't successful in parsing the ctl file, return
	 * stacktrace.
	 *
	 * http://tmml.sourceforge.net/doc/tcl/AddErrInfo.html
	 */

	if (code != TCL_OK)
		error->all_tcl(code);
}

/**
 * Parse the command line routines
 *
 * @param argc
 * @param argv
 */

void SCADS::cmdline(int argc, char **argv) {
	//char *line;
	ctlfile = NULL;

	//line = new char[BLEN];
	int i = strlen(argv[argc - 1]) + 1;
	ctlfile = new char[i];

	if (argc < 2)
		error->all(FLERR, "You didn't specify a control file");

	int iarg = 0;
	while (iarg < argc) {
		/// The last argument issued should be the input file
		/// Do some cmdline parsing if necessary
		iarg++;
	}

	strcpy(ctlfile, argv[argc - 1]);

	//delete [] line;
}
