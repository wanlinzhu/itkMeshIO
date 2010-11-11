/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOFFMeshIOFactory.h,v $
  Language:  C++
  Date:      $Date: 2010-08-01 13:13:11 $
  Version:   $Revision: 0.01 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOFFMeshIOFactory_h
#define __itkOFFMeshIOFactory_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkObjectFactoryBase.h"
#include "itkMeshIOBase.h"

namespace itk
{
/** \class OFFMeshIOFactory
   * \brief Create instances of OFFMeshIO objects using an object factory.
   */
class ITK_EXPORT OFFMeshIOFactory:public ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef OFFMeshIOFactory           Self;
  typedef ObjectFactoryBase          Superclass;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char * GetITKSourceVersion(void) const;

  virtual const char * GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OFFMeshIOFactory, ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
    {
    OFFMeshIOFactory::Pointer offFactory = OFFMeshIOFactory::New();

    ObjectFactoryBase::RegisterFactory(offFactory);
    }

protected:
  OFFMeshIOFactory();
  ~OFFMeshIOFactory();

  virtual void PrintSelf(std::ostream & os, Indent indent) const;

private:
  OFFMeshIOFactory(const Self &); // purposely not implemented
  void operator=(const Self &);   // purposely not implemented
};
} // end namespace itk

#endif
