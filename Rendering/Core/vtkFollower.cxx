/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkFollower.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "vtkFollower.h"

#include "vtkMatrix4x4.h"
#include "vtkObjectFactory.h"
#include "vtkProp3DFollower.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkTexture.h"
#include "vtkTransform.h"

vtkStandardNewMacro(vtkFollower);

//----------------------------------------------------------------------
// Creates a follower with no camera set
vtkFollower::vtkFollower()
{
  this->Device = vtkActor::New();
  this->Follower = vtkProp3DFollower::New();
}

//----------------------------------------------------------------------
vtkFollower::~vtkFollower()
{
  this->Device->Delete();
  this->Follower->Delete();
}

//----------------------------------------------------------------------
void vtkFollower::SetCamera(vtkCamera* camera)
{
  this->Follower->SetCamera(camera);
}

//----------------------------------------------------------------------
vtkCamera* vtkFollower::GetCamera()
{
  return this->Follower->GetCamera();
}

//----------------------------------------------------------------------------
void vtkFollower::ComputeMatrix()
{
  this->Follower->ComputeMatrix();
  // For backward compatibility.
  double elements[16];
  this->Follower->GetMatrix(elements);
  this->Matrix->DeepCopy(elements);
}

//----------------------------------------------------------------------
void vtkFollower::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Prop3D Follower:\n";
  this->Follower->PrintSelf(os,indent.GetNextIndent());
}

//----------------------------------------------------------------------
int vtkFollower::RenderOpaqueGeometry(vtkViewport *vp)
{
  if ( ! this->Mapper )
    {
    return 0;
    }

  if (!this->Property)
    {
    // force creation of a property
    this->GetProperty();
    }

  if (this->GetIsOpaque())
    {
    vtkRenderer *ren = static_cast<vtkRenderer *>(vp);
    this->Render(ren);
    return 1;
    }
  return 0;
}

//-----------------------------------------------------------------------------
int vtkFollower::RenderTranslucentPolygonalGeometry(vtkViewport *vp)
{
  if ( ! this->Mapper )
    {
    return 0;
    }

  if (!this->Property)
    {
    // force creation of a property
    this->GetProperty();
    }

  if (!this->GetIsOpaque())
    {
    vtkRenderer *ren = static_cast<vtkRenderer *>(vp);
    this->Render(ren);
    return 1;
    }
  return 0;
}

//-----------------------------------------------------------------------------
void vtkFollower::ReleaseGraphicsResources(vtkWindow *w)
{
  this->Follower->ReleaseGraphicsResources(w);
  this->Superclass::ReleaseGraphicsResources(w);
}

//-----------------------------------------------------------------------------
// Description:
// Does this prop have some translucent polygonal geometry?
int vtkFollower::HasTranslucentPolygonalGeometry()
{
  if ( ! this->Mapper )
    {
    return 0;
    }
  // make sure we have a property
  if (!this->Property)
    {
    // force creation of a property
    this->GetProperty();
    }

  // is this actor opaque ?
  return !this->GetIsOpaque();
}

//-----------------------------------------------------------------------------
// This causes the actor to be rendered. It, in turn, will render the actor's
// property and then mapper.
void vtkFollower::Render(vtkRenderer *ren)
{
  this->Property->Render(this, ren);

  this->Device->SetProperty (this->Property);
  this->Property->Render(this, ren);
  if (this->BackfaceProperty)
    {
    this->BackfaceProperty->BackfaceRender(this, ren);
    this->Device->SetBackfaceProperty(this->BackfaceProperty);
    }

  /* render the texture */
  if (this->Texture)
    {
    this->Texture->Render(ren);
    }

  // make sure the device has the same matrix
  this->ComputeMatrix();

  this->Device->Render(ren,this->Mapper);
}

//----------------------------------------------------------------------
void vtkFollower::ShallowCopy(vtkProp *prop)
{
  vtkFollower *f = vtkFollower::SafeDownCast(prop);
  if ( f != NULL )
    {
    this->Follower->ShallowCopy(f->Follower);
    }

  // Now do superclass
  this->Superclass::ShallowCopy(prop);
}



