/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMeshFileReader.txx,v $
  Language:  C++
  Date:      $Date: 2010-11-11 23:33:07 $
  Version:   $Revision: 0.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMeshFileReader_txx
#define __itkMeshFileReader_txx

#include "itkConvertPixelBuffer.h"
#include "itkConvertArrayPixelBuffer.h"
#include "itkConvertVariableLengthVectorPixelBuffer.h"
#include "itkMeshIOFactory.h"
#include "itkMeshFileReader.h"
#include "itkMeshRegion.h"
#include "itkObjectFactory.h"
#include "itkPixelTraits.h"

#include <itksys/SystemTools.hxx>
#include <fstream>

namespace itk
{
template< class TOutputMesh, class ConvertPointPixelTraits, class ConvertCellPixelTraits >
MeshFileReader< TOutputMesh, ConvertPointPixelTraits, ConvertCellPixelTraits >::MeshFileReader()
{
  m_MeshIO = 0;
  m_FileName = "";
  m_UserSpecifiedMeshIO = false;
}

template< class TOutputMesh, class ConvertPointPixelTraits, class ConvertCellPixelTraits >
void MeshFileReader< TOutputMesh, ConvertPointPixelTraits, ConvertCellPixelTraits >::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  if ( m_MeshIO )
    {
    os << indent << "MeshIO: \n";
    m_MeshIO->Print( os, indent.GetNextIndent() );
    }
  else
    {
    os << indent << "MeshIO: (null)" << "\n";
    }

  os << indent << "UserSpecifiedMeshIO flag: " << m_UserSpecifiedMeshIO << "\n";
  os << indent << "m_FileName: " << m_FileName << "\n";
}

template< class TOutputMesh, class ConvertPointPixelTraits, class ConvertCellPixelTraits >
void MeshFileReader< TOutputMesh, ConvertPointPixelTraits, ConvertCellPixelTraits >::SetMeshIO(MeshIOBase *meshIO)
{
  itkDebugMacro("setting MeshIO to " << meshIO);

  if ( this->m_MeshIO != meshIO )
    {
    this->m_MeshIO = meshIO;
    this->Modified();
    }

  m_UserSpecifiedMeshIO = true;
}

template< class TOutputMesh, class ConvertPointPixelTraits, class ConvertCellPixelTraits >
void MeshFileReader< TOutputMesh, ConvertPointPixelTraits, ConvertCellPixelTraits >::TestFileExistanceAndReadability()
{
  // Test if the file exists.
  if ( !itksys::SystemTools::FileExists( m_FileName.c_str() ) )
    {
    MeshFileReaderException e(__FILE__, __LINE__);
    std::ostringstream      msg;
    msg << "The file doesn't exist. "
        << std::endl << "Filename = " << m_FileName
        << std::endl;
    e.SetDescription( msg.str().c_str() );
    throw e;
    return;
    }

  // Test if the file can be open for reading access.
  std::ifstream readTester;
  readTester.open( m_FileName.c_str() );
  if ( readTester.fail() )
    {
    readTester.close();
    std::ostringstream msg;
    msg << "The file couldn't be opened for reading. "
        << std::endl << "Filename: " << m_FileName
        << std::endl;
    MeshFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
    throw e;
    return;
    }
  readTester.close();
}

template< class TOutputMesh, class ConvertPointPixelTraits, class ConvertCellPixelTraits >
template< class T >
void MeshFileReader< TOutputMesh, ConvertPointPixelTraits, ConvertCellPixelTraits >::ReadPoints(T *buffer)
{
  typename TOutputMesh::Pointer output = this->GetOutput();
  output->GetPoints()->Reserve( m_MeshIO->GetNumberOfPoints() );
  OutputPointType point;

  for ( OutputPointIdentifier id = 0; id < output->GetNumberOfPoints(); id++ )
    {
    for ( OutputPointIdentifier ii = 0; ii < OutputPointDimension; ii++ )
      {
      point[ii] = static_cast< typename OutputPointType::ValueType >( buffer[id * OutputPointDimension + ii] );
      }

    output->SetPoint(id, point);
    }
}

