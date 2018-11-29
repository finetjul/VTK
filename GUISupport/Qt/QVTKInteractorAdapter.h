/*=========================================================================

  Program:   Visualization Toolkit
  Module:    QVTKInteractorAdapter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

/*=========================================================================

  Copyright 2004 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
  license for use of this work by or on behalf of the
  U.S. Government. Redistribution and use in source and binary forms, with
  or without modification, are permitted provided that this Notice and any
  statement of authorship are reproduced on all copies.

=========================================================================*/

/*========================================================================
 For general information about using VTK and Qt, see:
 http://www.trolltech.com/products/3rdparty/vtksupport.html
=========================================================================*/

// .NAME QVTKInteractorAdapter - Handle Qt events.
// .SECTION Description
// QVTKInteractor handles relaying Qt events to VTK.

#ifndef Q_VTK_INTERACTOR_ADAPTER_H
#define Q_VTK_INTERACTOR_ADAPTER_H

#include "vtkGUISupportQtModule.h" // For export macro
#include "QVTKWin32Header.h"
#include <QtCore/QObject>
#include <QtCore/QMap>

class vtkRenderWindowInteractor;
class QEvent;

// .NAME QVTKInteractorAdapter - A QEvent translator.
// .SECTION Description
// QVTKInteractorAdapter translates QEvents and send them to a
// vtkRenderWindowInteractor.
class VTKGUISUPPORTQT_EXPORT QVTKInteractorAdapter : public QObject
{
  Q_OBJECT
public:
  // Description:
  // Constructor: takes QObject parent
  QVTKInteractorAdapter(QObject* parent);

  // Description:
  // Destructor
  ~QVTKInteractorAdapter() override;

  // Description:
  // Set the device pixel ration, this defaults to 1, but in Qt 5 can be 2.
  void SetDevicePixelRatio(int ratio, vtkRenderWindowInteractor* iren = nullptr);
  int GetDevicePixelRatio() { return this->DevicePixelRatio; }

  // Description:
  // Adds a new mapping from Qt point id to VTK pointer index if needed.
  // Returns the inserted VTK index or -1 in case of failure.
  int UpdateTouchPointIdMap(const int qtPointId);
  int GetPointerIndex(int qtPointId) const;

  // Description:
  // Process a QEvent and send it to the interactor
  // returns whether the event was recognized and processed
  bool ProcessEvent(QEvent* e, vtkRenderWindowInteractor* iren);

protected:
  int AccumulatedDelta;
  int DevicePixelRatio;
  QMap<int, int> TouchPointIdMap;
};

#endif
