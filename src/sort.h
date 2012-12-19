/**
 * @file   sort.h
 * @author Chris <chris@mount-doom.chem.upenn.edu>
 * @date   Sun Jan 29 09:04:05 2012
 *
 * @brief Sort an array using the quicksort algorithm.
 *
 * If the list is short e.g. <= 10 elements, then
 * insertion sort is used.
 *
 * The algorithm's base code is taken from google's V8
 * sourcecode, available at: http://code.google.com/p/v8/
 *
 * We use templates to take advantage of the
 * differing types that can be sorted.
 *
 * Sort<int> sort(ccb);
 * sort.quicksort(sortme, 0, 1000);
 *
 * Sort<Group> sort(ccb);
 * sort.quicksort(sortme, 0, 1000, compare_group_prob)
 *
 * \todo {parallelize the quicksort routine}
 */

#ifndef CCB_SORT_H
#define CCB_SORT_H

#include "pointers.h"
#include "group.h"
#include "string.h"
#include "atom.h"
#include "site.h"
#include "group.h"

namespace CCB_NS {

    template <class TYPE>
        class Sort : protected Pointers {

    public:
    Sort(class CCB *) : Pointers(ccb) {/* ... */}
        ~Sort() {}

        // customized comparables MUST BE STATIC!!!
        // or the calling class won't be able to determine
        // it's address...

        /* +-------------------------+  */
        /* | ATOM LEVEL COMPARISONS  |  */
        /* +-------------------------+  */

        // Compare atoms chain
        static int compare_atom_chain(Atom *x, Atom *y) {
            return strcmp(x->site->chain, y->site->chain);
        }

        // Compare atoms resid
        static int compare_atom_resid(Atom *x, Atom *y) {
            if (x->site->resid == y->site->resid) return 0;
            else return x->site->resid < y->site->resid ? -1 : 1;
        }

        // Compare atoms type
        static int compare_atom_type(Atom *x, Atom *y) {
            return strcmp(x->type, y->type);
        }

        // Compare atoms name
        static int compare_atom_name(Atom *x, Atom *y) {
            return strcmp(x->name, y->name);
        }

       // Compare atoms id 
        static int compare_atom_id(Atom *x, Atom *y) {
            if (x->id == y->id) return 0;
            else return x->id < y->id ? -1 : 1;
        }


        /* +--------------------------+  */
        /* | GROUP LEVEL COMPARISONS  |  */
        /* +--------------------------+  */

        // Compare group types
        static int compare_group_type(Group *x, Group *y) {
            return strcmp(x->type,y->type);
        }

        // Compare physical address in memory
        static int compare_group_address(Group *x, Group *y) {
            if (x == y ) return 0;
            else return x < y ? -1 : 1;
        }

        // Compare group resid
        static int compare_group_resid(Group *x, Group *y) {
            if (x->site->resid == y->site->resid) return 0;
            else return x->site->resid < y->site->resid ? -1 : 1;
        }

        // compare group chain
        static int compare_group_chain(Group *x, Group *y) {
            return strcmp(x->site->chain,y->site->chain);
        }

        /* +-------------------------+  */
        /* | SITE LEVEL COMPARISONS  |  */
        /* +-------------------------+  */

        // Compare site resid
        static int compare_site_resid(Site *x, Site *y) {
            if (x->resid == y->resid) return 0;
            else return x->resid < y->resid ? -1 : 1;
        }

        // compare site chain
        static int compare_site_chain(Site *x, Site *y) {
            return strcmp(x->chain,y->chain);
        }

        /* +----------------------+  */
        /* | DEFAULT COMPARISONS  |  */
        /* +----------------------+  */

        // Default comparable and swap functions
        int compare(TYPE x, TYPE y) {
            if ( x == y) return 0;
            else return x < y ? -1 : 1;
        }

        // Compare physical address in memory
        static int compare(TYPE *x, TYPE *y) {
            if (x == y ) return 0;
            else return x < y ? -1 : 1;
        }

        void swap(TYPE a[], int i, int j) {
            TYPE temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }

        void swap(TYPE **a, int i, int j) {
            TYPE *temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }

        /* ----------------------------------------------------------------------
         *  Sort an array in place, moves array value, not pointer.
         *  No need to add anything down here.
         * ---------------------------------------------------------------------- */