template< class TOutputMesh, class ConvertPointPixelTraits, class ConvertCellPixelTraits >
template< class T >
void MeshFileReader< TOutputMesh, ConvertPointPixelTraits, ConvertCellPixelTraits >::ReadCells(T *buffer)
{
  typename TOutputMesh::Pointer output = this->GetOutput();

  SizeValueType        index = NumericTraits< SizeValueType >::Zero;
  OutputCellIdentifier id = NumericTraits< OutputCellIdentifier >::Zero;
  while ( index < m_MeshIO->GetCellBufferSize() )
    {
    MeshIOBase::CellGeometryType type = static_cast< MeshIOBase::CellGeometryType >( static_cast< int >( buffer[index++] ) );
    switch ( type )
      {
      case MeshIOBase::VERTEX_CELL:
        {
        unsigned int numberOfPoints = static_cast< unsigned int >( buffer[index++] );
        if ( numberOfPoints != OutputVertexCellType::NumberOfPoints )
          {
          itkExceptionMacro(<< "Invalid Vertex Cell with number of points = " << numberOfPoints);
          }
        OutputCellAutoPointer cell;
        OutputVertexCellType *vertexCell = new OutputVertexCellType;
        for ( unsigned int jj = 0; jj < OutputVertexCellType::NumberOfPoints; jj++ )
          {
          vertexCell->SetPointId( jj, static_cast< OutputPointIdentifier >( buffer[index++] ) );
          }

        cell.TakeOwnership(vertexCell);
        output->SetCell(id++, cell);
        break;
        }
      case MeshIOBase::LINE_CELL:
        {
        // for polylines will be loaded as individual edges.
        unsigned int numberOfPoints = static_cast< unsigned int >( buffer[index++] );
        if ( numberOfPoints < 2 )
          {
          itkExceptionMacro(<< "Invalid Line Cell with number of points = " << numberOfPoints);
          }
        OutputPointIdentifier pointIDBuffer = static_cast< OutputPointIdentifier >( buffer[index++] );
        for ( unsigned int jj = 1; jj < numberOfPoints; ++jj )
          {
          OutputCellAutoPointer cell;
          OutputLineCellType *  lineCell = new OutputLineCellType;
          lineCell->SetPointId(0, pointIDBuffer);
          pointIDBuffer = static_cast< OutputPointIdentifier >( buffer[index++] );
          lineCell->SetPointId(1, pointIDBuffer);
          cell.TakeOwnership(lineCell);
          output->SetCell(id++, cell);
          }
        break;
        }
      case MeshIOBase::TRIANGLE_CELL:
        {
        unsigned int numberOfPoints = static_cast< unsigned int >( buffer[index++] );
        if ( numberOfPoints != OutputTriangleCellType::NumberOfPoints )
          {
          itkExceptionMacro(<< "Invalid Triangle Cell with number of points = " << numberOfPoints);
          }

        OutputCellAutoPointer   cell;
        OutputTriangleCellType *triangleCell = new OutputTriangleCellType;
        for ( unsigned int jj = 0; jj < OutputTriangleCellType::NumberOfPoints; jj++ )
          {
          triangleCell->SetPointId( jj, static_cast< OutputPointIdentifier >( buffer[index++] ) );
          }

        cell.TakeOwnership(triangleCell);
        output->SetCell(id++, cell);
        break;
        }
      case MeshIOBase::QUADRILATERAL_CELL:
        {
        unsigned int numberOfPoints = static_cast< unsigned int >( buffer[index++] );
        if ( numberOfPoints != OutputQuadrilateralCellType::NumberOfPoints )
          {
          itkExceptionMacro(<< "Invalid Quadrilateral Cell with number of points = " << numberOfPoints);
          }

        OutputCellAutoPointer        cell;
        OutputQuadrilateralCellType *quadrilateralCell = new OutputQuadrilateralCellType;
        for ( unsigned int jj = 0; jj < OutputQuadrilateralCellType::NumberOfPoints; jj++ )
          {
          quadrilateralCell->SetPointId( jj, static_cast< OutputPointIdentifier >( buffer[index++] ) );
          }

        cell.TakeOwnership(quadrilateralCell);
        output->SetCell(id++, cell);
        break;
        }
      case MeshIOBase::POLYGON_CELL:
        {
        // For polyhedron, if the number of points is 3, then we treat it as
        // triangle cell
        OutputCellAutoPointer cell;
        unsigned int          numberOfPoints = static_cast< unsigned int >( buffer[index++] );
        if ( numberOfPoints == OutputTriangleCellType::NumberOfPoints )
          {
          OutputTriangleCellType *triangleCell = new OutputTriangleCellType;
          for ( unsigned int jj = 0; jj < OutputTriangleCellType::NumberOfPoints; jj++ )
            {
            triangleCell->SetPointId( jj, static_cast< OutputPointIdentifier >( buffer[index++] ) );
            }
          cell.TakeOwnership(triangleCell);
          }
        else
          {
          OutputPolygonCellType *polygonCell = new OutputPolygonCellType;
          for ( unsigned int jj = 0; jj < numberOfPoints; jj++ )
            {
            polygonCell->SetPointId( jj, static_cast< OutputPointIdentifier >( buffer[index++] ) );
            }
          cell.TakeOwnership(polygonCell);
          }

        output->SetCell(id++, cell);
        break;
        }
      case MeshIOBase::TETRAHEDRON_CELL:
        {
        unsigned int numberOfPoints = static_cast< unsigned int >( buffer[index++] );
        if ( numberOfPoints != OutputTetrahedronCellType::NumberOfPoints )
          {
          itkExceptionMacro(<< "Invalid Tetrahedron Cell with number of points = " << numberOfPoints);
          }

        OutputCellAutoPointer      cell;
        OutputTetrahedronCellType *tetrahedronCell = new OutputTetrahedronCellType;
        for ( unsigned int jj = 0; jj < OutputTetrahedronCellType::NumberOfPoints; jj++ )
          {
          tetrahedronCell->SetPointId( jj, static_cast< OutputPointIdentifier >( buffer[index++] ) );
          }

        cell.TakeOwnership(tetrahedronCell);
        output->SetCell(id++, cell);
        break;
        }
      case MeshIOBase::HEXAHEDRON_CELL:
        {
        unsigned int numberOfPoints = static_cast< unsigned int >( buffer[index++] );
        if ( numberOfPoints != OutputHexahedronCellType::NumberOfPoints )
          {
          itkExceptionMacro(<< "Invalid Hexahedron Cell with number of points = " << numberOfPoints);
          }

        OutputCellAutoPointer     cell;
        OutputHexahedronCellType *hexahedronCell = new OutputHexahedronCellType;
        for ( unsigned int jj = 0; jj < OutputHexahedronCellType::NumberOfPoints; jj++ )
          {
          hexahedronCell->SetPointId( jj, static_cast< OutputPointIdentifier >( buffer[index++] ) );
          }

        cell.TakeOwnership(hexahedronCell);
        output->SetCell(id++, cell);
        break;
        }
      case MeshIOBase::QUADRATIC_EDGE_CELL:
        {
        unsigned int numberOfPoints = static_cast< unsigned int >( buffer[index++] );
        if ( numberOfPoints != OutputQuadraticEdgeCellType::NumberOfPoints )
          {
          itkExceptionMacro(<< "Invalid Quadratic edge Cell with number of points = " << numberOfPoints);
          }

        OutputCellAutoPointer        cell;
        OutputQuadraticEdgeCellType *quadraticEdgeCell = new OutputQuadraticEdgeCellType;
        for ( unsigned int jj = 0; jj < OutputQuadraticEdgeCellType::NumberOfPoints; jj++ )
          {
          quadraticEdgeCell->SetPointId( jj, static_cast< OutputPointIdentifier >( buffer[index++] ) );
          }

        cell.TakeOwnership(quadraticEdgeCell);
        output->SetCell(id++, cell);
        break;
        }
      case MeshIOBase::QUADRATIC_TRIANGLE_CELL:
        {
        unsigned int numberOfPoints = static_cast< unsigned int >( buffer[index++] );
        if ( numberOfPoints != OutputQuadraticTriangleCellType::NumberOfPoints )
          {
          itkExceptionMacro(<< "Invalid Quadratic triangle Cell with number of points = " << numberOfPoints);
          }

        OutputCellAutoPointer            cell;
        OutputQuadraticTriangleCellType *quadraticTriangleCell = new OutputQuadraticTriangleCellType;
        for ( unsigned int jj = 0; jj < OutputQuadraticTriangleCellType::NumberOfPoints; jj++ )
          {
          quadraticTriangleCell->SetPointId( jj, static_cast< OutputPointIdentifier >( buffer[index++] ) );
          }

        cell.TakeOwnership(quadraticTriangleCell);
        output->SetCell(id++, cell);
        break;
        }
      default:
        {
        itkExceptionMacro(<< "Unknown cell type");
        }
      }
    }
}

