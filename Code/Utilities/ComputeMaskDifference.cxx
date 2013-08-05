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
#include <itkDisplacementFieldTransform.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkMultiplyImageFilter.h>
#include <itkBinaryDilateImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkSubtractImageFilter.h>
#include <itkImageRegionIteratorWithIndex.h>

#include "ComputeMaskDifferenceCLP.h"

typedef double TFLOAT;
const unsigned int VImageDimension = 2;


template < unsigned int VImageDimension >
int DoIt( int argc, char **argv )
{
    PARSE_ARGS;
    typedef float                                                         ScalarPixelType;
    typedef itk::Vector<ScalarPixelType,VImageDimension>                  VectorPixelType;
    typedef itk::Image< ScalarPixelType, VImageDimension >                ImageType;
    typedef itk::Image< VectorPixelType, VImageDimension >                      Image3DType;
    typedef itk::ImageFileReader< ImageType >                             ReaderType;
    typedef itk::ImageFileReader< Image3DType >                           Reader3DType;
//     typedef itk::ImageFileWriter< ImageType >                           WriterType;
    typedef itk::ImageFileWriter< Image3DType >                           WriterType;
    typedef itk::ImageRegionIteratorWithIndex<ImageType>                  ImageIteratorType;
    typedef itk::ImageRegionIteratorWithIndex<Image3DType>                Image3DIteratorType;
    typedef itk::BinaryBallStructuringElement<ScalarPixelType,VImageDimension>      StructuringElementType;
    typedef itk::BinaryDilateImageFilter<ImageType,ImageType,
                                         StructuringElementType>          DilateFilterType;
    typedef itk::MultiplyImageFilter<ImageType,ImageType>                 MultiplyFilterType;
    typedef itk::SubtractImageFilter<ImageType,ImageType,ImageType>     SubtractFilterType;

    typename Reader3DType::Pointer bgReader = Reader3DType::New();
    typename ReaderType::Pointer readerI0 = ReaderType::New();
    typename ReaderType::Pointer readerI1 = ReaderType::New();
    typename ReaderType::Pointer readerM0 = ReaderType::New();
    typename ReaderType::Pointer readerM1 = ReaderType::New();
    typename WriterType::Pointer writer   = WriterType::New();

    bgReader->SetFileName( inputBgFieldFilename ); bgReader->Update();
    readerI0->SetFileName( inputFilenameI0 );
    readerI1->SetFileName( inputFilenameI1 );
    readerM0->SetFileName( inputFilenameM0 );
    readerM1->SetFileName( inputFilenameM1 );

    writer->SetFileName( outputFilename );
    writer->UseCompressionOn();

    typename DilateFilterType::Pointer dilateFilterM0 = DilateFilterType::New();
    typename DilateFilterType::Pointer dilateFilterM1 = DilateFilterType::New();
    typename SubtractFilterType::Pointer substractFilterM0 = SubtractFilterType::New();
    typename SubtractFilterType::Pointer substractFilterM1 = SubtractFilterType::New();
    typename MultiplyFilterType::Pointer multiplyFilterM0 = MultiplyFilterType::New();
    typename MultiplyFilterType::Pointer multiplyFilterM1 = MultiplyFilterType::New();

    typename Image3DType::Pointer input = bgReader->GetOutput();
    typename Image3DType::Pointer outputImage = Image3DType::New();
    outputImage->SetRegions(input->GetLargestPossibleRegion());
    outputImage->SetSpacing(input->GetSpacing());
    outputImage->SetOrigin(input->GetOrigin());
    outputImage->Allocate();

    StructuringElementType structuringElement;
    structuringElement.SetRadius(radius);
    structuringElement.CreateStructuringElement();

    dilateFilterM0->SetInput(readerM0->GetOutput());
    dilateFilterM0->SetKernel(structuringElement);
    dilateFilterM0->SetDilateValue(dilateValue);

    dilateFilterM1->SetInput(readerM1->GetOutput());
    dilateFilterM1->SetKernel(structuringElement);
    dilateFilterM1->SetDilateValue(dilateValue);

    substractFilterM0->SetInput1(dilateFilterM0->GetOutput());
    substractFilterM0->SetInput2(readerM0->GetOutput());

    substractFilterM1->SetInput1(dilateFilterM1->GetOutput());
    substractFilterM1->SetInput2(readerM1->GetOutput());

    multiplyFilterM0->SetInput1(substractFilterM0->GetOutput());
    multiplyFilterM0->SetInput2(readerI0->GetOutput());
    multiplyFilterM0->Update();

    multiplyFilterM1->SetInput1(substractFilterM1->GetOutput());
    multiplyFilterM1->SetInput2(readerI1->GetOutput());
    multiplyFilterM1->Update();

    Image3DIteratorType input_i(input,input->GetRequestedRegion());
    Image3DIteratorType output_i(outputImage,outputImage->GetRequestedRegion());
    ImageIteratorType mask_i(multiplyFilterM0->GetOutput(),
                           multiplyFilterM0->GetOutput()->GetRequestedRegion());
    for(mask_i.GoToBegin(),
        input_i.GoToBegin(),
        output_i.GoToBegin(); !mask_i.IsAtEnd();  ++mask_i, ++input_i, ++output_i)
    {
        if(mask_i.Get() > 0)
          output_i.Set(input_i.Get());
    }

//     writer->SetInput( multiplyFilterM0->GetOutput() );
    writer->SetInput( outputImage );
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

  unsigned int uiSourceVectorImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( inputBgFieldFilename );
  unsigned int uiSourceImageDimension = CALATK::GetNonSingletonImageDimensionFromFile( inputFilenameI0 );

  std::cout << "Vector field size = " << uiSourceVectorImageDimension << std::endl;
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
