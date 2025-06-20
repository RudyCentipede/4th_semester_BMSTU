#pragma once

#include "MatrixIteratorBase.h"

#include <iterator>
#include <memory>
#include <concepts>

template <MatrixElement T>
class ConstMatrixIterator: public MatrixIteratorBase<ConstMatrixIterator<T>, T>
{
public:
    using value_type = const T;
    using pointer = const T*;
    using reference = const T&;

    using MatrixIteratorBase<ConstMatrixIterator<T>, T>::MatrixIteratorBase;
    ConstMatrixIterator(const ConstMatrixIterator<T>& other) noexcept = default;
    ConstMatrixIterator() noexcept = default;

    ~ConstMatrixIterator() noexcept override = default;

    reference operator[](size_t n) const noexcept;
    pointer operator->() const;
    reference operator*() const;
};

#include "ConstMatrixIterator.hpp"