template< class TOutputMesh, class ConvertPointPixelTraits, class ConvertCellPixelTraits >
void MeshFileReader< TOutputMesh, ConvertPointPixelTraits, ConvertCellPixelTraits >::ReadPointData()
{
  typename TOutputMesh::Pointer output = this->GetOutput();

  char *                inputPointDataBuffer = 0;
  OutputPointPixelType *outputPointDataBuffer = new OutputPointPixelType[m_MeshIO->GetNumberOfPointPixels()];

  try
    {
    if ( ( m_MeshIO->GetComponentTypeInfo( m_MeshIO->GetPointPixelComponentType() ) !=
          typeid( ITK_TYPENAME ConvertPointPixelTraits::ComponentType ) )
        || ( m_MeshIO->GetNumberOfPointPixelComponents() != ConvertPointPixelTraits::GetNumberOfComponents() ) )
      {
      // the pixel types don't match so a type conversion needs to be performed
      itkDebugMacro( << "Buffer conversion required from: "
                     << m_MeshIO->GetComponentTypeInfo( m_MeshIO->GetPointPixelComponentType() ).name()
                     << " to: "
                     << typeid( ITK_TYPENAME ConvertPointPixelTraits::ComponentType ).name() );

      inputPointDataBuffer =
        new char[m_MeshIO->GetNumberOfPointPixelComponents() * m_MeshIO->GetComponentSize( m_MeshIO->GetPointPixelComponentType() )
                 * m_MeshIO->GetNumberOfPointPixels()];
      m_MeshIO->ReadPointData( static_cast< void * >( inputPointDataBuffer ) );

      this->ConvertPointPixelBuffer( static_cast< void * >( inputPointDataBuffer ), outputPointDataBuffer, m_MeshIO->GetNumberOfPointPixels() );
      }
    else
      {
      itkDebugMacro(<< "No buffer conversion required.");
      m_MeshIO->ReadPointData( static_cast< void * >( outputPointDataBuffer ) );
      }
    }
  catch ( ... )
    {
    // if an exception is thrown catch it
    if ( inputPointDataBuffer )
      {
      // clean up
      delete[] inputPointDataBuffer;
      inputPointDataBuffer = 0;
      }

    if ( outputPointDataBuffer )
      {
      // clean up
      delete[] outputPointDataBuffer;
      outputPointDataBuffer = 0;
      }

    // then rethrow
    throw;
    }

  // clean up
  if ( inputPointDataBuffer )
    {
    delete[] inputPointDataBuffer;
    inputPointDataBuffer = 0;
    }

  for ( OutputPointIdentifier id = 0; id < m_MeshIO->GetNumberOfPointPixels(); id++ )
    {
    output->SetPointData(id, outputPointDataBuffer[id]);
    }

  if ( outputPointDataBuffer )
    {
    delete[] outputPointDataBuffer;
    outputPointDataBuffer = 0;
    }
}

