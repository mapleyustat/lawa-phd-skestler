/*
 *   Copyright (c) 2007, Michael Lehn
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

#ifndef FLENS_STORAGE_FULLSTORAGE_FULLSTORAGEVIEW_H
#define FLENS_STORAGE_FULLSTORAGE_FULLSTORAGEVIEW_H 1

#include <cxxblas/typedefs.h>
#include <flens/storage/indexoptions.h>
#include <flens/storage/storageinfo.h>

namespace flens {

template <typename T, cxxblas::StorageOrder Order, typename I, typename A>
    class FullStorage;

template <typename T, cxxblas::StorageOrder Order, typename I, typename A>
    class ConstFullStorageView;

template <typename T, cxxblas::StorageOrder Order, typename I, typename A>
class FullStorageView
{
    public:
        typedef T                                     ElementType;
        typedef typename I::IndexType                 IndexType;
        typedef A                                     Allocator;

        typedef ConstFullStorageView<T, Order, I, A>  ConstView;
        typedef FullStorageView                       View;
        typedef FullStorage<T, Order, I, A>           NoView;

        typedef flens::ConstArrayView<T, I, A>        ConstArrayView;
        typedef flens::ArrayView<T, I, A>             ArrayView;
        typedef flens::Array<T, I, A>                 Array;

        FullStorageView(ElementType *data, const Allocator &allocator,
                        IndexType numRows, IndexType numCols,
                        IndexType leadingDimension,
                        IndexType firstRow = I::defaultIndexBase,
                        IndexType firstCol = I::defaultIndexBase);

        FullStorageView(const FullStorageView &rhs);

        template <typename RHS>
            FullStorageView(RHS &rhs);

        ~FullStorageView();

        //-- operators ---------------------------------------------------------

        FullStorageView &
        operator=(const FullStorageView &rhs);

        template <typename RHS>
            FullStorageView &
            operator=(const RHS &rhs);

        const ElementType &
        operator()(IndexType row, IndexType col) const;

        ElementType &
        operator()(IndexType row, IndexType col);

        //-- methods -----------------------------------------------------------

        IndexType
        firstRow() const;

        IndexType
        lastRow() const;

        IndexType
        firstCol() const;

        IndexType
        lastCol() const;

        IndexType
        numRows() const;

        IndexType
        numCols() const;

        IndexType
        leadingDimension() const;

        IndexType
        strideRow() const;

        IndexType
        strideCol() const;

        const ElementType *
        data() const;

        ElementType *
        data();

        const Allocator &
        allocator() const;

        bool
        resize(IndexType numRows, IndexType numCols,
               IndexType firstRow = I::defaultIndexBase,
               IndexType firstCol = I::defaultIndexBase,
               const ElementType &value = ElementType());

        template <typename FS>
            bool
            resize(const FS &rhs, const ElementType &value = ElementType());

        bool
        fill(const ElementType &value = ElementType(0));

        void
        changeIndexBase(IndexType firstRow, IndexType firstCol);

        // view of rectangular part
        const ConstView
        view(IndexType fromRow, IndexType fromCol,
             IndexType toRow, IndexType toCol,
             IndexType firstViewRow = I::defaultIndexBase,
             IndexType firstViewCol = I::defaultIndexBase) const;

        FullStorageView
        view(IndexType fromRow, IndexType fromCol,
             IndexType toRow, IndexType toCol,
             IndexType firstViewRow = I::defaultIndexBase,
             IndexType firstViewCol = I::defaultIndexBase);

        // view of single row
        const ConstArrayView
        viewRow(IndexType row,
                IndexType firstViewIndex = I::defaultIndexBase) const;

        ArrayView
        viewRow(IndexType row,
                IndexType firstViewIndex = I::defaultIndexBase);

        // view of single column
        const ConstArrayView
        viewCol(IndexType col,
                IndexType firstViewIndex = I::defaultIndexBase) const;

        ArrayView
        viewCol(IndexType col,
                IndexType firstViewIndex = I::defaultIndexBase);

        // view of d-th diagonal
        const ConstArrayView
        viewDiag(IndexType d,
                 IndexType firstViewIndex = I::defaultIndexBase) const;

        ArrayView
        viewDiag(IndexType d,
                 IndexType firstViewIndex = I::defaultIndexBase);

    private:
        ElementType  *_data;
        Allocator    _allocator;
        IndexType    _numRows, _numCols;
        IndexType    _leadingDimension;
        IndexType    _firstRow, _firstCol;
};

//------------------------------------------------------------------------------

template <typename T, cxxblas::StorageOrder _Order, typename I, typename A>
struct StorageInfo<FullStorageView<T, _Order, I, A> >
{
    static const cxxblas::StorageOrder Order = _Order;
    static const typename I::IndexType defaultIndexBase = I::defaultIndexBase;
};

} // namespace flens

#include <flens/storage/fullstorage/fullstorageview.tcc>

#endif // FLENS_STORAGE_FULLSTORAGE_FULLSTORAGEVIEW_H
