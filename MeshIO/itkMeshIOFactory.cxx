/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMeshIOFactory.cxx,v $
  Language:  C++
  Date:      $Date: 2010-08-01 02:41:03 $
  Version:   $Revision: 0.05 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkBYUMeshIOFactory.h"
#include "itkFreeSurferAsciiMeshIOFactory.h"
#include "itkFreeSurferBinaryMeshIOFactory.h"
#include "itkGiftiMeshIOFactory.h"
#include "itkMeshIOFactory.h"
#include "itkOBJMeshIOFactory.h"
#include "itkOFFMeshIOFactory.h"
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"
#include "itkVTKPolyDataMeshIOFactory.h"

namespace itk
{
MeshIOBase::Pointer MeshIOFactory::CreateMeshIO(const char *path, FileModeType mode)
{
  RegisterBuiltInFactories();

  std::list< MeshIOBase::Pointer >  possibleMeshIO;
  std::list< LightObject::Pointer > allobjects = ObjectFactoryBase::CreateAllInstance("itkMeshIOBase");

  for ( std::list< LightObject::Pointer >::iterator it = allobjects.begin(); it != allobjects.end(); ++it )
    {
    MeshIOBase *io = dynamic_cast< MeshIOBase * >( it->GetPointer() );

    if ( io )
      {
      possibleMeshIO.push_back(io);
      }
    else
      {
      std::cerr << "Error MeshIO factory did not return an MeshIOBase: "
                << ( *it )->GetNameOfClass()
                << std::endl;
      }
    }

  for ( std::list< MeshIOBase::Pointer >::iterator k = possibleMeshIO.begin(); k != possibleMeshIO.end(); ++k )
    {
    if ( mode == ReadMode )
      {
      if ( ( *k )->CanReadFile(path) )
        {
        return *k;
        }
      }
    else if ( mode == WriteMode )
      {
      if ( ( *k )->CanWriteFile(path) )
        {
        return *k;
        }
      }
    }

  return 0;
}


void MeshIOFactory::RegisterBuiltInFactories()
{
  static bool firstTime = true;

  static SimpleMutexLock mutex;

    {
    // This helper class makes sure the Mutex is unlocked
    // in the event an exception is thrown.
    MutexLockHolder< SimpleMutexLock > mutexHolder(mutex);
    if ( firstTime )
      {
      ObjectFactoryBase::RegisterFactory( BYUMeshIOFactory::New() );
      ObjectFactoryBase::RegisterFactory( FreeSurferAsciiMeshIOFactory::New() );
      ObjectFactoryBase::RegisterFactory( FreeSurferBinaryMeshIOFactory::New() );
      ObjectFactoryBase::RegisterFactory( GiftiMeshIOFactory::New() );
      ObjectFactoryBase::RegisterFactory( OBJMeshIOFactory::New() );
      ObjectFactoryBase::RegisterFactory( OFFMeshIOFactory::New() );
      ObjectFactoryBase::RegisterFactory( VTKPolyDataMeshIOFactory::New() );

      firstTime = false;
      }
    }
}


} // end namespace itk