template< class TOutputMesh, class ConvertPointPixelTraits, class ConvertCellPixelTraits >
void MeshFileReader< TOutputMesh, ConvertPointPixelTraits, ConvertCellPixelTraits >::ReadCellData()
{
  typename TOutputMesh::Pointer output = this->GetOutput();

  char *               inputCellDataBuffer = 0;
  OutputCellPixelType *outputCellDataBuffer = new OutputCellPixelType[m_MeshIO->GetNumberOfCellPixels()];

  try
    {
    if ( ( m_MeshIO->GetComponentTypeInfo( m_MeshIO->GetCellPixelComponentType() ) !=
          typeid( ITK_TYPENAME ConvertCellPixelTraits::ComponentType ) )
        || ( m_MeshIO->GetNumberOfCellPixelComponents() != ConvertCellPixelTraits::GetNumberOfComponents() ) )
      {
      // the pixel types don't match so a type conversion needs to be performed
      itkDebugMacro( << "Buffer conversion required from: "
                     << m_MeshIO->GetComponentTypeInfo( m_MeshIO->GetCellPixelComponentType() ).name()
                     << " to: "
                     << typeid( ITK_TYPENAME ConvertCellPixelTraits::ComponentType ).name() );

      inputCellDataBuffer =
        new char[m_MeshIO->GetNumberOfCellPixelComponents() * m_MeshIO->GetComponentSize( m_MeshIO->GetCellPixelComponentType() )
                 * m_MeshIO->GetNumberOfCellPixels()];
      m_MeshIO->ReadCellData( static_cast< void * >( inputCellDataBuffer ) );

      this->ConvertCellPixelBuffer( static_cast< void * >( inputCellDataBuffer ), outputCellDataBuffer, m_MeshIO->GetNumberOfCellPixels() );
      }
    else
      {
      itkDebugMacro(<< "No buffer conversion required.");
      m_MeshIO->ReadCellData( static_cast< void * >( outputCellDataBuffer ) );
      }
    }
  catch ( ... )
    {
    // if an exception is thrown catch it

    if ( inputCellDataBuffer )
      {
      // clean up
      delete[] inputCellDataBuffer;
      inputCellDataBuffer = 0;
      }

    if ( outputCellDataBuffer )
      {
      // clean up
      delete[] outputCellDataBuffer;
      outputCellDataBuffer = 0;
      }

    // then rethrow
    throw;
    }

  // clean up
  if ( inputCellDataBuffer )
    {
    delete[] inputCellDataBuffer;
    inputCellDataBuffer = 0;
    }

  for ( OutputCellIdentifier id = 0; id < m_MeshIO->GetNumberOfCellPixels(); id++ )
    {
    output->SetCellData(id, outputCellDataBuffer[id]);
    }

  if ( outputCellDataBuffer )
    {
    delete[] outputCellDataBuffer;
    outputCellDataBuffer = 0;
    }
}

template< class TOutputMesh, class ConvertPointPixelTraits, class ConvertCellPixelTraits >
void MeshFileReader< TOutputMesh, ConvertPointPixelTraits, ConvertCellPixelTraits >::GenerateOutputInformation()
{
  if ( m_FileName == "" )
    {
    throw MeshFileReaderException(__FILE__, __LINE__, "FileName must be specified", ITK_LOCATION);
    }

  try
    {
    m_ExceptionMessage = "";
    this->TestFileExistanceAndReadability();
    }
  catch ( itk::ExceptionObject & err )
    {
    m_ExceptionMessage = err.GetDescription();
    }

  if ( m_UserSpecifiedMeshIO == false ) // try creating via factory
    {
    m_MeshIO = MeshIOFactory::CreateMeshIO(m_FileName.c_str(), MeshIOFactory::ReadMode);
    }

  if ( m_MeshIO.IsNull() )
    {
    std::ostringstream msg;
    msg << " Could not create IO object for file " << m_FileName.c_str() << std::endl;
    if ( m_ExceptionMessage.size() )
      {
      msg << m_ExceptionMessage;
      }
    else
      {
      msg << "  Tried to create one of the following:" << std::endl;
      std::list< LightObject::Pointer > allobjects = ObjectFactoryBase::CreateAllInstance("itkMeshIOBase");
      for ( std::list< LightObject::Pointer >::iterator it = allobjects.begin(); it != allobjects.end(); ++it )
        {
        MeshIOBase *io = dynamic_cast< MeshIOBase * >( it->GetPointer() );
        msg << "    " << io->GetNameOfClass() << std::endl;
        }
      msg << "  You probably failed to set a file suffix, or" << std::endl;
      msg << "  set the suffix to an unsupported type." << std::endl;
      }

    MeshFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
    throw e;
    return;
    }
}

