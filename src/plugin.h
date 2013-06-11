// -*-c++-*-

  // +------------------------------------------------------------------------------------+ 
  // |  This file is part of Coiled-Coil Builder.                                         | 
  // |                                                                                    | 
  // |  Coiled-Coil Builder is free software: you can redistribute it and/or modify       | 
  // |  it under the terms of the GNU General Public License as published by              | 
  // |  the Free Software Foundation, either version 3 of the License, or                 | 
  // |  (at your option) any later version.                                               | 
  // |                                                                                    | 
  // |  Coiled-Coil Builder is distributed in the hope that it will be useful,            | 
  // |  but WITHOUT ANY WARRANTY without even the implied warranty of                     | 
  // |  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                     | 
  // |  GNU General Public License for more details.                                      | 
  // |                                                                                    | 
  // |  You should have received a copy of the GNU General Public License                 | 
  // |  along with Coiled-Coil Builder.  If not, see <http:www.gnu.org/licenses/>.        | 
  // |                                                                                    | 
  // |   *cr                                                                              | 
  // |   *cr            (C) Copyright 1995-2013 The Board of Trustees of the              | 
  // |   *cr                        University of Pennsylvania                            | 
  // |   *cr                         All Rights Reserved                                  | 
  // |   *cr                                                                              | 
  // +------------------------------------------------------------------------------------+ 

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
