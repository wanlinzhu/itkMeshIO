/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBYUMeshIO.h,v                                   $
  Language:  C++
  Date:      $Date: 2010-08-01 22:34:44                                   $
  Version:   $Revision: 0.02                                              $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkBYUMeshIO_h
#define __itkBYUMeshIO_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkMeshIOBase.h"

#include <fstream>
#include <vector>
#include <itksys/SystemTools.hxx>

namespace itk
{
/** \class BYUMeshIO
   *
   * \ingroup IOFilters
 */

class ITK_EXPORT BYUMeshIO:public MeshIOBase
{
public:
  /** Standard class typedefs. */
  typedef BYUMeshIO                  Self;
  typedef MeshIOBase                 Superclass;
  typedef SmartPointer< const Self > ConstPointer;
  typedef SmartPointer< Self >       Pointer;

  typedef Superclass::StreamOffsetType StreamOffsetType;
  typedef Superclass::SizeValueType    SizeValueType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BYUMeshIO, MeshIOBase);

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

  /** Writes the data to disk from the memory buffer provided. */
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
    Indent indent(1);
    SizeValueType index = itk::NumericTraits< SizeValueType >::Zero;

    for( SizeValueType ii = 0; ii < this->m_NumberOfPoints; ii++ )
      {
      outputFile << indent;
      for( unsigned int jj = 0; jj < this->m_PointDimension; jj++ )
        {
        outputFile << std::scientific << buffer[index++] << " ";
        }
      outputFile << '\n';
      }
    }

  template< typename T >
  void WriteCells(T *buffer, std::ofstream & outputFile)
    {
    Indent        indent(7);
    SizeValueType index = itk::NumericTraits< SizeValueType >::Zero;

    for( unsigned long ii = 0; ii < this->m_NumberOfCells; ii++ )
      {
      unsigned int numberOfCellPoints = static_cast< unsigned int >( buffer[++index] );
      index++;
      for ( unsigned int jj = 0; jj < numberOfCellPoints - 1; jj++ )
        {
        outputFile << indent << buffer[index++] + 1;
        }

      outputFile << indent << -( buffer[index++] + 1 ) << '\n';
      }
    }

protected:
  BYUMeshIO();
  virtual ~BYUMeshIO(){}

  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  BYUMeshIO(const Self &);      // purposely not implemented
  void operator=(const Self &); // purposely not implemented

  StreamOffsetType m_FilePosition;
  unsigned int     m_PartId;
  unsigned int     m_FirstCellId;
  unsigned int     m_LastCellId;
};
} // end namespace itk

#endif
