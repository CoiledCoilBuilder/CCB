/**
 * @file   domain.h
 * @author Chris Von Bargen <cvb@magrathea.chem.upenn.edu>
 * @date   Nov 17 2011
 *
 * @brief  Domain, storage of the entire molecular complex
 *
 * Class to describe a specific molecular domain.
 * Here comprised of a series of sites. There is also a series of
 * for residue/monomer types, describing connectivity, etc. of
 * allowable residues in the domain (used to then construct the
 * coordinates in the domain).
 *
 */

#ifndef SCADS_DOMAIN_H
#define SCADS_DOMAIN_H

#include "pointers.h"

namespace SCADS_NS {
    class Domain: protected Pointers {

    public:

        // Constructor and Destructor
        Domain(class SCADS *); /**< Domain constructor */
        ~Domain(); /**< Domain deconstructor */

        ///Thermodynamic Variables (These should be public??)
        double S;
        double U;
        double T;
        double B;
        double F;

        class Site **site; /**< list of sites */
        int nsite; /**< Total number of sites */
        int maxsite; /**< Maximum number of sites based on currently allocated space  */

        class Restype **restype; /**< Allowable residue/monomer types in domain */
        int nrestype; /**< Total number of restypes */
        int maxrestype; /**< Maximum number of restypes based on currently allocated space  */

        class Restype **captype; /**< Allowable cap types in domain */
        int ncaptype; /**< Total number of caps */
        int maxcaptype; /**< Maximum number of caps based on currently allocated space  */

        bool autoAngle;
        bool autoDihedral;
        class Restype *paratype; /**< Container for all parameter values */

        class Type *type; /**< Types read in from the typefile */

        // Global atom identifiers (gids)
        class Gid *gid; /** < Instance to manage atom GIDs */
        void make_global(int mask); /**< Make all atoms in the domain global, called after the domain is built */

        //domain management:
        void reset(); /**< clear the domain of all sites */

        // Functions to manage sites
        int add_site(); /**< add an empty site */
        int add_site(unsigned int resid, const char *chain); /**< Add a site to the domain, set resid,chain */
        int add_site(const Site &s); /**< add a site to the domain using the copy constructor */
        void delete_site(int id);
        int find_site(int id);
        int find_site(unsigned int resid, const char *chain);
        void writeDomain();

        // Functions to manage residue/monomer types
        int add_restype();
        void delete_restype(int id);
        int find_restype(int id);
        int find_restype_name(const char *name);
        void clone_restype(const char *name, class Restype *&clone);

        // Functions to manage cap types
        int add_captype();
        void delete_captype(int id);
        int find_captype(int id);
        int find_captype_name(const char *name);
        int create_capped_restype(const char *resi, const char *cap);

        void set_type(class Group* g); /**< Set the atom types based on those in restypes */
        void set_params(class Group* g); /**< Set the atom parameters contained in paratype */

        int get_chains(char**& chain_list); /**< Get a list of sorted, unique chains */
        int get_resids(const char *chain, int*& resid_list); /**< Get a list of sorted resids for a given chain */

        void load_capped_restypes(const char *chain);
        void get_site_restype(const char *chain, int resid, const char *type, class Restype *&rescap);

        void load_domain(); /**< Load capped restyes and rotamer library bins */
        void build_domain(const char *maskname); /**< Build the domain based on initial structure, residue types, and rotamer libraries */

        void get_most_probable(); /**< identifies the most probable monomer type at each site */
        double memory_usage(); /**< Calculate domain memory usage */

    private:
        bigint nsite_iter; /** < The unique site id iterator, this should only be increased, bigint because it remembers every atom ever created */
    };
}

#endif

