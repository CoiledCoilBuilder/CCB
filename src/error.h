

/**
 * @file   error.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Tue Jun 21 15:37:10 2011
 *
 * @brief  Error Routines header.
 *
 * Contains all of the necessary error routines
 */

#ifndef CCB_ERROR_H
#define CCB_ERROR_H

#include "pointers.h"

#define FLERR __FILE__,__LINE__
#define CCB_OK 0
#define CCB_ERROR 1

namespace CCB_NS {

/**
 * Error Class
 * these can be called like: error->all("Hey, this is broken")
 */

class Error: protected Pointers {
  public:
    Error(class CCB *);

    int verbosity_level; /**< Verbosity Level, higher = more verbose */

    int  all(const char *file, int line, const char *str);
    int  one(const char *file, int line, const char *str);
    void warning(const char *file, int line, const char *str);
    void message(const char *file, int line, const char *str);
    void message_verb(char *str, int verbosity);
};

}

#endif