        void quicksort(TYPE a[], const int from, const int to) {

            // Insertion sort is faster for short arrays
            if (to - from <= 10) {
                insertionsort(a, from, to);
                return;
            }

            // find a pivot as the median of first, last and middle element

            TYPE v0 = a[from];
            TYPE v1 = a[to - 1];
            int middle_index = from + ((to - from) >> 1);
            TYPE v2 = a[middle_index];

            // compare v0 and v1.
            int c01 = compare(v0,v1);
            if (c01 > 0) {
                // v1 < v0, swap
                TYPE temp = v0;
                v0 = v1;
                v1 = temp;
            } // v0 <= v1

            // compare v0 and v2
            int c02 = compare(v0,v2);
            if (c02 > 0) {
                // v2 <= v0 <= v1
                TYPE temp = v0;
                v0 = v2;
                v2 = v1;
                v1 = temp;
            } else {
                // v0 <= v1 && v0 < v2
                // compare v1 and v2
                int c12 = compare(v1,v2);
                if (c12 > 0) {
                    // v0 <= v2 < v1
                    TYPE temp = v1;
                    v1 = v2;
                    v2 = temp;
                }
            }

            // v0 <= v1 <= v2
            a[from] = v0;
            a[to - 1] = v2;
            TYPE pivot = v1;
            int low_end = from + 1; // Upper bound of elements lower than pivot.
            int high_start = to - 1; // Lower bound of elements greater than pivot.
            a[middle_index] = a[low_end];
            a[low_end] = pivot;

            // From low_end to i are elements equal to pivot.
            // From i to high_start are elements that haven't been compared yet.

            for (int i = low_end + 1; i < high_start; i++) {
                TYPE element = a[i];
                int order = compare(element, pivot);
                if (order < 0) {
                    swap(a, i, low_end);
                    low_end++;
                } else if (order > 0) {
                    do {
                        high_start--;
                        if (high_start == i) goto loopDone;
                        TYPE top_elem = a[high_start];
                        order = compare(top_elem, pivot);
                    } while (order > 0);
                    swap(a, i, high_start);
                    if (order < 0) {
                        swap(a, i, low_end);
                        low_end++;
                    }
                }
            }

            // This is a label called by goto to break out of the
            // nested loops. Deal with it.
        loopDone:
            quicksort(a, from, low_end);
            quicksort(a, high_start, to);
        }

        void insertionsort(TYPE a[], const int from, const int to) {
            for (int i = from + 1; i < to; i++) {
                TYPE element = a[i];
                int j;
                for (j = i - 1; j >= from; j--) {
                    TYPE temp = a[j];
                    int order = compare(temp, element);
                    if (order > 0) {
                        a[j + 1] = temp;
                    } else {
                        break;
                    }
                }
                a[j + 1] = element;
            }
        }

        /**
         * @brief Identify the unique values in the array a
         *
         * Identify unique values, put those "n" values at the
         * beginning of the array "a". The number of unique values
         * is returned in the variable n.
         *
         * The unique values are returned sorted.
         *
         * @param a the array to sort
         * @param from index to start at
         * @param to index to end at
         * @param comparefn the function used to compare the values
         * @param n the number of unique values identified
         */

        void unique(TYPE a[], const int from, const int to, int &n) {

            // First, sort the array
            quicksort(a, from, to);

            // Now, go over the array and identify the unique values
            n = 0;
            int i = from;
            while (i < to) {

                // Swap the values so we never lose any values
                swap(a,n,i++);

                // skip over array while values are the same
                while (i < to && compare(a[n], a[i]) == 0)
                    i++;

                n++;
            }
        }

        /* ----------------------------------------------------------------------
         *  Sort an array of pointers to pointers to structs/class.
         *  we need to provide one additional thing to make this work :
         *
         *  1. an function describing how to compare the values in the
         *  objs. This function should always return an int:
         *
         *   0 = compared values are equal.
         *  -1 = val1 < val2
         *   1 = val1 > val 2
         *
         *  and take pointers to the objects to compare. This function is passed on
         *  evocation of the qsort function but is defined IN the template as a
         *  static member function.
         *
         *  sort.insertionsort(group, 0, ngroup, sort.compare_group_prob);
         *
         *  The reason we pass function pointers is because the data structure
         *  types are completely arbitrary and would require template
         *  specilization for each compare function. However, the qsort
         *  method for sorting the arrays is the same for all types.
         *
         *  This method is implemented to work on sorting the pointer to pointer
         *  to object array, e.g. sorting the pointers directly based on their
         *  dereferenced values from the compare function.
         *
         * ---------------------------------------------------------------------- */

