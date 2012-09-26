/**
 * @file   scadsio.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Thu Jun 23 16:54:19 2011
 * 
 * @brief  Creates and manages input and output styles
 * 
 * Anything that is input or output to/from the program
 * must be setup using these routines.
 *
 * To call this, it should be something like:
 * input in1 control fname
 * output out1 pdb fname
 *
 * input = directive
 * in1 = unique name given to the input
 * control = the input "style"
 * fname = the path to the input file
 */

#ifndef SCADS_SCADSIO_H
#define SCADS_SCADSIO_H

#include "pointers.h"

namespace SCADS_NS {
	class Scadsio: protected Pointers {
		public:

			//Constructor and Destructor
			Scadsio(class SCADS *); /**< Scadsio constructor */
			~Scadsio(); /**< Scadsio deconstructor (deletes all associated styles on termination) */

			// Memory Usage
			void memory_usage();

			class Output **output; /**< List of Outputs, e.g. pointer to pointer to output */

			//Output Styles
			int noutput; /**< Running count of output styles */
			int maxoutput; /**< Maximum number of outputs to create */

			//Outputs
			void init_output(const char *);
			void write_output(const char *);
			void add_output(int, const char **);
			void delete_output(const char *);
			void active_outputs();
			int find_output(const char *);
		
                private:

	};
}

#endif
