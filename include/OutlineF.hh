/*=========================================================================

  Program:   Visualization Library
  Module:    OutlineF.hh
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

This file is part of the Visualization Library. No part of this file
or its contents may be copied, reproduced or altered in any way
without the express written consent of the authors.

Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen 1993, 1994 

=========================================================================*/
// .NAME vlOutlineFilter - create wireframe outline for arbitrary data set
// .SECTION Description
// vlOutlineFilter is a filter that generates a wireframe outline of any 
// data set. The outline consists of the twelve edges of the bounding box.

#ifndef __vlOutlineFilter_h
#define __vlOutlineFilter_h

#include "DS2PolyF.hh"

class vlOutlineFilter : public vlDataSetToPolyFilter
{
public:
  vlOutlineFilter() {};
  ~vlOutlineFilter() {};
  char *GetClassName() {return "vlOutlineFilter";};

protected:
  void Execute();
};

#endif


