#ifndef MATRIXBASE_H
#define MATRIXBASE_H

#include <cstddef>

class MatrixBase
{
public:
    using size_type = size_t;

    MatrixBase(size_t rowCount = 0, size_t colCount = 0) noexcept;
    virtual ~MatrixBase() noexcept = 0;

    size_t rowCount() const noexcept;
    size_t colCount() const noexcept;

protected:
    size_t _rowCount;
    size_t _colCount;
};

#endif // MATRIXBASE_H
