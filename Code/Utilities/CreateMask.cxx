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
#include <itkBinaryThresholdImageFilter.h>
#include <itkAddImageFilter.h>
#include <itkImageRegionIterator.h>
#include <itkImageRegionConstIteratorWithIndex.h>
#include <itkRescaleIntensityImageFilter.h>

#include "CreateMaskCLP.h"

typedef double TFLOAT;
const unsigned int VImageDimension = 2;

//
// CreateMaskFromTumorSim
//
template < typename ImageType >
void CreateMask(typename ImageType::Pointer I1,
                typename ImageType::Pointer I2,
                typename ImageType::Pointer out,
                float lowThreshold,
                bool addImages)
{
  // Process T0 and T1
  typedef itk::AddImageFilter<ImageType,ImageType,ImageType>   AddFilterType;
  typedef itk::BinaryThresholdImageFilter<ImageType,ImageType> ThresholdFilterType;
  typedef itk::RescaleIntensityImageFilter<ImageType,ImageType> RescaleFilterType;

  typename ThresholdFilterType::Pointer thresholder = ThresholdFilterType::New();
  typename AddFilterType::Pointer       adder = AddFilterType::New();
  typename RescaleFilterType::Pointer   rescaler = RescaleFilterType::New();

  thresholder->SetLowerThreshold(lowThreshold);
  thresholder->SetUpperThreshold(2.0);
  thresholder->SetOutsideValue(0.0);
  thresholder->SetInsideValue(1.0);
  rescaler->SetOutputMaximum(1.0);
  rescaler->SetOutputMinimum(0.0);

  if (addImages)
    {
    adder->SetInput1(I1);
    adder->SetInput2(I2);
    rescaler->SetInput(adder->GetOutput());
    }
  else
    {
    rescaler->SetInput(I2);
    }

  thresholder->SetInput(rescaler->GetOutput());
  thresholder->Update();

  out->SetRegions(thresholder->GetOutput()->GetLargestPossibleRegion());
  out->SetSpacing(thresholder->GetOutput()->GetSpacing());
  out->SetOrigin(thresholder->GetOutput()->GetOrigin());

  out->Allocate();

  itk::ImageRegionConstIterator<ImageType> inputIterator(thresholder->GetOutput(), thresholder->GetOutput()->GetLargestPossibleRegion());
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

  typename ReaderType::Pointer reader1 = ReaderType::New();
  typename ReaderType::Pointer reader2 = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();

  reader1->SetFileName( inputFilename1  );
  reader2->SetFileName( inputFilename2  );
  writer->SetFileName( outputFilename );
  writer->UseCompressionOn();

  reader1->Update();
  reader2->Update();

  typename ImageType::Pointer input1 = reader1->GetOutput();
  typename ImageType::Pointer input2 = reader2->GetOutput();
  typename ImageType::Pointer output = ImageType::New();
  CreateMask<ImageType>(input1,input2,output,thresholdParameter,addImages);

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
  return EXIT_SUCCESS;
}

int main( int argc, char **argv)
{
  PARSE_ARGS;

  unsigned int uiSourceImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( inputFilename1 );

  std::cout << "Image size = " << uiSourceImageDimension << std::endl;

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
