/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVTKPolyDataMeshIOFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2010-08-01 13:13:11 $
  Version:   $Revision: 0.01 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkCreateObjectFunction.h"
#include "itkVTKPolyDataMeshIO.h"
#include "itkVTKPolyDataMeshIOFactory.h"
#include "itkVersion.h"

namespace itk
{
void VTKPolyDataMeshIOFactory::PrintSelf(std::ostream &, Indent) const
{}

VTKPolyDataMeshIOFactory::VTKPolyDataMeshIOFactory()
{
  this->RegisterOverride( "itkMeshIOBase",
                         "itkVTKPolyDataMeshIO",
                         "VTK Polydata IO",
                         1,
                         CreateObjectFunction< VTKPolyDataMeshIO >::New() );
}

VTKPolyDataMeshIOFactory::~VTKPolyDataMeshIOFactory()
{}

const char * VTKPolyDataMeshIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char * VTKPolyDataMeshIOFactory::GetDescription() const
{
  return "VTK MeshIO Factory, allows the loading of VTK polydata into insight";
}
} // end namespace itk
