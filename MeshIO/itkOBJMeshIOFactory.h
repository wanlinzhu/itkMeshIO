/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOBJMeshIOFactory.h,v                            $
  Language:  C++
  Date:      $Date: 2010-08-01 13:13:11                                   $
  Version:   $Revision: 0.02 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkOBJMeshIOFactory_h
#define __itkOBJMeshIOFactory_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkObjectFactoryBase.h"
#include "itkMeshIOBase.h"

namespace itk
{
/** \class OBJMeshIOFactory
   * \brief Create instances of OBJMeshIO objects using an object factory.
   */
class ITK_EXPORT OBJMeshIOFactory:public ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef OBJMeshIOFactory           Self;
  typedef ObjectFactoryBase          Superclass;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char * GetITKSourceVersion(void) const;

  virtual const char * GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OBJMeshIOFactory, ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
    {
    OBJMeshIOFactory::Pointer objFactory = OBJMeshIOFactory::New();

    ObjectFactoryBase::RegisterFactory(objFactory);
    }

protected:
  OBJMeshIOFactory();
  ~OBJMeshIOFactory();

  virtual void PrintSelf(std::ostream & os, Indent indent) const;

private:
  OBJMeshIOFactory(const Self &); // purposely not implemented
  void operator=(const Self &);   // purposely not implemented
};
} // end namespace itk

#endif
