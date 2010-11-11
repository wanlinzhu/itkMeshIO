/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOFFMeshIOFactory.cxx,v $
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
#include "itkOFFMeshIO.h"
#include "itkOFFMeshIOFactory.h"
#include "itkVersion.h"

namespace itk
{
void OFFMeshIOFactory::PrintSelf(std::ostream &, Indent) const
{}

OFFMeshIOFactory::OFFMeshIOFactory()
{
  this->RegisterOverride( "itkMeshIOBase",
                         "itkOFFMeshIO",
                         "OFF Mesh IO",
                         1,
                         CreateObjectFunction< OFFMeshIO >::New() );
}

OFFMeshIOFactory::~OFFMeshIOFactory()
{}

const char * OFFMeshIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char * OFFMeshIOFactory::GetDescription() const
{
  return "OFF Mesh IO Factory, allows the loading of OFF mesh into insight";
}
} // end namespace itk
