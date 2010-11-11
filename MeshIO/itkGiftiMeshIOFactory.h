/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGiftiMeshIOFactory.h $
  Language:  C++
  Date:      $Date: 2010-10-18 21:11:48 $
  Version:   $Revision: 0.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGiftiMeshIOFactory_h
#define __itkGiftiMeshIOFactory_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkMeshIOBase.h"
#include "itkObjectFactoryBase.h"

namespace itk
{
/** \class GiftiMeshIOFactory
   * \brief Create instances of GiftiMeshIO objects using an object factory.
   */
class ITK_EXPORT GiftiMeshIOFactory:public ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef GiftiMeshIOFactory         Self;
  typedef ObjectFactoryBase          Superclass;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char * GetITKSourceVersion(void) const;

  virtual const char * GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GiftiMeshIOFactory, ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    GiftiMeshIOFactory::Pointer giftiFactory = GiftiMeshIOFactory::New();

    ObjectFactoryBase::RegisterFactory(giftiFactory);
  }

protected:
  GiftiMeshIOFactory();
  ~GiftiMeshIOFactory();

  virtual void PrintSelf(std::ostream & os, Indent indent) const;

private:
  GiftiMeshIOFactory(const Self &); // purposely not implemented
  void operator=(const Self &);     // purposely not implemented
};
// /////////////////////////////////////////////////////////////////////
} // end namespace itk

#endif
