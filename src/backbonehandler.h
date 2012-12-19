/**
 * @file   backbonehandler.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Fri Jan 20 16:35:19 2012
 * 
 * @brief  Creates and manages backbone modeling styles 
 * 
 */

#ifndef CCB_BACKBONEHANDLER_H
#define CCB_BACKBONEHANDLER_H

#include "pointers.h"

namespace CCB_NS {
	class BackboneHandler: protected Pointers {
     public:

          //Constructor and Destructor
          BackboneHandler(class CCB *); /**< BackboneHandler constructor */
          ~BackboneHandler(); /**< BackboneHandler deconstructor */

          // Memory Usage
          void memory_usage();

          class Backbone **backbone; /**< List of backbone modelers */

          // Backbone Modeler Styles
          int nbackbone; /**< Running count of input styles */
          int maxbackbone; /**< Maximum number of inputs to create */

          // Public Functions for manipulating backbone modelers 

          void init_backbone(const char *);
          void add_backbone(int, const char **);
          void delete_backbone(const char *);
          void update_backbone(const char *id, int argc, const char **argv, int n);
          void generate_backbone(const char *id);
          int find_backbone(const char *);
               
     private:

	};
}

#endif
