/*
*
*  Copyright 2011, 2012 by the CALATK development team
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
*
*/

/**
 * To apply a map to an image to get a transformed image
 */

#include <iostream>
#include "LDDMMUtils.h"
#include "VectorImageUtils.h"
#include "VectorFieldUtils.h"

// ITK INLCUDES
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIteratorWithIndex.h>
#include <itkResampleImageFilter.h>

#include "ReSampleCLP.h"

typedef double TFLOAT;
const unsigned int VImageDimension = 2;

//
// Resampe image
//
template < typename ImageType >
void ReSample(typename ImageType::Pointer I,
              typename ImageType::Pointer out,
                float factor)
{
  // Process T0 and T1
  typedef itk::ResampleImageFilter<ImageType,ImageType> ResampleFilterType;

  typename ResampleFilterType::Pointer resampler = ResampleFilterType::New();

  resampler->SetOutputParametersFromImage(I);

  typename ImageType::RegionType::SizeType size = I->GetLargestPossibleRegion().GetSize();
  size[0] /= factor;
  size[1] /= factor;
  size[2] /= factor;

  typename ImageType::SpacingType spacing = I->GetSpacing();
  spacing[0] *= factor;
  spacing[1] *= factor;
  spacing[2] *= factor;

  resampler->SetOutputSpacing(spacing);
  resampler->SetSize(size);

  resampler->SetInput(I);
  resampler->Update();

  out->SetRegions(resampler->GetOutput()->GetLargestPossibleRegion());
  out->SetSpacing(resampler->GetOutput()->GetSpacing());
  out->SetOrigin(resampler->GetOutput()->GetOrigin());

  out->Allocate();

  itk::ImageRegionConstIterator<ImageType> inputIterator(resampler->GetOutput(), resampler->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionIterator<ImageType> outputIterator(out, out->GetLargestPossibleRegion());

  while(!inputIterator.IsAtEnd())
    {
    outputIterator.Set(inputIterator.Get());
    ++inputIterator;
    ++outputIterator;
    }

}

template < unsigned int VImageDimension >
int DoIt( int argc, char **argv )
{
  PARSE_ARGS;

  typedef itk::Image< float, VImageDimension > ImageType;
  typedef itk::ImageFileReader< ImageType >    ReaderType;
  typedef itk::ImageFileWriter< ImageType >    WriterType;

  for(size_t i = 0, end = inputFilename.size(); i != end; ++i)
  {
    typename ReaderType::Pointer reader = ReaderType::New();
    typename WriterType::Pointer writer = WriterType::New();

    reader->SetFileName( inputFilename[i]  );
    writer->SetFileName( outputFilename[i] );
    writer->UseCompressionOn();

    reader->Update();

    typename ImageType::Pointer input = reader->GetOutput();
    typename ImageType::Pointer output = ImageType::New();
    ReSample<ImageType>(input,output,factor);

    writer->SetInput( output );
    try
    {
        writer->Update();
    }
    catch( itk::ExceptionObject & err )
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int main( int argc, char **argv)
{
  PARSE_ARGS;

  unsigned int uiSourceImageDimension;

  if(inputFilename.size() != outputFilename.size())
  {
    std::cerr << "Input files do not match output names." << std::endl;
    return EXIT_FAILURE;
  }

  if(inputFilename.size() > 1)
  {
    for(size_t i = 0, end = inputFilename.size(); i != end; ++i )
    {
      uiSourceImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( inputFilename[i] );
      std::cout << "Image " << i << " size = " << uiSourceImageDimension << std::endl;
    }
  }
  else
  {
    uiSourceImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( inputFilename[0] );
    std::cout << "Image size = " << uiSourceImageDimension << std::endl;
  }
  switch ( uiSourceImageDimension )
    {
    case 2:
      return DoIt<2>( argc, argv );
      break;
    case 3:
      return DoIt<3>( argc, argv );
      break;
    default:
      std::cerr << "Unsupported image dimension = " << uiSourceImageDimension << std::endl;
    }

  return EXIT_FAILURE;
}
