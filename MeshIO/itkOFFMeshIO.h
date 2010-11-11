/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkOFFMeshIO.h,v $
  Language:  C++
  Date:      $Date: 2010-08-01 12:25:44  $
  Version:   $Revision: 0.02 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkOFFMeshIO_h
#define __itkOFFMeshIO_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkMeshIOBase.h"

#include <fstream>
#include <vector>

#include <itksys/SystemTools.hxx>
// #include <limits>
namespace itk
{
/** \class OFFMeshIO
 *
 * \ingroup IOFilters
 */

class ITK_EXPORT OFFMeshIO:public MeshIOBase
{
public:
  /** Standard class typedefs. */
  typedef OFFMeshIO                    Self;
  typedef MeshIOBase                   Superclass;
  typedef SmartPointer< const Self >   ConstPointer;
  typedef SmartPointer< Self >         Pointer;
  typedef Superclass::StreamOffsetType StreamOffsetType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OFFMeshIO, MeshIOBase);

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
  /** Read buffer as ascii stream */
  template< typename T >
  void ReadCellsBufferAsAscii(T *buffer, std::ifstream & inputFile)
    {
    unsigned int index = 0;
    unsigned int numberOfPoints = 0;
    std::string  line;

    for ( unsigned long ii = 0; ii < this->m_NumberOfCells; ii++ )
      {
      inputFile >> numberOfPoints;
      buffer[index++] = static_cast< T >( numberOfPoints );
      for ( unsigned int jj = 0; jj < numberOfPoints; jj++ )
        {
        inputFile >> buffer[index++];
        }
      std::getline(inputFile, line, '\n');
      }
    }

  /** Read cells from a data buffer, used when writting cells. This function
    write all kind of cells as it is stored in cells container. It is used when
    cells container have only one kind of cells */
  template< typename TInput, typename TOutput >
  void ReadCellsBuffer(TInput *input, TOutput *output)
    {
    if ( input && output )
      {
      unsigned int indInput = 0;
      unsigned int indOutput = 0;
      for ( size_t ii = 0; ii < this->m_NumberOfCells; ii++ )
        {
        indInput++; // ignore the cell type
        unsigned int numberOfPoints = static_cast< unsigned int >( input[indInput++] );
        output[indOutput++] = static_cast< TOutput >( numberOfPoints );
        for ( unsigned int jj = 0; jj < numberOfPoints; jj++ )
          {
          output[indOutput++] = static_cast< TOutput >( input[indInput++] );
          }
        }
      }
    }

  template< typename T >
  void WriteCellsAsAscii(T *buffer, std::ofstream & outputFile)
    {
    unsigned long index = 0;

    for ( unsigned long ii = 0; ii < this->m_NumberOfCells; ii++ )
      {
      index++;
      unsigned int numberOfCellPoints = static_cast< unsigned int >( buffer[index++] );
      outputFile << numberOfCellPoints << "  ";

      for ( unsigned int jj = 0; jj < numberOfCellPoints; jj++ )
        {
        outputFile << buffer[index++] << "  ";
        }

      outputFile << '\n';
      }
    }

  template< typename TOutput, typename TInput >
  void WriteCellsAsBinary(TInput *buffer, std::ofstream & outputFile)
    {
    TOutput *data = new TOutput[m_CellBufferSize - this->m_NumberOfCells];

    ReadCellsBuffer(buffer, data);
    WriteBufferAsBinary< TOutput >(data, outputFile, m_CellBufferSize - this->m_NumberOfCells);

    delete[] data;
    }

protected:
  OFFMeshIO();
  virtual ~OFFMeshIO(){}

  void PrintSelf(std::ostream & os, Indent indent) const;

  void OpenFile();

  void CloseFile();

private:
  OFFMeshIO(const Self &);      // purposely not implemented
  void operator=(const Self &); // purposely not implemented

  std::ifstream    m_InputFile;
  StreamOffsetType m_PointsStartPosition; // file position for points rlative to std::ios::beg
  bool             m_TriangleCellType;    // if all cells are trinalge it is true. otherwise, it is false.
};
} // end namespace itk

#endif
