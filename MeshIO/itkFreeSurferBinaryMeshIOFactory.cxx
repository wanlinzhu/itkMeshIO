/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFreeSurferBinaryMeshIOFactory.cxx,v $
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
#include "itkFreeSurferBinaryMeshIO.h"
#include "itkFreeSurferBinaryMeshIOFactory.h"
#include "itkVersion.h"

namespace itk
{
void FreeSurferBinaryMeshIOFactory::PrintSelf(std::ostream &, Indent) const
{}

FreeSurferBinaryMeshIOFactory::FreeSurferBinaryMeshIOFactory()
{
  this->RegisterOverride( "itkMeshIOBase",
                         "itkFreeSurferBinaryMeshIO",
                         "Freesurfer Binary Mesh IO",
                         1,
                         CreateObjectFunction< FreeSurferBinaryMeshIO >::New() );
}

FreeSurferBinaryMeshIOFactory::~FreeSurferBinaryMeshIOFactory()
{}

const char * FreeSurferBinaryMeshIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char * FreeSurferBinaryMeshIOFactory::GetDescription() const
{
  return "FreeSurfer BINARY Mesh IO Factory, allows the loading of FreeSurfer Binary mesh into insight";
}
} // end namespace itk
