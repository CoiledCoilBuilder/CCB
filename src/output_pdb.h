/**
 * @file   output_pdb.h
 * @author CVB <chris@CirithUngol>
 * @date   Sun Jun 26 14:39:03 2011
 * 
 * @brief  pdb output style header
 * 
 * 
 */


#ifdef OUTPUT_CLASS

OutputStyle(pdb,OutputPDB)

#else

#ifndef CCB_OUTPUT_PDB_H
#define CCB_OUTPUT_PDB_H

#include "output.h"

namespace CCB_NS {
     
     class OutputPDB : public Output {
     
     public:
          OutputPDB(class CCB *, int, const char **);
          ~OutputPDB() {}
     
     private:
          int mask;                   /**< bitmask for atoms to output */
          void init_style();          /**< Initialize the style */
          void write_style();         /**< read the file based on style */
          void chomp(char *s, int n); /**< Shorten string to n characters */
     };
}

#endif
#endif
