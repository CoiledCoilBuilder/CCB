

/**
 * @file   universe.cpp
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Wed Jun 22 09:15:04 2011
 * 
 * @brief  Contains information about the state of the processors
 *
 * Contains routines pertaining to the number of processors, the processor IDs (ranks), 
 * the partitioning of the processors etc..
 */

#include "universe.h"
#include "version.h"

using namespace CCB_NS;

/** 
 * The universe constructor.
 *
 * @param ccb The ccb class pointer 
 */

Universe::Universe(CCB *ccb) : Pointers(ccb)
{
     version = (char *) CCB_VERSION; 

	//Figure out how many threads running with
 	nthreads = 1;

        // Always rank 0 wrt the universe 
        me = 0;

}

/** 
 * The Universe Deconstructor
 *
 *
 */

Universe::~Universe()
{
}