template< class TOutputMesh, class ConvertPointPixelTraits, class ConvertCellPixelTraits >
void MeshFileReader< TOutputMesh, ConvertPointPixelTraits, ConvertCellPixelTraits >::GenerateData()
{
  typename TOutputMesh::Pointer output = this->GetOutput();

  // Test if the file exists and if it can be opened.
  // An exception will be thrown otherwise, since we can't
  // successfully read the file. We catch the exception because some
  // MeshIO's may not actually open a file. Still
  // reports file error if no MeshIO is loaded.

  output->Initialize();
  output->SetBufferedRegion( output->GetRequestedRegion() );

  // Test existance and readability of input file
  try
    {
    m_ExceptionMessage = "";
    this->TestFileExistanceAndReadability();
    }
  catch ( itk::ExceptionObject & err )
    {
    m_ExceptionMessage = err.GetDescription();
    }

  // Tell the MeshIO to read the file
  m_MeshIO->SetFileName( m_FileName.c_str() );

  // Get mesh information
  m_MeshIO->ReadMeshInformation();

  // Read points
  if ( m_MeshIO->GetUpdatePoints() )
    {
    switch ( m_MeshIO->GetPointComponentType() )
      {
      case MeshIOBase::CHAR:
        {
        char *pointsBuffer = new char[m_MeshIO->GetNumberOfPoints() * OutputPointDimension];
        m_MeshIO->ReadPoints( static_cast< void * >( pointsBuffer ) );
        ReadPoints(pointsBuffer);
        delete[] pointsBuffer;
        break;
        }
      case MeshIOBase::UCHAR:
        {
        unsigned char *pointsBuffer = new unsigned char[m_MeshIO->GetNumberOfPoints() * OutputPointDimension];
        m_MeshIO->ReadPoints( static_cast< void * >( pointsBuffer ) );
        ReadPoints(pointsBuffer);
        delete[] pointsBuffer;
        break;
        }
      case MeshIOBase::SHORT:
        {
        short *pointsBuffer = new short[m_MeshIO->GetNumberOfPoints() * OutputPointDimension];
        m_MeshIO->ReadPoints( static_cast< void * >( pointsBuffer ) );
        ReadPoints(pointsBuffer);
        delete[] pointsBuffer;
        break;
        }
      case MeshIOBase::USHORT:
        {
        unsigned short *pointsBuffer = new unsigned short[m_MeshIO->GetNumberOfPoints() * OutputPointDimension];
        m_MeshIO->ReadPoints( static_cast< void * >( pointsBuffer ) );
        ReadPoints(pointsBuffer);
        delete[] pointsBuffer;
        break;
        }
      case MeshIOBase::INT:
        {
        int *pointsBuffer = new int[m_MeshIO->GetNumberOfPoints() * OutputPointDimension];
        m_MeshIO->ReadPoints( static_cast< void * >( pointsBuffer ) );
        ReadPoints(pointsBuffer);
        delete[] pointsBuffer;
        break;
        }
      case MeshIOBase::UINT:
        {
        unsigned int *pointsBuffer = new unsigned int[m_MeshIO->GetNumberOfPoints() * OutputPointDimension];
        m_MeshIO->ReadPoints( static_cast< void * >( pointsBuffer ) );
        ReadPoints(pointsBuffer);
        delete[] pointsBuffer;
        break;
        }
      case MeshIOBase::LONG:
        {
        long *pointsBuffer = new long[m_MeshIO->GetNumberOfPoints() * OutputPointDimension];
        m_MeshIO->ReadPoints( static_cast< void * >( pointsBuffer ) );
        ReadPoints(pointsBuffer);
        delete[] pointsBuffer;
        break;
        }
      case MeshIOBase::ULONG:
        {
        unsigned long *pointsBuffer = new unsigned long[m_MeshIO->GetNumberOfPoints() * OutputPointDimension];
        m_MeshIO->ReadPoints( static_cast< void * >( pointsBuffer ) );
        ReadPoints(pointsBuffer);
        delete[] pointsBuffer;
        break;
        }
      case MeshIOBase::LONGLONG:
        {
        long long *pointsBuffer = new long long[m_MeshIO->GetNumberOfPoints() * OutputPointDimension];
        m_MeshIO->ReadPoints( static_cast< void * >( pointsBuffer ) );
        ReadPoints(pointsBuffer);
        delete[] pointsBuffer;
        break;
        }
      case MeshIOBase::ULONGLONG:
        {
        unsigned long long *pointsBuffer = new unsigned long long[m_MeshIO->GetNumberOfPoints() * OutputPointDimension];
        m_MeshIO->ReadPoints( static_cast< void * >( pointsBuffer ) );
        ReadPoints(pointsBuffer);
        delete[] pointsBuffer;
        break;
        }
      case MeshIOBase::FLOAT:
        {
        float *pointsBuffer = new float[m_MeshIO->GetNumberOfPoints() * OutputPointDimension];
        m_MeshIO->ReadPoints( static_cast< void * >( pointsBuffer ) );
        ReadPoints(pointsBuffer);
        delete[] pointsBuffer;
        break;
        }
      case MeshIOBase::DOUBLE:
        {
        double *pointsBuffer = new double[m_MeshIO->GetNumberOfPoints() * OutputPointDimension];
        m_MeshIO->ReadPoints( static_cast< void * >( pointsBuffer ) );
        ReadPoints(pointsBuffer);
        delete[] pointsBuffer;
        break;
        }
      case MeshIOBase::LDOUBLE:
        {
        long double *pointsBuffer = new long double[m_MeshIO->GetNumberOfPoints() * OutputPointDimension];
        m_MeshIO->ReadPoints( static_cast< void * >( pointsBuffer ) );
        ReadPoints(pointsBuffer);
        delete[] pointsBuffer;
        break;
        }
      case MeshIOBase::UNKNOWNCOMPONENTTYPE:
      default:
        {
        itkExceptionMacro(<< "Unknown point component type");
        }
      }
    }

  // Read cells
  if ( m_MeshIO->GetUpdateCells() )
    {
    switch ( m_MeshIO->GetCellComponentType() )
      {
      case MeshIOBase::CHAR:
        {
        char *cellsBuffer = new char[m_MeshIO->GetCellBufferSize()];
        m_MeshIO->ReadCells( static_cast< void * >( cellsBuffer ) );
        ReadCells(cellsBuffer);
        delete[] cellsBuffer;
        break;
        }
      case MeshIOBase::UCHAR:
        {
        unsigned char *cellsBuffer = new unsigned char[m_MeshIO->GetCellBufferSize()];
        m_MeshIO->ReadCells( static_cast< void * >( cellsBuffer ) );
        ReadCells(cellsBuffer);
        delete[] cellsBuffer;
        break;
        }
      case MeshIOBase::SHORT:
        {
        short *cellsBuffer = new short[m_MeshIO->GetCellBufferSize()];
        m_MeshIO->ReadCells( static_cast< void * >( cellsBuffer ) );
        ReadCells(cellsBuffer);
        delete[] cellsBuffer;
        break;
        }
      case MeshIOBase::USHORT:
        {
        unsigned short *cellsBuffer = new unsigned short[m_MeshIO->GetCellBufferSize()];
        m_MeshIO->ReadCells( static_cast< void * >( cellsBuffer ) );
        ReadCells(cellsBuffer);
        delete[] cellsBuffer;
        break;
        }
      case MeshIOBase::INT:
        {
        int *cellsBuffer = new int[m_MeshIO->GetCellBufferSize()];
        m_MeshIO->ReadCells( static_cast< void * >( cellsBuffer ) );
        ReadCells(cellsBuffer);
        delete[] cellsBuffer;
        break;
        }
      case MeshIOBase::UINT:
        {
        unsigned int *cellsBuffer = new unsigned int[m_MeshIO->GetCellBufferSize()];
        m_MeshIO->ReadCells( static_cast< void * >( cellsBuffer ) );
        ReadCells(cellsBuffer);
        delete[] cellsBuffer;
        break;
        }
      case MeshIOBase::LONG:
        {
        long *cellsBuffer = new long[m_MeshIO->GetCellBufferSize()];
        m_MeshIO->ReadCells( static_cast< void * >( cellsBuffer ) );
        ReadCells(cellsBuffer);
        delete[] cellsBuffer;
        break;
        }
      case MeshIOBase::ULONG:
        {
        unsigned long *cellsBuffer = new unsigned long[m_MeshIO->GetCellBufferSize()];
        m_MeshIO->ReadCells( static_cast< void * >( cellsBuffer ) );
        ReadCells(cellsBuffer);
        delete[] cellsBuffer;
        break;
        }
      case MeshIOBase::LONGLONG:
        {
        long long *cellsBuffer = new long long[m_MeshIO->GetCellBufferSize()];
        m_MeshIO->ReadCells( static_cast< void * >( cellsBuffer ) );
        ReadCells(cellsBuffer);
        delete[] cellsBuffer;
        break;
        }
      case MeshIOBase::ULONGLONG:
        {
        unsigned long long *cellsBuffer = new unsigned long long[m_MeshIO->GetCellBufferSize()];
        m_MeshIO->ReadCells( static_cast< void * >( cellsBuffer ) );
        ReadCells(cellsBuffer);
        delete[] cellsBuffer;
        break;
        }
      case MeshIOBase::FLOAT:
        {
        float *cellsBuffer = new float[m_MeshIO->GetCellBufferSize()];
        m_MeshIO->ReadCells( static_cast< void * >( cellsBuffer ) );
        ReadCells(cellsBuffer);
        delete[] cellsBuffer;
        break;
        }
      case MeshIOBase::DOUBLE:
        {
        double *cellsBuffer = new double[m_MeshIO->GetCellBufferSize()];
        m_MeshIO->ReadCells( static_cast< void * >( cellsBuffer ) );
        ReadCells(cellsBuffer);
        delete[] cellsBuffer;
        break;
        }
      case MeshIOBase::LDOUBLE:
        {
        long double *cellsBuffer = new long double[m_MeshIO->GetCellBufferSize()];
        m_MeshIO->ReadCells( static_cast< void * >( cellsBuffer ) );
        ReadCells(cellsBuffer);
        delete[] cellsBuffer;
        break;
        }
      case MeshIOBase::UNKNOWNCOMPONENTTYPE:
      default:
        {
        itkExceptionMacro(<< "Unknown cell component type");
        }
      }
    }

  // Read Point Data
  if ( m_MeshIO->GetUpdatePointData() )
    {
    ReadPointData();
    }

  // Read Cell Data
  if ( m_MeshIO->GetUpdateCellData() )
    {
    ReadCellData();
    }
}

