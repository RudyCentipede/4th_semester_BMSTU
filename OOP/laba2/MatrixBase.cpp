#include "MatrixBase.h"

MatrixBase::MatrixBase(size_t rowCount, size_t colCount) noexcept
    : _rowCount{rowCount}, _colCount{colCount} { }

MatrixBase::~MatrixBase() noexcept {}

size_t MatrixBase::rowCount() const noexcept
{
    return _rowCount;
}

size_t MatrixBase::colCount() const noexcept
{
    return _colCount;
}
