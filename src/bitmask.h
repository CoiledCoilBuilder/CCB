/**
 * @file   bitmask.h
 * @author Chris MacDermaid <chris@cabal.nfs>
 * @date   Sun Dec 11 19:05:07 2011
 * 
 * @brief  keeps track of groups of atoms via bitmask 
 *
 *
 */

#ifndef CCB_BITMASK_H
#define CCB_BITMASK_H

#include "pointers.h"

namespace CCB_NS {

  class Bitmask: protected Pointers {
      public:
  
          //Constructor and Destructor
          Bitmask(class CCB *);
          ~Bitmask(); 
  
          int nmask;
          char **names;
          int *mask;
          int *inversemask;

          int find(const char *name);
          int find_unused();
          int find_mask(const char *name);

          void add_bitmask(const char *name);
          void delete_bitmask(const char *name);

      private:
  
  };
  
}

#endif
