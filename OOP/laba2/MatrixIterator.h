#pragma once

#include "MatrixIteratorBase.h"

#include <iterator>
#include <memory>
#include "concept.h"

template <MatrixElement T>
class MatrixIterator: public MatrixIteratorBase<MatrixIterator<T>, T>
{
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    using MatrixIteratorBase<MatrixIterator<T>, T>::MatrixIteratorBase;
    MatrixIterator(const MatrixIterator& other) noexcept = default;
    MatrixIterator() noexcept = default;

    ~MatrixIterator() noexcept override = default;

    reference operator[](size_t n) noexcept;
    const reference operator[](size_t n) const noexcept;

    pointer operator->();
    const pointer operator->() const;
    reference operator*();
    const reference operator*() const;
};

#include "MatrixIterator.hpp"
