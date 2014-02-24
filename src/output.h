

/**
 * @file   output.h
 * @author CVB <cvb@magrathea.chem.upenn.edu>
 * @date   Thu Jun 23 15:53:06 2011
 * 
 * @brief  Main header for Output class and subclasses
 * 
 * All output styles inherit from this output class
 * note the virtual funtions that are replaced by the
 * inheriting subclass "style". Because of this scheme
 * subclasses can still use the pointer notation error->all!
 */

#ifndef CCB_OUTPUT_H
#define CCB_OUTPUT_H

#include "pointers.h"

namespace CCB_NS {

class Output: protected Pointers {

public:

	char *filename; /**< the output filename */
	char *id; /**< the ID of the syle */
	char *style; /**< the name of the style */

	//Constructor and Destructor
	Output(class CCB *, int, const char **); /**< Output constructor */
	virtual ~Output(); /**< Output destructor, must be virtual! */

	int init();
	int write();

protected:
	int me; /**< Processors info */

	// Child Class Functions
	virtual int init_style() = 0; /**< Initialize the style (declare member variables, etc.. */
	virtual int write_style() = 0; /**< read the file */

	virtual int openfile(); /**< open the file, note this is NOT pure virtual */
	virtual int closefile();

	FILE *fp; /**< file pointer */
private:

};

}

#endif
