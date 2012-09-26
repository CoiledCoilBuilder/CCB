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

#include "pointers.h"

namespace SCADS_NS {

     /**
      * The Universe Class
      */

     class Universe : protected Pointers {
       public:
 
         // Constructors and Destructors
          Universe(class SCADS *); /**< The Universe Constructor */
          ~Universe();                       /**< The Universe Deconstructor */
         
          char *version;        /**< SCADS version */
          int nthreads;			/**< OMP_NUM_THREADS */

          int me;

     };
}

#endif
