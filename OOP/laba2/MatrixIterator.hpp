#pragma once
#include "MatrixIterator.h"
#include "exception.h"

template <MatrixElement T>
MatrixIterator<T>::reference MatrixIterator<T>::operator[](size_t n) noexcept
{
    this->assertDerefAbility(std::source_location::current(), n);
    size_t idx{this->_idx + n};
    std::shared_ptr<typename Matrix<T>::MatrixRow[]> elems{this->_elems.lock()};
    return elems[idx / this->_colCount][idx % this->_colCount];
}

template <MatrixElement T>
const MatrixIterator<T>::reference MatrixIterator<T>::operator[](size_t n) const noexcept
{
    this->assertDerefAbility(std::source_location::current(), n);
    size_t idx{this->_idx + n};
    std::shared_ptr<typename Matrix<T>::MatrixRow[]> elems{this->_elems.lock()};
    return elems[idx / this->_colCount][idx % this->_colCount];
}

template <MatrixElement T>
const MatrixIterator<T>::pointer MatrixIterator<T>::operator->() const
{
    this->assertDerefAbility(std::source_location::current());
    std::shared_ptr<typename Matrix<T>::MatrixRow[]> elems{this->_elems.lock()};
    return &elems[this->_idx / this->_colCount][this->_idx % this->_colCount];
}

template <MatrixElement T>
MatrixIterator<T>::pointer MatrixIterator<T>::operator->()
{
    this->assertDerefAbility(std::source_location::current());
    std::shared_ptr<typename Matrix<T>::MatrixRow[]> elems{this->_elems.lock()};
    return &elems[this->_idx / this->_colCount][this->_idx % this->_colCount];
}

template <MatrixElement T>
MatrixIterator<T>::reference MatrixIterator<T>::operator*()
{
    this->assertDerefAbility(std::source_location::current());
    std::shared_ptr<typename Matrix<T>::MatrixRow[]> elems{this->_elems.lock()};
    return elems[this->_idx / this->_colCount][this->_idx % this->_colCount];
}

template <MatrixElement T>
const MatrixIterator<T>::reference MatrixIterator<T>::operator*() const
{
    this->assertDerefAbility(std::source_location::current());
    std::shared_ptr<typename Matrix<T>::MatrixRow[]> elems{this->_elems.lock()};
    return elems[this->_idx / this->_colCount][this->_idx % this->_colCount];
}
