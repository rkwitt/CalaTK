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
#include <itkBinaryDilateImageFilter.h>
#include <itkMaskImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkImageRegionIteratorWithIndex.h>

#include "RemoveMaskedRegionCLP.h"

typedef double TFLOAT;
const unsigned int VImageDimension = 2;


template < unsigned int VImageDimension >
int DoIt( int argc, char **argv )
{
    PARSE_ARGS;
    typedef float                                                ScalarPixelType;
    typedef itk::Image< ScalarPixelType, VImageDimension >       ImageType;
    typedef itk::ImageFileReader< ImageType >                    ReaderType;
    typedef itk::ImageFileWriter< ImageType >                    WriterType;
    typedef itk::ImageRegionIteratorWithIndex<ImageType>         ImageIteratorType;
    typedef itk::BinaryBallStructuringElement<ScalarPixelType,
                                              VImageDimension>   StructuringElementType;
    typedef itk::BinaryDilateImageFilter<ImageType,ImageType,
                                         StructuringElementType> DilateFilterType;
    typedef itk::MaskImageFilter<ImageType,ImageType>            MaskImageFilterType;

    typename ReaderType::Pointer readerI0 = ReaderType::New();
    typename ReaderType::Pointer readerI1 = ReaderType::New();
    typename ReaderType::Pointer readerM0 = ReaderType::New();
    typename ReaderType::Pointer readerM1 = ReaderType::New();
    typename WriterType::Pointer writerI0 = WriterType::New();
    typename WriterType::Pointer writerI1 = WriterType::New();
    typename DilateFilterType::Pointer dilateFilterM0 = DilateFilterType::New();
    typename DilateFilterType::Pointer dilateFilterM1 = DilateFilterType::New();
    typename MaskImageFilterType::Pointer maskImageFilterI0 = MaskImageFilterType::New();
    typename MaskImageFilterType::Pointer maskImageFilterI1 = MaskImageFilterType::New();

    readerI0->SetFileName( inputFilenameI0 );
    readerI1->SetFileName( inputFilenameI1 );
    readerM0->SetFileName( inputFilenameM0 );
    readerM1->SetFileName( inputFilenameM1 );

    writerI0->SetFileName( outputFilenameI0 );
    writerI0->UseCompressionOn();

    writerI1->SetFileName( outputFilenameI1 );
    writerI1->UseCompressionOn();

    StructuringElementType structuringElement;
    structuringElement.SetRadius(radius);
    structuringElement.CreateStructuringElement();

    dilateFilterM0->SetInput(readerM0->GetOutput());
    dilateFilterM0->SetKernel(structuringElement);
    dilateFilterM0->SetDilateValue(dilateValue);

    dilateFilterM1->SetInput(readerM1->GetOutput());
    dilateFilterM1->SetKernel(structuringElement);
    dilateFilterM1->SetDilateValue(dilateValue);

    maskImageFilterI0->SetInput(readerI0->GetOutput());
    maskImageFilterI0->SetMaskImage(dilateFilterM0->GetOutput());
    maskImageFilterI0->SetMaskingValue(maskingValue);
    maskImageFilterI0->SetOutsideValue(outsideValue);

    maskImageFilterI1->SetInput(readerI1->GetOutput());
    maskImageFilterI1->SetMaskImage(dilateFilterM1->GetOutput());
    maskImageFilterI1->SetMaskingValue(maskingValue);
    maskImageFilterI1->SetOutsideValue(outsideValue);

//     writer->SetInput( multiplyFilterM0->GetOutput() );
    writerI0->SetInput( maskImageFilterI0->GetOutput() );
    writerI1->SetInput( maskImageFilterI1->GetOutput() );
    try
    {
        writerI0->Update();
        writerI1->Update();
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

  unsigned int uiSourceImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( inputFilenameI0 );

  std::cout << "Image size = " << uiSourceImageDimension << std::endl;

  switch ( uiSourceImageDimension )
    {
    case 3:
      return DoIt<3>( argc, argv );
      break;
    case 4:
      return DoIt<4>( argc, argv );
      break;
    default:
      std::cerr << "Unsupported image dimension = " << uiSourceImageDimension << std::endl;
    }

  return EXIT_FAILURE;
}
