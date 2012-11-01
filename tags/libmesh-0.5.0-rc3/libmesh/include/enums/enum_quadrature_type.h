// $Id: enum_quadrature_type.h,v 1.4 2005-02-22 22:17:30 jwpeterson Exp $

// The libMesh Finite Element Library.
// Copyright (C) 2002-2005  Benjamin S. Kirk, John W. Peterson
  
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#ifndef __enum_quadrature_type_h__
#define __enum_quadrature_type_h__

// C++ includes

// Local includes


// ------------------------------------------------------------
// enum QuadratureType definition
namespace libMeshEnums {
  
  /**
   * Defines an \p enum for currently available quadrature rules.
   */
  enum QuadratureType {QGAUSS      = 0,

		       QJACOBI_1_0 = 1,
		       QJACOBI_2_0 = 2,

		       QSIMPSON    = 3,
		       QTRAP       = 4,
		       QGRID       = 5,

		       QCLOUGH     = 21,

		       INVALID_Q_RULE = 127};
}

using namespace libMeshEnums;



#endif



