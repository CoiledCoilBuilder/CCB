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

#ifndef SCADS_OUTPUT_H
#define SCADS_OUTPUT_H

#include "pointers.h"

namespace SCADS_NS {

class Output: protected Pointers {

public:

	char *filename; /**< the output filename */
	char *id; /**< the ID of the syle */
	char *style; /**< the name of the style */

	//Constructor and Destructor
	Output(class SCADS *, int, const char **); /**< Output constructor */
	virtual ~Output(); /**< Output destructor, must be virtual! */

	void init();
	void write();

protected:
	int me; /**< Processors info */

	// Child Class Functions
	virtual void init_style() = 0; /**< Initialize the style (declare member variables, etc.. */
	virtual void openfile(); /**< open the file, note this is NOT pure virtual */
	virtual void write_style() = 0; /**< read the file */
	virtual void closefile();

	FILE *fp; /**< file pointer */
private:

};

}

#endif
