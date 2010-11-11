/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMeshIOFactory.h,v $
  Language:  C++
  Date:      $Date: 2010-08-01 14:28:51 $
  Version:   $Revision: 0.01 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMeshIOFactory_h
#define __itkMeshIOFactory_h

#include "itkObject.h"
#include "itkMeshIOBase.h"

namespace itk
{
/** \class MeshIOFactory
 * \brief Create instances of MeshIO objects using an object factory.
 */
class ITK_EXPORT MeshIOFactory:public Object
{
public:
  /** Standard class typedefs. */
  typedef MeshIOFactory              Self;
  typedef Object                     Superclass;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Class Methods used to interface with the registered factories */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MeshIOFactory, Object);

  /** Convenient typedefs. */
  typedef::itk::MeshIOBase::Pointer MeshIOBasePointer;

  /** Mode in which the files is intended to be used */
  typedef enum { ReadMode, WriteMode } FileModeType;

  /** Create the appropriate MeshIO depending on the particulars of the file. */
  static MeshIOBasePointer CreateMeshIO(const char *path, FileModeType mode);

  /** Register Built-in factories */
  static void RegisterBuiltInFactories();

protected:
  MeshIOFactory();
  ~MeshIOFactory();
private:
  MeshIOFactory(const Self &);  // purposely not implemented
  void operator=(const Self &); // purposely not implemented
};
} // end namespace itk

#endif
