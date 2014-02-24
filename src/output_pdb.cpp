// -*-c++-*-

// *hd +------------------------------------------------------------------------------------+
// *hd |  This file is part of Coiled-Coil Builder.                                         |
// *hd |                                                                                    |
// *hd |  Coiled-Coil Builder is free software: you can redistribute it and/or modify       |
// *hd |  it under the terms of the GNU General Public License as published by              |
// *hd |  the Free Software Foundation, either version 3 of the License, or                 |
// *hd |  (at your option) any later version.                                               |
// *hd |                                                                                    |
// *hd |  Coiled-Coil Builder is distributed in the hope that it will be useful,            |
// *hd |  but WITHOUT ANY WARRANTY without even the implied warranty of                     |
// *hd |  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                     |
// *hd |  GNU General Public License for more details.                                      |
// *hd |                                                                                    |
// *hd |  You should have received a copy of the GNU General Public License                 |
// *hd |  along with Coiled-Coil Builder.  If not, see <http:www.gnu.org/licenses/>.        |
// *hd +------------------------------------------------------------------------------------+

// *hd | If you intend to use this software for your research, please cite:
// *hd | and inform Chris MacDermaid <chris.macdermaid@gmail.com> of any pending publications.

// *hd | Copyright (c) 2012,2013,2014 by Chris M. MacDermaid <chris.macdermaid@gmail.com>
// *hd | and Jeffery G. Saven <saven@sas.upenn.edu>


/**
 * @file   output_pdb.cpp
 * @author CVB <chris@CirithUngol>
 * @date   Sun Jun 26 14:41:16 2011
 *
 * @brief  pdb output style routine.
 *
 * Outputs the most probable rotamer of the most probable monomer type
 * in the pdb file format
 *
 * http://deposit.rcsb.org/adit/docs/pdb_atom_format.html
 *
 *
 * COLUMNS        DATA TYPE       CONTENTS
 * --------------------------------------------------------------------------------
 *  1 -  6        Record name     "ATOM  "
 *  7 - 11        Integer         Atom serial number.
 * 13 - 16        Atom            Atom name.
 * 17             Character       Alternate location indicator.
 * 18 - 20        Residue name    Residue name.
 * 22             Character       Chain identifier.
 * 23 - 26        Integer         Residue sequence number.
 * 27             AChar           Code for insertion of residues.
 * 31 - 38        Real(8.3)       Orthogonal coordinates for X in Angstroms.
 * 39 - 46        Real(8.3)       Orthogonal coordinates for Y in Angstroms.
 * 47 - 54        Real(8.3)       Orthogonal coordinates for Z in Angstroms.
 * 55 - 60        Real(6.2)       Occupancy.
 * 61 - 66        Real(6.2)       Temperature factor (Default = 0.0).
 * 73 - 76        LString(4)      Segment identifier, left-justified.
 * 77 - 78        LString(2)      Element symbol, right-justified.
 * 79 - 80        LString(2)      Charge on the atom.
 *              1         2         3         4         5         6         7         8
 *     12345678901234567890123456789012345678901234567890123456789012345678901234567890
 *     ATOM    145  N   VAL A  25      32.433  16.336  57.540  1.00 11.92      A1   N
 *     ATOM    146  CA  VAL A  25      31.132  16.439  58.160  1.00 11.85      A1   C
 */

#include "stdio.h"
#include "string.h"
#include "output_pdb.h"
#include "error.h"
#include "universe.h"
#include "memory.h"
#include "atom.h"
#include "group.h"
#include "site.h"
#include "domain.h"
#include "bitmask.h"

/**
 * @def BLEN
 *
 * Length of path name or genaric buffer
 */

#define BLEN 200

using namespace CCB_NS;

/**
 * The OutputPDB Constructor
 *
 * @param ccb The ccb pointer
 * @param narg number of arguments passed
 * @param arg the arguments passed
 */

OutputPDB::OutputPDB(CCB *ccb, int narg, const char **arg) :
        Output(ccb, narg, arg) {
    // Check to see that we have a legit style and the format is correct.
    if (strcmp(style, "PDB") != 0 && narg < 4)
        error->one(FLERR, "Illegal output PDB command");

    // Set the bitmask for atoms to output
    if (narg > 4) {
        int imask = bitmask->find(arg[4]);

        if (imask < 0)
            error->one(FLERR, "output_pdb: bitmask doesn't exist");

        mask = bitmask->mask[imask];

    } else {
        // By default, output all atoms
        mask = 1;
    }
}

/**
 * Opens the PDB file
 *
 */

int OutputPDB::init_style() {

    return CCB_OK;

}

/**
 * Reads the PDB file and stores the atoms.
 *
 */

int OutputPDB::write_style() {

    int serial = 1;
    Atom *atom = NULL;

    if (universe->me == 0) {
        // Open output file
        openfile();

        if (fp) {

            char *name = new char[10];
            char *type = new char[10];
            char *seg = new char[10];
            char *chain = new char[10];

            // Loop over all sites
            for (int i = 0; i < domain->nsite; i++) {

                for (int j = 0; j < domain->site[i]->fixed_atoms->natom; j++) {
                    atom = domain->site[i]->fixed_atoms->atom[j];

                    // Copy stings
                    strcpy(name, atom->name);
                    strcpy(type, atom->group->type);
                    strcpy(seg, atom->site->seg);
                    strcpy(chain, atom->site->chain);

                    // Shorten the strings
                    chomp(name, 5);
                    chomp(type, 3);
                    chomp(seg, 4);
                    chomp(chain, 1);

                    // Check mask and write out atoms
                    if (atom->mask & mask)
                        fprintf(fp, "ATOM  %5d  %-4s%-3s %1s%4d    %8.3lf%8.3lf%8.3lf%6.2lf%6.2lf      %-4s%2s\n", serial++, name, type, chain,
                                atom->site->resid, atom->x, atom->y, atom->z, atom->o, atom->b, seg, atom->element);
                }

            }

            fprintf(fp, "END\n");

            if (universe->me == 0 && error->verbosity_level >= 3)
                fprintf(screen, "Output PDB: %d atoms, written to %s\n", serial - 1, filename);

            delete[] name;
            delete[] type;
            delete[] chain;
            delete[] seg;

        } else {
            char str[128];
            sprintf(str, "Unable to open %s for writing", filename);
            return error->one(FLERR, str);
        }

        closefile();
    }

    return CCB_OK;

}

/**
 * @brief Shorten a string s to n characters
 *
 * Assume ths string isn't null terminated
 *
 * @param s string to be shortened
 * @param n length to shortned string s to
 */

void OutputPDB::chomp(char *s, int n) {

    int len = strlen(s);

    if (len == 0)
        return;

    int i = 0;

    while (i < len && i < n) {
        *(s++);
        i++;
    }

    *s++ = '\0';
}

