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

#include "mpi.h"
#include <omp.h>
#include "universe.h"
#include "version.h"

using namespace SCADS_NS;

/** 
 * The universe constructor.
 *
 * Set the scads version, and define variables for the communicator, rank and size of
 * processor pool.
 *
 * @param scads The scads class pointer 
 * @param communicator the MPI communicator
 */

Universe::Universe(SCADS *scads, MPI_Comm communicator) : Pointers(scads)
{
     version = (char *) SCADS_VERSION; 

     world = communicator;
     MPI_Comm_rank(world,&me);
     MPI_Comm_size(world,&nprocs);

 	//Figure out how many threads running with
 	nthreads = 1;
 	#pragma omp parallel default(shared)
 	{
 	 	#pragma omp master
 		{
 			nthreads = omp_get_num_threads();
 		}
 	}
}

/** 
 * The Universe Deconstructor
 *
 *
 */

Universe::~Universe()
{
}
