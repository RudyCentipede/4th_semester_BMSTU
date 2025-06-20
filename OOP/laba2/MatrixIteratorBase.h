#pragma once

#include <cstddef>
#include "concept.h"
#include <compare>
#include <memory>
#include <iterator>
#include "source_location"

template <MatrixElement T>
class Matrix;

template <typename DerivedIter, MatrixElement T>
class MatrixIteratorBase
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = ptrdiff_t;

    MatrixIteratorBase() noexcept = default;
    MatrixIteratorBase(const Matrix<T>& matrix, size_t idx);
    explicit MatrixIteratorBase(const Matrix<T>& matrix);
    MatrixIteratorBase(const MatrixIteratorBase& other) noexcept = default;

    virtual ~MatrixIteratorBase() noexcept = 0;

    difference_type operator-(const MatrixIteratorBase& other) const noexcept;
    DerivedIter& operator+=(size_t n) noexcept;
    DerivedIter& operator-=(size_t n) noexcept;
    DerivedIter operator-(size_t n) const noexcept;
    DerivedIter operator+(size_t n) const noexcept;
    DerivedIter& operator++() noexcept;
    DerivedIter operator++(int) noexcept;
    DerivedIter& operator--() noexcept;
    DerivedIter operator--(int) noexcept;

    std::strong_ordering operator<=>(const MatrixIteratorBase& other) const noexcept;
    bool operator==(const MatrixIteratorBase& other) const noexcept;

    explicit operator bool() const noexcept;

protected:
    void assertDerefAbility(const std::source_location& info, size_t idx = 0) const;

    size_t _idx;
    size_t _size;
    size_t _colCount;

    std::weak_ptr<typename Matrix<T>::MatrixRow[]> _elems;
};

template <typename DerivedIter, MatrixElement T>
DerivedIter operator+(size_t n, const MatrixIteratorBase<DerivedIter, T>& iter) noexcept;

#include "MatrixIteratorBase.hpp"


