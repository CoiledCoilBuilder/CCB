/**
 * @file   site.h
 * @author Chris Von Bargen <cvb@magrathea.chem.upenn.edu>
 * @date   Nov 14 2011
 *
 * @brief  Site, describing a location in the domain
 *
 * Class to describe a specific site in the domain.
 * Here contains descriptive fields (name, resid, etc.) as well as
 * a static group fixed common atoms and a collection of corresponding groups (rotamers)
 *
 */

#ifndef SCADS_SITE_H
#define SCADS_SITE_H

#include "pointers.h"

namespace SCADS_NS {
    class Site: protected Pointers {

    public:

        // Constructor and Destructor
        Site(class SCADS *, int i); /**< Site constructor */
        ~Site(); /**< Site deconstructor */
        Site(const Site &);   /**< Copy Constructor */
        Site& operator=(Site const &); /**< assignment operator */

        class Group *fixed_atoms; /**< Static site member of fixed, common atoms */
        class Group **rotamer; /**< list of site members corresponding to fixed atoms (rotamers) */
        class Group *most_probable; /**< The most probable group at this site */

        int nrotamer; /**< Total number of rotamers */

        int id; /**< Internal indexing counter */
        unsigned int resid; /**< Site/Resid  number */
        char chain[10]; /**< Chain name */
        char seg[10]; /**< Segment Identifier */
        double com_cart[3];
        double com_frac[3];

        // Functions to manage rotamers (dynamic groups);
        int add_rotamer();
        int add_rotamer(Group *res);
        int add_rotamer(Group &g); /**< add a group using the copy-constructor */
        void delete_rotamer(int id);
        int find_rotamer(int id);
        void delete_all_rotamers();
        void put_com_cart(double v[3]); /**< Residue position in Cartesian coordinates. */
        void put_com_frac(double v[3]); /**< Residue position in fractional coordinates. */

        int build_site(); /**< Builds all rotamers at this site */
        int mask;           /** < The site mask, used for build specifications. All atoms in this site inherit the site mask */

        void make_global();     /**< Make all atoms in this site global */
        void get_most_probable(); /**< determins the most probable monomer at this site */
        double memory_usage();

    private:
        bigint ngroup_iter; /** < The unique site id iterator, this should only be increased */
        int maxrotamer; /**< Maximum number of rotamers based on currently allocated space  */


    };
}

#endif

