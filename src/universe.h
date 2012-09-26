/**
 * @file   universe.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Wed Jun 22 09:06:38 2011
 * 
 * @brief  Contains information about the state of the processors
 * 
 * 
 */

#ifndef SCADS_UNIVERSE_H
#define SCADS_UNIVERSE_H

#include "mpi.h"
#include "pointers.h"

namespace SCADS_NS {

     /**
      * The Universe Class
      */

     class Universe : protected Pointers {
       public:
          
          char *version;        /**< SCADS version */
          
          MPI_Comm world;       /**< MPI Communicator */
          int me;               /**< What is my processor ID? */
          int nprocs;           /**< How many processes started? */

          int nthreads;			/**< OMP_NUM_THREADS */

          // Constructors and Destructors
          Universe(class SCADS *, MPI_Comm); /**< The Universe Constructor */
          ~Universe();                       /**< The Universe Deconstructor */
     };
}

#endif
