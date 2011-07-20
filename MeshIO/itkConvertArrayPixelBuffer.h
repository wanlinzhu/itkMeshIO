/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkConvertArrayPixelBuffer_h
#define __itkConvertArrayPixelBuffer_h

#include "itkConvertPixelBuffer.h"
#include "itkArray.h"

namespace itk
{
/**
 * \class ConvertPixelBuffer
 *  \brief Class to convert blocks of data from one type to another.
 *
 * ConvertPixelBuffer has a static method Convert().  It is used by
 * itkImageFileReader to convert from an unknown type at run-time to the
 * compile-time template pixel type in itkImageFileReader.  To work with
 * complex pixel types like RGB and RGBA a traits class is used.
 * OutputConvertTraits() is the traits class.  The default one used is
 * DefaultConvertPixelTraits.  
 *
 */
                                        
template <typename InputPixelType, typename T, typename OutputConvertTraits>
class ConvertPixelBuffer<InputPixelType, Array<T>, OutputConvertTraits> 
{
public:
  /** Determine the output data type. */
  typedef typename OutputConvertTraits::ComponentType OutputComponentType;

  /** General method converts from one type to another. */
  static void Convert(InputPixelType* inputData, 
                      int inputNumberOfComponents, 
                      Array<T>* outputData , 
                      size_t size);

private:
  ConvertPixelBuffer();
  ~ConvertPixelBuffer();
};
} //namespace ITK

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkConvertArrayPixelBuffer.hxx"
#endif

#endif // __itkConvertArrayPixelBuffer_h
