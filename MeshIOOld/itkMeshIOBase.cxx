/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMeshIOBase.cxx,v $
  Language:  C++
  Date:      $Date: 2010-11-11 23:28:40 $
  Version:   $Revision: 0.08 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined( _MSC_VER )
#pragma warning ( disable : 4786 )
#endif

#include "itkMeshIOBase.h"

namespace itk
{
MeshIOBase::MeshIOBase():
  m_ByteOrder(OrderNotApplicable),
  m_FileType(ASCII),
  m_UseCompression(false),
  m_PointComponentType(UNKNOWNCOMPONENTTYPE),
  m_CellComponentType(UNKNOWNCOMPONENTTYPE),
  m_PointPixelComponentType(UNKNOWNCOMPONENTTYPE),
  m_CellPixelComponentType(UNKNOWNCOMPONENTTYPE),
  m_PointPixelType(SCALAR),
  m_CellPixelType(SCALAR),
  m_NumberOfPointPixelComponents(0),
  m_NumberOfCellPixelComponents(0),
  m_PointDimension(0),
  m_NumberOfPoints(itk::NumericTraits< SizeValueType >::Zero),
  m_NumberOfCells(itk::NumericTraits< SizeValueType >::Zero),
  m_NumberOfPointPixels(itk::NumericTraits< SizeValueType >::Zero),
  m_NumberOfCellPixels(itk::NumericTraits< SizeValueType >::Zero),
  m_CellBufferSize(itk::NumericTraits< SizeValueType >::Zero),
  m_UpdatePoints(false),
  m_UpdateCells(false),
  m_UpdatePointData(false),
  m_UpdateCellData(false)
{}

const MeshIOBase::ArrayOfExtensionsType & MeshIOBase::GetSupportedReadExtensions() const
{
  return this->m_SupportedReadExtensions;
}

const MeshIOBase::ArrayOfExtensionsType & MeshIOBase::GetSupportedWriteExtensions() const
{
  return this->m_SupportedWriteExtensions;
}

void MeshIOBase::AddSupportedReadExtension(const char *extension)
{
  this->m_SupportedReadExtensions.push_back(extension);
}

void MeshIOBase::AddSupportedWriteExtension(const char *extension)
{
  this->m_SupportedWriteExtensions.push_back(extension);
}

const std::type_info & MeshIOBase::GetComponentTypeInfo(IOComponentType componentType) const
{
  switch ( componentType )
    {
    case UCHAR:
      return typeid( unsigned char );
    case CHAR:
      return typeid( char );
    case USHORT:
      return typeid( unsigned short );
    case SHORT:
      return typeid( short );
    case UINT:
      return typeid( unsigned int );
    case INT:
      return typeid( int );
    case ULONG:
      return typeid( unsigned long );
    case LONG:
      return typeid( long );
    case LONGLONG:
      return typeid( long long );
    case ULONGLONG:
      return typeid( unsigned long long );
    case FLOAT:
      return typeid( float );
    case DOUBLE:
      return typeid( double );
    case LDOUBLE:
      return typeid( long double );
    case UNKNOWNCOMPONENTTYPE:
    default:
      itkExceptionMacro ("Unknown component type: " << componentType);
    }

  return typeid( MeshIOBase::UnknownType );
}

unsigned int MeshIOBase::GetComponentSize(IOComponentType componentType) const
{
  switch ( componentType )
    {
    case UCHAR:
      return sizeof( unsigned char );
    case CHAR:
      return sizeof( char );
    case USHORT:
      return sizeof( unsigned short );
    case SHORT:
      return sizeof( short );
    case UINT:
      return sizeof( unsigned int );
    case INT:
      return sizeof( int );
    case ULONG:
      return sizeof( unsigned long );
    case LONG:
      return sizeof( long );
    case LONGLONG:
      return sizeof( long long );
    case ULONGLONG:
      return sizeof( unsigned long long );
    case FLOAT:
      return sizeof( float );
    case DOUBLE:
      return sizeof( double );
    case LDOUBLE:
      return sizeof( long double );
    case UNKNOWNCOMPONENTTYPE:
    default:
      itkExceptionMacro ("Unknown component type: " << componentType);
    }

  return 0;
}

std::string MeshIOBase::GetFileTypeAsString(FileType t) const
{
  std::string s;

  switch ( t )
    {
    case ASCII:
      return s = "ASCII";
    case BINARY:
      return s = "BINARY";
    case TYPENOTAPPLICABLE:
    default:
      return s = "TYPENOTAPPLICABLE";
    }

  return s = "TYPENOTAPPLICABLE";
}

std::string MeshIOBase::GetByteOrderAsString(ByteOrder t) const
{
  std::string s;

  switch ( t )
    {
    case BigEndian:
      return s = "BigEndian";
    case LittleEndian:
      return s = "LittleEndian";
    case OrderNotApplicable:
    default:
      return s = "OrderNotApplicable";
    }

  return s = "OrderNotApplicable";
}

std::string MeshIOBase::GetComponentTypeAsString(IOComponentType t) const
{
  std::string s;

  switch ( t )
    {
    case UCHAR:
      return ( s = "unsigned_char" );
    case CHAR:
      return ( s = "char" );
    case USHORT:
      return ( s = "unsigned_short" );
    case SHORT:
      return ( s = "short" );
    case UINT:
      return ( s = "unsigned_int" );
    case INT:
      return ( s = "int" );
    case ULONG:
      return ( s = "unsigned_long" );
    case LONG:
      return ( s = "long" );
    case LONGLONG:
      return ( s = "long_long" );
    case ULONGLONG:
      return ( s = "unsigned_long_long" );
    case FLOAT:
      return ( s = "float" );
    case DOUBLE:
      return ( s = "double" );
    case LDOUBLE:
      return ( s = "long_doulbe" );
    case UNKNOWNCOMPONENTTYPE:
    default:
      return ( s = "unknown" );
    }

  return ( s = "unknown" );
}

std::string MeshIOBase::GetPixelTypeAsString(IOPixelType t) const
{
  std::string s;

  switch ( t )
    {
    case SCALAR:
      return ( s = "scalar" );
    case VECTOR:
      return ( s = "vector" );
    case COVARIANTVECTOR:
      return ( s = "covariant_vector" );
    case POINT:
      return ( s = "point" );
    case OFFSET:
      return ( s = "offset" );
    case RGB:
      return ( s = "rgb" );
    case RGBA:
      return ( s = "rgba" );
    case SYMMETRICSECONDRANKTENSOR:
      return ( s = "symmetric_second_rank_tensor" );
    case DIFFUSIONTENSOR3D:
      return ( s = "diffusion_tensor_3D" );
    case COMPLEX:
      return ( s = "complex" );
    case UNKNOWNPIXELTYPE:
    default:
      itkExceptionMacro ("Unknown pixel type: " << t);
    }

  return ( s = "unknown" );
}

void MeshIOBase::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FileName: " << this->m_FileName << std::endl;
  os << indent << "FileType: " << GetFileTypeAsString(m_FileType) << std::endl;
  os << indent << "ByteOrder: " << GetByteOrderAsString(m_ByteOrder) << std::endl;
  os << indent << "Point dimension: " << m_PointDimension << std::endl;
  os << indent << "Point component type: " << GetComponentTypeAsString(m_PointComponentType) << std::endl;
  os << indent << "Cell  component type: " << GetComponentTypeAsString(m_CellComponentType) << std::endl;
  os << indent << "Number of point pixel components: " << m_NumberOfPointPixelComponents << std::endl;
  os << indent << "Number of cell  pixel components: " << m_NumberOfCellPixelComponents << std::endl;
  os << indent << "Number of points: " << m_NumberOfPointPixels << std::endl;
  os << indent << "Number of cells: " << m_NumberOfCellPixels << std::endl;
  os << indent << "Number of point pixels: " << m_NumberOfPoints << std::endl;
  os << indent << "Number of cell pixels: " << m_NumberOfCells << std::endl;
  os << indent << "Point pixel type: " << GetPixelTypeAsString(m_PointPixelType) << std::endl;
  os << indent << "Cell  pixel type: " << GetPixelTypeAsString(m_CellPixelType) << std::endl;
  os << indent << "Point pixel component type: " << GetComponentTypeAsString(m_PointPixelComponentType) << std::endl;
  os << indent << "Cell  pixel component type: " << GetComponentTypeAsString(m_CellPixelComponentType) << std::endl;
}
} // namespace itk end