template< class TOutputMesh, class ConvertPointPixelTraits, class ConvertCellPixelTraits >
template< typename T >
void MeshFileReader< TOutputMesh, ConvertPointPixelTraits, ConvertCellPixelTraits >
::ConvertPointPixelBuffer(void *inputData, T *outputData, size_t numberOfPixels)
{
  // TODO:
  // Pass down the PixelType (RGB, VECTOR, etc.) so that any vector to
  // scalar conversion be type specific. i.e. RGB to scalar would use
  // a formula to convert to luminance, VECTOR to scalar would use
  // vector magnitude.

  // Create a macro as this code is a bit lengthy and repetitive
  // if the MeshIO pixel type is typeid(type) then use the ConvertPixelBuffer
  // class to convert the data block to TOutputMesh's pixel type
  // see DefaultConvertPixelTraits and ConvertPixelBuffer

#define ITK_CONVERT_POINT_PIXEL_BUFFER_IF_BLOCK(type)                                                   \
  else if ( m_MeshIO->GetComponentTypeInfo( m_MeshIO->GetPointPixelComponentType() ) == typeid( type ) )   \
    {                                                                                               \
    ConvertPixelBuffer<                                                                             \
      type,                                                                                           \
      OutputPointPixelType,                                                                           \
      ConvertPointPixelTraits                                                                         \
      >                                                                                               \
    ::Convert(                                                                                      \
      static_cast< type * >( inputData ),                                                                  \
      m_MeshIO->GetNumberOfPointPixelComponents(),                                                    \
      outputData,                                                                                     \
      numberOfPixels);                                                                                \
    }

  if ( 0 )
        {}
  ITK_CONVERT_POINT_PIXEL_BUFFER_IF_BLOCK(unsigned char)
  ITK_CONVERT_POINT_PIXEL_BUFFER_IF_BLOCK(char)
  ITK_CONVERT_POINT_PIXEL_BUFFER_IF_BLOCK(unsigned short)
  ITK_CONVERT_POINT_PIXEL_BUFFER_IF_BLOCK(short)
  ITK_CONVERT_POINT_PIXEL_BUFFER_IF_BLOCK(unsigned int)
  ITK_CONVERT_POINT_PIXEL_BUFFER_IF_BLOCK(int)
  ITK_CONVERT_POINT_PIXEL_BUFFER_IF_BLOCK(unsigned long)
  ITK_CONVERT_POINT_PIXEL_BUFFER_IF_BLOCK(long)
  ITK_CONVERT_POINT_PIXEL_BUFFER_IF_BLOCK(unsigned long long)
  ITK_CONVERT_POINT_PIXEL_BUFFER_IF_BLOCK(long long)
  ITK_CONVERT_POINT_PIXEL_BUFFER_IF_BLOCK(float)
  ITK_CONVERT_POINT_PIXEL_BUFFER_IF_BLOCK(double)
  ITK_CONVERT_POINT_PIXEL_BUFFER_IF_BLOCK(long double)
  else
    {
    MeshFileReaderException e(__FILE__, __LINE__);
    std::ostringstream      msg;
    msg << "Couldn't convert component type: "
        << std::endl << "    "
        << m_MeshIO->GetComponentTypeAsString( m_MeshIO->GetPointPixelComponentType() )
        << std::endl << "to one of: "
        << std::endl << "    " << typeid( unsigned char ).name()
        << std::endl << "    " << typeid( char ).name()
        << std::endl << "    " << typeid( unsigned short ).name()
        << std::endl << "    " << typeid( short ).name()
        << std::endl << "    " << typeid( unsigned int ).name()
        << std::endl << "    " << typeid( int ).name()
        << std::endl << "    " << typeid( unsigned long ).name()
        << std::endl << "    " << typeid( long ).name()
        << std::endl << "    " << typeid( unsigned long long ).name()
        << std::endl << "    " << typeid( long long ).name()
        << std::endl << "    " << typeid( float ).name()
        << std::endl << "    " << typeid( double ).name()
        << std::endl << "    " << typeid( long double ).name()
        << std::endl;
    e.SetDescription( msg.str().c_str() );
    e.SetLocation(ITK_LOCATION);
    throw e;
    return;
    }
#undef ITK_CONVERT_POINT_PIXEL_BUFFER_IF_BLOCK
}

