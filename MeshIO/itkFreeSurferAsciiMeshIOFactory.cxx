/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFreeSurferAsciiMeshIOFactory.cxx,v $
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
#include "itkFreeSurferAsciiMeshIO.h"
#include "itkFreeSurferAsciiMeshIOFactory.h"
#include "itkVersion.h"

namespace itk
{
void FreeSurferAsciiMeshIOFactory::PrintSelf(std::ostream &, Indent) const
{}

FreeSurferAsciiMeshIOFactory::FreeSurferAsciiMeshIOFactory()
{
  this->RegisterOverride( "itkMeshIOBase",
                         "itkFreeSurferAsciiMeshIO",
                         "Freesurfer Mesh IO",
                         1,
                         CreateObjectFunction< FreeSurferAsciiMeshIO >::New() );
}

FreeSurferAsciiMeshIOFactory::~FreeSurferAsciiMeshIOFactory()
{}

const char * FreeSurferAsciiMeshIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char * FreeSurferAsciiMeshIOFactory::GetDescription() const
{
  return "FreeSurfer ASCII Mesh IO Factory, allows the loading of FreeSurfer Ascii mesh into insight";
}
} // end namespace itk
