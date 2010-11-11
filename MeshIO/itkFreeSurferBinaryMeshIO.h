/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFreeSurferBinaryMeshIO.h,v $
  Language:  C++
  Date:      $Date: 2010-11-11 12:25:44 $
  Version:   $Revision: 0.04 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkFreeSurferBinaryMeshIO_h
#define __itkFreeSurferBinaryMeshIO_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkByteSwapper.h"
#include "itkMeshIOBase.h"
#include "itkIntTypes.h"

#include <fstream>
#include <vector>
#include <itksys/SystemTools.hxx>

namespace itk
{
/** \class FreeSurferBinaryMeshIO
  * Freesurfer binary surface data format, the suffix is set as *.fsb
 * \ingroup IOFilters
 */

class ITK_EXPORT FreeSurferBinaryMeshIO:public MeshIOBase
{
public:
  /** Standard class typedefs. */
  typedef FreeSurferBinaryMeshIO       Self;
  typedef MeshIOBase                   Superclass;
  typedef SmartPointer< const Self >   ConstPointer;
  typedef SmartPointer< Self >         Pointer;
  typedef Superclass::StreamOffsetType StreamOffsetType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FreeSurferBinaryMeshIO, MeshIOBase);

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
  /** Write points to output stream */
  template< typename T >
  void WritePoints(T *buffer, std::ofstream & outputFile)
  {
    float *data = new float[this->m_NumberOfPoints * this->m_PointDimension];

    for ( unsigned long ii = 0; ii < this->m_NumberOfPoints; ii++ )
      {
      for ( unsigned int jj = 0; jj < this->m_PointDimension; jj++ )
        {
        data[ii * this->m_PointDimension + jj] = static_cast< float >( buffer[ii * this->m_PointDimension + jj] );
        }
      }

    itk::ByteSwapper< float >::SwapWriteRangeFromSystemToBigEndian(data, this->m_NumberOfPoints * this->m_PointDimension, &outputFile);
    delete[] data;
  }

  /** Write cells to utput stream */
  template< typename T >
  void WriteCells(T *buffer, std::ofstream & outputFile)
  {
    const itk::uint32_t numberOfCellPoints = 3;

    itk::uint32_t *data = new itk::uint32_t[this->m_NumberOfCells * numberOfCellPoints];

    ReadCellsBuffer(buffer, data);
    itk::ByteSwapper< itk::uint32_t >::SwapWriteRangeFromSystemToBigEndian(data, this->m_NumberOfCells * numberOfCellPoints, &outputFile);

    delete[] data;
  }

  /** Read cells from a data buffer, used when writting mesh */
  template< typename TInput, typename TOutput >
  void ReadCellsBuffer(TInput *input, TOutput *output)
  {
    if ( input && output )
      {
      for ( size_t ii = 0; ii < this->m_NumberOfCells; ii++ )
        {
        for ( unsigned int jj = 0; jj < 3; jj++ )
          {
          /** point identifiers start from the third elements, first element is
            cellType, the second is numberOfPoints. */
          output[ii * 3 + jj] = static_cast< TOutput >( input[5 * ii + jj + 2] );
          }
        }
      }
  }

  /** Write points to output stream */
  template< typename T >
  void WritePointData(T *buffer, std::ofstream & outputFile)
  {
    float *data = new float[this->m_NumberOfPointPixels];

    for ( unsigned long ii = 0; ii < this->m_NumberOfPointPixels; ii++ )
      {
      data[ii] = static_cast< float >( buffer[ii] );
      }

    itk::ByteSwapper< float >::SwapWriteRangeFromSystemToBigEndian(data, this->m_NumberOfPointPixels, &outputFile);
    delete[] data;
  }

protected:
  FreeSurferBinaryMeshIO();
  virtual ~FreeSurferBinaryMeshIO(){}

  void PrintSelf(std::ostream & os, Indent indent) const;

  void OpenFile();

  void CloseFile();

private:
  FreeSurferBinaryMeshIO(const Self &); // purposely not implemented
  void operator=(const Self &);         // purposely not implemented

  StreamOffsetType m_FilePosition;
  itk::uint32_t    m_FileTypeIdentifier;
  std::ifstream    m_InputFile;
};
} // end namespace itk

#endif
