/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGiftiMeshIO.h,v $
  Language:  C++
  Date:      $Date: 2010-10-22 21:41:44 $
  Version:   $Revision: 0.04 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGiftiMeshIO_h
#define __itkGiftiMeshIO_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkMatrix.h"
#include "itkMeshIOBase.h"
#include "gifti_io.h"

#include <fstream>

namespace itk
{
/** \class GiftiMeshIO
 *
 * \ingroup IOFilters
 */

class ITK_EXPORT GiftiMeshIO:public MeshIOBase
{
public:
  /** Standard class typedefs. */
  typedef GiftiMeshIO                Self;
  typedef MeshIOBase                 Superclass;
  typedef SmartPointer< const Self > ConstPointer;
  typedef SmartPointer< Self >       Pointer;
  typedef Matrix< double, 4, 4 >     DirectionType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GiftiMeshIO, MeshIOBase);

  itkGetConstMacro(ReadPointData, bool);
  itkSetMacro(ReadPointData, bool);
  itkBooleanMacro(ReadPointData);

  void SetDirection(const DirectionType direction);

  itkGetConstReferenceMacro(Direction, DirectionType);

  /*-------- This part of the interfaces deals with reading data. ----- */

  /** Determine if the file can be read with this MeshIO implementation.
  * \param FileNameToRead The name of the file to test for reading.
  * \post Sets classes MeshIOBase::m_FileName variable to be FileNameToWrite
  * \return Returns true if this MeshIO can read the file specified.
  */
  virtual bool CanReadFile(const char *FileNameToRead);

  /** Set the spacing and dimension information for the set filename. */
  virtual void ReadMeshInformation();

  /** Reads the data from disk into the memory buffer provided. */
  virtual void ReadPoints(void *buffer);

  virtual void ReadCells(void *buffer);

  virtual void ReadPointData(void *buffer);

  virtual void ReadCellData(void *buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine if the file can be written with this MeshIO implementation.
   * \param FileNameToWrite The name of the file to test for writing.
   * \post Sets classes MeshIOBase::m_FileName variable to be FileNameToWrite
   * \return Returns true if this MeshIO can write the file specified.
   */
  virtual bool CanWriteFile(const char *FileNameToWrite);

  /** Set the spacing and dimension information for the set filename. */
  virtual void WriteMeshInformation();

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegions has been set properly. */
  virtual void WritePoints(void *buffer);

  virtual void WriteCells(void *buffer);

  virtual void WritePointData(void *buffer);

  virtual void WriteCellData(void *buffer);

  virtual void Write();

protected:
  GiftiMeshIO();
  virtual ~GiftiMeshIO(){gifti_free_image(m_GiftiImage); }

  void PrintSelf(std::ostream & os, Indent indent) const;

  template< class TInput, class TOutput >
  void ConvertBuffer(TInput *input, TOutput *output, SizeValueType numberOfElements)
  {
    if ( input && output )
      {
      for ( SizeValueType ii = 0; ii < numberOfElements; ii++ )
        {
        output[ii] = static_cast< TOutput >( input[ii] );
        }
      }
  }

private:
  bool          m_ReadPointData;
  gifti_image * m_GiftiImage;
  DirectionType m_Direction;

private:
  GiftiMeshIO(const Self &);    // purposely not implemented
  void operator=(const Self &); // purposely not implemented
};
} // end namespace itk

#endif
