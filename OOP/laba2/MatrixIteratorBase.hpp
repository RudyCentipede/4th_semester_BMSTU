#pragma once

#include "MatrixIteratorBase.h"
#include "exception.h"

template <typename DerivedIter, MatrixElement T>
MatrixIteratorBase<DerivedIter, T>::MatrixIteratorBase(const Matrix<T>& matrix, size_t idx)
{
    if (idx > matrix.size())
        throw MatrixIteratorRangeError(std::source_location::current(), "Index out of range");

    this->_colCount = matrix._colCount;
    this->_size = matrix.size();
    this->_elems = matrix._rows;
    this->_idx = idx;
}

template <typename DerivedIter, MatrixElement T>
MatrixIteratorBase<DerivedIter, T>::MatrixIteratorBase(const Matrix<T>& matrix)
    : MatrixIteratorBase(matrix, 0) {}

template <typename DerivedIter, MatrixElement T>
MatrixIteratorBase<DerivedIter, T>::~MatrixIteratorBase() noexcept { }

template <typename DerivedIter, MatrixElement T>
DerivedIter MatrixIteratorBase<DerivedIter, T>::operator-(size_t n) const noexcept
{
    DerivedIter iter{static_cast<DerivedIter>(*this)};
    iter._idx -= n;
    return iter;
}

template <typename DerivedIter, MatrixElement T>
DerivedIter MatrixIteratorBase<DerivedIter, T>::operator+(size_t n) const noexcept
{
    DerivedIter iter{static_cast<DerivedIter>(*this)};
    iter._idx += n;
    return iter;
}

template <typename DerivedIter, MatrixElement T>
DerivedIter operator+(size_t n, const MatrixIteratorBase<DerivedIter, T>& iter) noexcept
{
    return iter + n;
}

template <typename DerivedIter, MatrixElement T>
ptrdiff_t MatrixIteratorBase<DerivedIter, T>::operator-(const MatrixIteratorBase& other) const noexcept
{
    ptrdiff_t res{_idx};
    res -= other._idx;
    return res;
}

template <typename DerivedIter, MatrixElement T>
DerivedIter& MatrixIteratorBase<DerivedIter, T>::operator+=(size_t n) noexcept
{
    _idx += n;
    return static_cast<DerivedIter&>(*this);
}

template <typename DerivedIter, MatrixElement T>
DerivedIter& MatrixIteratorBase<DerivedIter, T>::operator-=(size_t n) noexcept
{
    _idx -= n;
    return static_cast<DerivedIter&>(*this);
}

template <typename DerivedIter, MatrixElement T>
DerivedIter& MatrixIteratorBase<DerivedIter, T>::operator++() noexcept
{
    ++_idx;
    return static_cast<DerivedIter&>(*this);
}

template <typename DerivedIter, MatrixElement T>
DerivedIter MatrixIteratorBase<DerivedIter, T>::operator++(int) noexcept
{
    DerivedIter iter{static_cast<DerivedIter>(*this)};
    ++_idx;
    return iter;
}

template <typename DerivedIter, MatrixElement T>
DerivedIter& MatrixIteratorBase<DerivedIter, T>::operator--() noexcept
{
    --_idx;
    return static_cast<DerivedIter&>(*this);
}

template <typename DerivedIter, MatrixElement T>
DerivedIter MatrixIteratorBase<DerivedIter, T>::operator--(int) noexcept
{
    DerivedIter iter{static_cast<DerivedIter>(*this)};
    --_idx;
    return iter;
}

template <typename DerivedIter, MatrixElement T>
MatrixIteratorBase<DerivedIter, T>::operator bool() const noexcept
{
    return !_elems.expired();
}

template <typename DerivedIter, MatrixElement T>
void MatrixIteratorBase<DerivedIter, T>::assertDerefAbility(const std::source_location& info, size_t idx) const
{
    if (_elems.expired())
        throw MatrixIteratorExpiredDataError(info, "Pointer to data is expired");
    if (_idx + idx >= _size)
        throw MatrixIteratorRangeError(info, "Index out of range");
}

template <typename DerivedIter, MatrixElement T>
std::strong_ordering MatrixIteratorBase<DerivedIter, T>::operator<=>(const MatrixIteratorBase<DerivedIter, T>& other) const noexcept
{
    return _idx <=> other._idx;
}

template <typename DerivedIter, MatrixElement T>
bool MatrixIteratorBase<DerivedIter, T>::operator==(const MatrixIteratorBase<DerivedIter, T>& other) const noexcept
{
    return _idx == other._idx;
}
