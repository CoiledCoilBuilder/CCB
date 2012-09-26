
#include "scadstype.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "memory.h"
#include "error.h"

using namespace SCADS_NS;

/* ---------------------------------------------------------------------- */

Memory::Memory(SCADS *scads) : Pointers(scads) {}

/* ----------------------------------------------------------------------
   safe malloc 
------------------------------------------------------------------------- */

void *Memory::smalloc(bigint nbytes, const char *name)
{
  if (nbytes == 0) return NULL;

#if defined(SCADS_MEMALIGN)
  void *ptr;
  posix_memalign(&ptr, SCADS_MEMALIGN, nbytes);
#else
  void *ptr = malloc(nbytes);
#endif
  if (ptr == NULL) {
    char str[128];
    sprintf(str,"Failed to allocate " BIGINT_FORMAT " bytes for array %s",
	    nbytes,name);
    error->one(FLERR,str);
  }
  return ptr;
}

/* ----------------------------------------------------------------------
   safe realloc 
------------------------------------------------------------------------- */

void *Memory::srealloc(void *ptr, bigint nbytes, const char *name)
{
  if (nbytes == 0) {
    destroy(ptr);
    return NULL;
  }

  ptr = realloc(ptr,nbytes);
  if (ptr == NULL) {
    char str[128];
    sprintf(str,"Failed to reallocate " BIGINT_FORMAT " bytes for array %s",
	    nbytes,name);
    error->one(FLERR,str);
  }
  return ptr;
}

/* ----------------------------------------------------------------------
   safe free 
------------------------------------------------------------------------- */

void Memory::sfree(void *ptr)
{
  if (ptr == NULL) return;
  free(ptr);
}

/* ----------------------------------------------------------------------
   erroneous usage of templated create/grow functions
------------------------------------------------------------------------- */

void Memory::fail(const char *name)
{
  char str[128];
  sprintf(str,"Cannot create/grow a vector/array of pointers for %s",name);
  error->one(FLERR,str);
}
