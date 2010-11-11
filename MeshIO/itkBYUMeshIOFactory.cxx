/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBYUMeshIOFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2010-08-06 13:13:11 $
  Version:   $Revision: 0.01 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkBYUMeshIOFactory.h"
#include "itkCreateObjectFunction.h"
#include "itkBYUMeshIO.h"
#include "itkVersion.h"

namespace itk
{
void BYUMeshIOFactory::PrintSelf(std::ostream &, Indent) const
{}

// /////////////////////////////////////////////////////////////////////
BYUMeshIOFactory::BYUMeshIOFactory()
{
  this->RegisterOverride( "itkMeshIOBase",
                         "itkBYUMeshIO",
                         "BYU Mesh IO",
                         1,
                         CreateObjectFunction< BYUMeshIO >::New() );
}

// /////////////////////////////////////////////////////////////////////
BYUMeshIOFactory::~BYUMeshIOFactory()
{}

// /////////////////////////////////////////////////////////////////////
const char * BYUMeshIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

// /////////////////////////////////////////////////////////////////////
const char * BYUMeshIOFactory::GetDescription() const
{
  return "BYU Mesh IO Factory, allows the loading of BYU mesh into insight";
}

// /////////////////////////////////////////////////////////////////////
} // end namespace itk
