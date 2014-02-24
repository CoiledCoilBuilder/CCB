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


/* -*- c -*- */
/* header for plugin interface */
#ifndef PLUGIN_H
#define PLUGIN_H

#define PLUGIN_ABI 1

#define PLUGIN_OK 1
#define PLUGIN_FAIL 0

#ifdef __cplusplus
extern "C" 
{
#endif

    typedef struct 
    {
        int abi;               /* ABI version of the plugin interface */
        int version;           /* Version of the plugin itself */
        const char *name;      /* Name of the contained class */
        const char *desc;      /* Description of the contained class */
        const char *author;    /* Name of the plugin author */
        void *handle;          /* place to store handle of DSO */

        void *(*create_class)(const char *); /* function pointer for the constructor */
        void (*destroy_class)(void *); /* function pointer for the destructor */
    } plugin_t;

     /**
      * @brief A common struct between COMPASS and the plugin for sharing atomic data
      */

     typedef struct
     {

          // Atom based quantities
          int serial;
          char *name;
          char *type;
          char *element;

          double x;
          double y;
          double z;
          double o;
          double b;

          double mass;
          double charge;
          double rmin;
          double rmin14;
          double epsilon;
          double epsilon14;
          double user3;
          double user4;

          //Group based quantities
          char *groupname;
          char *grouptype;
     
          double totalcharge;
          double probability;

          // Site based quantities
          unsigned int resid;
          char *chain;
          char *seg;
          double *com_cart;
          double *com_frac;

     } pluginatomic_t;


    extern int register_plugin(const char *path, plugin_t *ptr);
    extern void unregister_plugin(plugin_t *ptr);
#ifdef __cplusplus
}
#endif

#endif
