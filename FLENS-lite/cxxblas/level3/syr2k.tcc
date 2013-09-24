/*
 *   Copyright (c) 2009, Michael Lehn
 *
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1) Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2) Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *   3) Neither the name of the FLENS development group nor the names of
 *      its contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

namespace cxxblas {

#ifdef HAVE_CBLAS

// ssyr2k
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
syr2k(StorageOrder order, StorageUpLo upLo,
      Transpose trans,
      IndexType n, IndexType k,
      float alpha,
      const float *A, IndexType ldA,
      const float *B, IndexType ldB,
      float beta,
      float *C, IndexType ldC)
{
    CXXBLAS_DEBUG_OUT("[" BLAS_IMPL "] cblas_ssyr2k");

    cblas_ssyr2k(CBLAS::value(order), CBLAS::value(upLo),
                 CBLAS::value(trans),
                 n, k,
                 alpha,
                 A, ldA,
                 B, ldB,
                 beta,
                 C, ldC);
}

// dsyr2k
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
syr2k(StorageOrder order, StorageUpLo upLo,
      Transpose trans,
      IndexType n, IndexType k,
      double alpha,
      const double *A, IndexType ldA,
      const double *B, IndexType ldB,
      double beta,
      double *C, IndexType ldC)
{
    CXXBLAS_DEBUG_OUT("[" BLAS_IMPL "] cblas_dsyr2k");

    cblas_dsyr2k(CBLAS::value(order), CBLAS::value(upLo),
                 CBLAS::value(trans),
                 n, k,
                 alpha,
                 A, ldA,
                 B, ldB,
                 beta,
                 C, ldC);
}

// csyr2k
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
syr2k(StorageOrder order, StorageUpLo upLo,
      Transpose trans,
      IndexType n, IndexType k,
      const ComplexFloat &alpha,
      const ComplexFloat *A, IndexType ldA,
      const ComplexFloat *B, IndexType ldB,
      const ComplexFloat &beta,
      ComplexFloat *C, IndexType ldC)
{
    CXXBLAS_DEBUG_OUT("[" BLAS_IMPL "] cblas_csyr2k");

    cblas_csyr2k(CBLAS::value(order), CBLAS::value(upLo),
                 CBLAS::value(trans),
                 n, k,
                 reinterpret_cast<const float *>(&alpha),
                 reinterpret_cast<const float *>(A), ldA,
                 reinterpret_cast<const float *>(B), ldB,
                 reinterpret_cast<const float *>(&beta),
                 reinterpret_cast<const float *>(C), ldC);
}

// zsyr2k
template <typename IndexType>
typename If<IndexType>::isBlasCompatibleInteger
syr2k(StorageOrder order, StorageUpLo upLo,
      Transpose trans,
      IndexType n, IndexType k,
      const ComplexDouble &alpha,
      const ComplexDouble *A, IndexType ldA,
      const ComplexDouble *B, IndexType ldB,
      const ComplexDouble &beta,
      ComplexDouble *C, IndexType ldC)
{
    CXXBLAS_DEBUG_OUT("[" BLAS_IMPL "] cblas_zsyr2k");

    cblas_zsyr2k(CBLAS::value(order), CBLAS::value(upLo),
                 CBLAS::value(trans),
                 n, k,
                 reinterpret_cast<const double *>(&alpha),
                 reinterpret_cast<const double *>(A), ldA,
                 reinterpret_cast<const double *>(B), ldB,
                 reinterpret_cast<const double *>(&beta),
                 reinterpret_cast<const double *>(C), ldC);
}

#endif // HAVE_CBLAS

} // namespace cxxblas
