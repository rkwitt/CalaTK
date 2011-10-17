#ifndef C_KERNEL_TXX
#define C_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CKernel< T, VImageDimension >::CKernel()
{
  m_MemoryWasAllocated = false;
  m_KernelsNeedToBeComputed = true;

  m_ptrL = NULL;
  m_ptrLInv = NULL;
}

template <class T, unsigned int VImageDimension >
CKernel< T, VImageDimension >::~CKernel()
{
  DeallocateMemory();
}

template <class T, unsigned int VImageDimension >
void CKernel< T, VImageDimension >::DeallocateMemory()
{
  DeallocateMemoryForKernelAndInverseKernel();
}


template <class T, unsigned int VImageDimension >
void CKernel< T, VImageDimension >::DeallocateMemoryForKernelAndInverseKernel()
{
  if ( this->m_ptrL != NULL )
    {
    delete this->m_ptrL;
    this->m_ptrL = NULL;
    }

  if ( this->m_ptrLInv != NULL )
    {
    delete this->m_ptrLInv;
    this->m_ptrLInv = NULL;
    }

  this->m_MemoryWasAllocated = false;
  this->m_KernelsNeedToBeComputed = true;

}

template <class T, unsigned int VImageDimension >
void CKernel< T, VImageDimension >::AllocateMemoryForKernelAndInverseKernel( VectorImageType* pVecImageGraft )
{

  unsigned int szX = pVecImageGraft->getSizeX();
  unsigned int szY = pVecImageGraft->getSizeY();
  unsigned int szZ = pVecImageGraft->getSizeZ();

  assert( this->m_ptrL == NULL );
  assert( this->m_ptrLInv == NULL );

  switch ( VImageDimension )
    {
    case 2:
      this->m_ptrL = new VectorImageType( szX, szY, 1 );
      this->m_ptrLInv = new VectorImageType( szX, szY, 1 );
      break;
    case 3:
      this->m_ptrL = new VectorImageType( szX, szY, szZ, 1 );
      this->m_ptrLInv = new VectorImageType( szX, szY, szZ, 1 );
      break;
    default:
      throw std::runtime_error("Cannot allocate kernel memory of desired dimension.");
    }
}

#endif
