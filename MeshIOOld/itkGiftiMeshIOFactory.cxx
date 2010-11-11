/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGiftiMeshIOFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2010-10-18 02:41:03 $
  Version:   $Revision: 0.05 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkCreateObjectFunction.h"
#include "itkGiftiMeshIO.h"
#include "itkGiftiMeshIOFactory.h"
#include "itkVersion.h"

namespace itk
{
void GiftiMeshIOFactory::PrintSelf(std::ostream &, Indent) const
{}

GiftiMeshIOFactory::GiftiMeshIOFactory()
{
  this->RegisterOverride( "itkMeshIOBase",
                         "itkGiftiMeshIO",
                         "Gifti Mesh IO",
                         1,
                         CreateObjectFunction< GiftiMeshIO >::New() );
}

GiftiMeshIOFactory::~GiftiMeshIOFactory()
{}

const char * GiftiMeshIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char * GiftiMeshIOFactory::GetDescription() const
{
  return "Gifti MeshIO Factory, allows the loading of Gifti meshs into insight";
}
} // end namespace itk