template< class TOutputMesh, class ConvertPointPixelTraits, class ConvertCellPixelTraits >
template< typename T >
void MeshFileReader< TOutputMesh, ConvertPointPixelTraits, ConvertCellPixelTraits >
::ConvertCellPixelBuffer(void *inputData, T *outputData, size_t numberOfPixels)
{
  // TODO:
  // Pass down the PixelType (RGB, VECTOR, etc.) so that any vector to
  // scalar conversion be type specific. i.e. RGB to scalar would use
  // a formula to convert to luminance, VECTOR to scalar would use
  // vector magnitude.

  // Create a macro as this code is a bit lengthy and repetitive
  // if the MeshIO pixel type is typeid(type) then use the ConvertPixelBuffer
  // class to convert the data block to TOutputMesh's pixel type
  // see DefaultConvertPixelTraits and ConvertPixelBuffer

#define ITK_CONVERT_CELL_PIXEL_BUFFER_IF_BLOCK(type)                                                   \
  else if ( m_MeshIO->GetComponentTypeInfo( m_MeshIO->GetCellPixelComponentType() ) == typeid( type ) )   \
    {                                                                                              \
    ConvertPixelBuffer<                                                                            \
      type,                                                                                          \
      OutputCellPixelType,                                                                           \
      ConvertCellPixelTraits                                                                         \
      >                                                                                              \
    ::Convert(                                                                                     \
      static_cast< type * >( inputData ),                                                                 \
      m_MeshIO->GetNumberOfCellPixelComponents(),                                                    \
      outputData,                                                                                    \
      numberOfPixels);                                                                               \
    }

  if ( 0 )
        {}
  ITK_CONVERT_CELL_PIXEL_BUFFER_IF_BLOCK(unsigned char)
  ITK_CONVERT_CELL_PIXEL_BUFFER_IF_BLOCK(char)
  ITK_CONVERT_CELL_PIXEL_BUFFER_IF_BLOCK(unsigned short)
  ITK_CONVERT_CELL_PIXEL_BUFFER_IF_BLOCK(short)
  ITK_CONVERT_CELL_PIXEL_BUFFER_IF_BLOCK(unsigned int)
  ITK_CONVERT_CELL_PIXEL_BUFFER_IF_BLOCK(int)
  ITK_CONVERT_CELL_PIXEL_BUFFER_IF_BLOCK(unsigned long)
  ITK_CONVERT_CELL_PIXEL_BUFFER_IF_BLOCK(long)
  ITK_CONVERT_CELL_PIXEL_BUFFER_IF_BLOCK(unsigned long long)
  ITK_CONVERT_CELL_PIXEL_BUFFER_IF_BLOCK(long long)
  ITK_CONVERT_CELL_PIXEL_BUFFER_IF_BLOCK(float)
  ITK_CONVERT_CELL_PIXEL_BUFFER_IF_BLOCK(double)
  ITK_CONVERT_CELL_PIXEL_BUFFER_IF_BLOCK(long double)
  else
    {
    MeshFileReaderException e(__FILE__, __LINE__);
    std::ostringstream      msg;
    msg << "Couldn't convert component type: "
        << std::endl << "    "
        << m_MeshIO->GetComponentTypeAsString( m_MeshIO->GetCellPixelComponentType() )
        << std::endl << "to one of: "
        << std::endl << "    " << typeid( unsigned char ).name()
        << std::endl << "    " << typeid( char ).name()
        << std::endl << "    " << typeid( unsigned short ).name()
        << std::endl << "    " << typeid( short ).name()
        << std::endl << "    " << typeid( unsigned int ).name()
        << std::endl << "    " << typeid( int ).name()
        << std::endl << "    " << typeid( unsigned long ).name()
        << std::endl << "    " << typeid( long ).name()
        << std::endl << "    " << typeid( unsigned long long ).name()
        << std::endl << "    " << typeid( long long ).name()
        << std::endl << "    " << typeid( float ).name()
        << std::endl << "    " << typeid( double ).name()
        << std::endl << "    " << typeid( long double ).name()
        << std::endl;
    e.SetDescription( msg.str().c_str() );
    e.SetLocation(ITK_LOCATION);
    throw e;
    return;
    }
#undef ITK_CONVERT_CELL_PIXEL_BUFFER_IF_BLOCK
}
} // namespace itk end
#endif
