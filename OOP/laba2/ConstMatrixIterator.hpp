#pragma once
#include "ConstMatrixIterator.h"
#include "exception.h"

template <MatrixElement T>
ConstMatrixIterator<T>::reference ConstMatrixIterator<T>::operator[](size_t n) const noexcept
{
    this->assertDerefAbility(std::source_location::current(), n);
    size_t idx{this->_idx + n};
    std::shared_ptr<typename Matrix<T>::MatrixRow[]> elems{this->_elems.lock()};
    return elems[idx / this->_colCount][idx % this->_colCount];
}

template <MatrixElement T>
ConstMatrixIterator<T>::pointer ConstMatrixIterator<T>::operator->() const
{
    this->assertDerefAbility(std::source_location::current());
    std::shared_ptr<typename Matrix<T>::MatrixRow[]> elems{this->_elems.lock()};
    return std::addressof(this->elems[this->_idx / this->_colCount][this->_idx % this->_colCount]);
}

template <MatrixElement T>
ConstMatrixIterator<T>::reference ConstMatrixIterator<T>::operator*() const
{
    this->assertDerefAbility(std::source_location::current());
    std::shared_ptr<typename Matrix<T>::MatrixRow[]> elems{this->_elems.lock()};
    return elems[this->_idx / this->_colCount][this->_idx % this->_colCount];
}