        void quicksort(TYPE **a, const int from, const int to,
                       int (*comparefn)(TYPE *, TYPE *))  {

            // Insertion sort is faster for short arrays
            if (to - from <= 10) {
                insertionsort(a, from, to, (*comparefn));
                return;
            }

            // find a pivot as the median of first, last and middle element

            TYPE *v0 = a[from];
            TYPE *v1 = a[to - 1];
            int middle_index = from + ((to - from) >> 1);
            TYPE *v2 = a[middle_index];

            // compare v0 and v1.
            int c01 = (*comparefn)(v0,v1);
            if (c01 > 0) {
                // v1 < v0, swap
                TYPE *temp = v0;
                v0 = v1;
                v1 = temp;
            } // v0 <= v1

            // compare v0 and v2
            int c02 = (*comparefn)(v0,v2);
            if (c02 > 0) {
                // v2 <= v0 <= v1
                TYPE *temp = v0;
                v0 = v2;
                v2 = v1;
                v1 = temp;
            } else {
                // v0 <= v1 && v0 < v2
                // compare v1 and v2
                int c12 = (*comparefn)(v1,v2);
                if (c12 > 0) {
                    // v0 <= v2 < v1
                    TYPE *temp = v1;
                    v1 = v2;
                    v2 = temp;
                }
            }

            // v0 <= v1 <= v2
            a[from] = v0;
            a[to - 1] = v2;
            TYPE *pivot = v1;
            int low_end = from + 1; // Upper bound of elements lower than pivot.
            int high_start = to - 1; // Lower bound of elements greater than pivot.
            a[middle_index] = a[low_end];
            a[low_end] = pivot;

            // From low_end to i are elements equal to pivot.
            // From i to high_start are elements that haven't been compared yet.

            for (int i = low_end + 1; i < high_start; i++) {
                TYPE *element = a[i];
                int order = (*comparefn)(element, pivot);
                if (order < 0) {
                    swap(a, i, low_end);
                    low_end++;
                } else if (order > 0) {
                    do {
                        high_start--;
                        if (high_start == i) goto loopDone;
                        TYPE *top_elem = a[high_start];
                        order = (*comparefn)(top_elem, pivot);
                    } while (order > 0);
                    swap(a, i, high_start);
                    if (order < 0) {
                        swap(a, i, low_end);
                        low_end++;
                    }
                }
            }

            // This is a label called by goto to break out of the
            // nested loops. Deal with it.
        loopDone:
            quicksort(a, from, low_end, (*comparefn));
            quicksort(a, high_start, to, (*comparefn));
        }

        void insertionsort(TYPE **a, const int from, const int to,
                           int (*comparefn)(TYPE *, TYPE *)) {

            for (int i = from + 1; i < to; i++) {
                TYPE *element = a[i];
                int j;
                for (j = i - 1; j >= from; j--) {
                    TYPE *temp = a[j];
                    int order = (*comparefn)(temp, element);
                    if (order > 0) {
                        a[j + 1] = temp;
                    } else {
                        break;
                    }
                }
                a[j + 1] = element;
            }
        }

        /**
         * @brief Identify the unique values in the array a
         *
         * Identify unique values, put those "n" values at the
         * beginning of the array "a". The number of unique values
         * is returned in the variable n.
         *
         * The unique values are returned sorted.
         *
         * @param a the array to sort
         * @param from index to start at
         * @param to index to end at
         * @param comparefn the function used to compare the values
         * @param n the number of unique values identified
         */

        void unique(TYPE **a, const int from, const int to,
                    int (*comparefn)(TYPE *, TYPE *), int &n) {

            // First, sort the array
            quicksort(a, from, to, (*comparefn));

            // Now, go over the array and identify the unique values
            n = 0;
            int i = from;
            while (i < to) {

                // Swap the values so we never lose any values
                swap(a,n,i++);

                // skip over array while values are the same
                while (i < to && (*comparefn)(a[n], a[i]) == 0)
                    i++;

                n++;

            }
        }

        /**
         * @brief double sort, sort using function 1 then function 2
         *
         * Do multiple sorts, first sort by function one, then use
         * function 1 to identify the unique values of the array,
         * then sort the subsets of the array using function 2.
         *
         * e.g. to sort an array of atoms by chain and then resid
         *
         * sort.quicksort(atom2,0,as->natom,sort.compare_atom_chain,sort.compare_atom_resid);
         *
         * @param a array of pointers to sort
         * @param from starting index
         * @param to ending index
         * @param comparefn1 the first compare function
         * @param comparefn2 the second compare function
         */

        void quicksort(TYPE **a, const int from, const int to,
                       int (*comparefn1)(TYPE *, TYPE *),
                       int (*comparefn2)(TYPE *, TYPE *))  {

            // Get the number of unique values for the first compare function
            int nunique = 0;
            unique(a,from,to,(*comparefn1),nunique);

            // re-Sort using the first function
            quicksort(a,from,to,(*comparefn1));

            //Now, get the sub-offsets
            int off[nunique + 1];

            int i = from, n = 0, idx = 0;

            while (i < to) {

                idx = i;
                off[n] = i++;

                while (i < to && (*comparefn1)(a[idx], a[i]) == 0)
                    i++;

                n++;
            }

            off[n] = to;

            //Sort array using offsets and second compare function
            for (i = 0; i < nunique; i++)
                quicksort(a,off[i],off[i+1],(*comparefn2));
        }
    };
}

#endif
