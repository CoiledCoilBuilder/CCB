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


// -*- c++ -*-

#ifndef _CCB_COMPASS_H_
#define _CCB_COMPASS_H_

#include <vector>
#include "plugin_baseclass.h"
#include "ccb.h"
#include "plugin.h"

extern "C"
{
    void *create_ccb(const char *);
    void destroy_ccb(void *);
    int plugin_init(plugin_t *);
}

namespace SCADS_NS {

class CCB_Compass : public Plugin_BaseClass
{
  public:
    CCB_Compass(const char *, const char *);
    virtual ~CCB_Compass();

  protected:
    CCB_Compass() {};

  protected:
    class CCB_NS::CCB *ccb;

  public:
    virtual int init(const int, const char **);
    virtual int compute(int);
    virtual double single(int,int);
    virtual bool get_atomic_data(std::vector<pluginatomic_t> &);

   private:
     int init_done;

};

}

#endif /* _CCB_COMPASS_H_ */
