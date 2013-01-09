/**
 * @file   universe.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Wed Jun 22 09:06:38 2011
 * 
 * @brief  Contains information about the state of the processors
 * 
 * 
 */

#ifndef CCB_UNIVERSE_H
#define CCB_UNIVERSE_H

#include "pointers.h"

namespace CCB_NS {

     /**
      * The Universe Class
      */

     class Universe : protected Pointers {
       public:
 
         // Constructors and Destructors
          Universe(class CCB *); /**< The Universe Constructor */
          ~Universe();                       /**< The Universe Deconstructor */
         
          char *version;        /**< CCB version */
          int nthreads;			/**< OMP_NUM_THREADS */

          int me;

     };
}

#endif
