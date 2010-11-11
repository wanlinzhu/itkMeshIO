/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOBJMeshIOFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2010-08-01 18:47:11 $
  Version:   $Revision: 0.01 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkCreateObjectFunction.h"
#include "itkOBJMeshIO.h"
#include "itkOBJMeshIOFactory.h"
#include "itkVersion.h"

namespace itk
{
void OBJMeshIOFactory::PrintSelf(std::ostream &, Indent) const
{}

OBJMeshIOFactory::OBJMeshIOFactory()
{
  this->RegisterOverride( "itkMeshIOBase",
                         "itkOBJMeshIO",
                         "OBJ Mesh IO",
                         1,
                         CreateObjectFunction< OBJMeshIO >::New() );
}

OBJMeshIOFactory::~OBJMeshIOFactory()
{}

const char * OBJMeshIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char * OBJMeshIOFactory::GetDescription() const
{
  return "OBJ Mesh IO Factory, allows the loading of OBJ mesh into insight";
}
} // end namespace itk
