/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkConvertVariableLengthVectorPixelBuffer.txx,v $
  Language:  C++
  Date:      $Date: 2010-08-18 22:59:57 $
  Version:   $Revision: 0.00 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkConvertVariableLengthVectorPixelBuffer_txx
#define __itkConvertVariableLengthVectorPixelBuffer_txx

#include "itkConvertVariableLengthVectorPixelBuffer.h"

namespace itk
{
template< typename InputPixelType, typename T, typename OutputConvertTraits >
void
ConvertPixelBuffer< InputPixelType, VariableLengthVector< T >, OutputConvertTraits >
::Convert(InputPixelType *inputData, int inputNumberOfComponents, VariableLengthVector< T > *outputData, size_t size)
{
  InputPixelType *endInput = inputData + size * static_cast< size_t >( inputNumberOfComponents );

  while ( inputData != endInput )
    {
    ( *outputData ).SetSize(inputNumberOfComponents);
    for ( int ii = 0; ii < inputNumberOfComponents; ++ii )
      {
      OutputConvertTraits::SetNthComponent( ii, *outputData, static_cast< T >( *inputData++ ) );
      }

    outputData++;
    }
}
} // end namespace itk

#endif
