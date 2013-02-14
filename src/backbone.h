/**
 * @file   backbone.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Fri Jan 20 16:44:06 2012
 * 
 * @brief  Main header for backbone modeler base class 
 * 
 */

#ifndef CCB_BACKBONE_H
#define CCB_BACKBONE_H

#include "pointers.h"

namespace CCB_NS {
  class Backbone: protected Pointers {

    public:

            char *id; /**< the ID of the syle */
            char *style; /**< the name of the style */

            //Constructor and Destructor
            Backbone(class CCB *, int, const char **); /**< Backbone constructor */
            virtual ~Backbone(); /**< Backbone destructor, must be virtual! */

            int init();
            int update(int argc, const char **argv, int n); /**< Update parameters */
            int generate();    /**< Generate coordiantes */

    protected:

            // Child Class Functions
            virtual int init_style() = 0; /**< Initialize the style (declare member variables, etc.. */
            virtual int update_style(int argc, const char **argv, int n) = 0; /**< update coordinates based on passed params*/
            virtual int generate_style() = 0; /**< Generate Coordiantes for the particular style */

    private:

  };
}

#endif
