#pragma once

#include "Matrix.h"
#include "exception.h"
#include "concept.h"
#include <algorithm>

template <MatrixElement T>
Matrix<T>::MatrixRow::MatrixRow(T* elems, size_t len) noexcept
    : _elems{elems}, _len{len} {}

template <MatrixElement T>
Matrix<T>::MatrixRow::MatrixRow() noexcept
    : _elems{nullptr}, _len{0} {}

template <MatrixElement T>
Matrix<T>::MatrixRow::MatrixRow(const MatrixRow& other)
{
    _len = other._len;
    allocateMemory(_len);

    for (size_t i{0}; i < _len; ++i)
        _elems[i] = other._elems[i];
}

template <MatrixElement T>
Matrix<T>::MatrixRow::MatrixRow(MatrixRow&& other) noexcept
    : _elems{std::move(other._elems)}, _len{other._len}
{
    other._len = 0;
}

template <MatrixElement T>
Matrix<T>::MatrixRow& Matrix<T>::MatrixRow::operator=(const MatrixRow& other)
{
    if (std::addressof(other) == this)
        return *this;

    if (_len != other._len)
    {
        _len = other._len;
        allocateMemory(_len);
    }

    for (size_t i{0}; i < _len; ++i)
        _elems[i] = other._elems[i];
}

template <MatrixElement T>
Matrix<T>::MatrixRow& Matrix<T>::MatrixRow::operator=(MatrixRow&& other) noexcept
{
    if (std::addressof(other) == this)
        return *this;

    _elems = std::move(other._elems);
    _len = other._len;
    other._len = 0;

    return *this;
}

template <MatrixElement T>
Matrix<T>::MatrixRow& Matrix<T>::MatrixRow::operator=(const std::initializer_list<T>& elems)
{
    if (elems.size() != _len)
        throw MatrixErrorArgument(std::source_location::current(), "Incorrect argument size");

    size_t idx{0};
    for (const auto& x: elems)
        _elems[idx++] = x;

    return *this;
}

template <MatrixElement T>
T& Matrix<T>::MatrixRow::operator[](size_t idx)
{
    if (idx >= _len)
        throw MatrixErrorRange(std::source_location::current(), "Index out of range");

    return _elems[idx];
}

template <MatrixElement T>
const T& Matrix<T>::MatrixRow::operator[](size_t idx) const
{
    if (idx >= _len)
        throw MatrixErrorRange(std::source_location::current(), "Index out of range");

    return _elems[idx];
}

template <MatrixElement T>
void Matrix<T>::MatrixRow::allocateMemory(size_t len)
{
    _len = len;
    _elems = std::make_shared<T[]>(len);
}

template <MatrixElement T>
void Matrix<T>::MatrixRow::fill(const T& value) noexcept
{
    for (size_t i{0}; i < _len; ++i)
        _elems[i] = value;
}

template <MatrixElement T>
template <MatrixElement OtherT>
void Matrix<T>::assertSizeEquality(const std::source_location& info, const Matrix<OtherT>& other) const
    requires std::convertible_to<OtherT, T>
{
    if (other.rowCount() != _rowCount || other.colCount() != _colCount)
        throw MatrixErrorArgument(info, "Incorrect argument size");
}

template <MatrixElement T>
Matrix<T>::Matrix(size_t rowCount, size_t colCount)
{
    allocateMemory(rowCount, colCount);
}

template <MatrixElement T>
Matrix<T>::Matrix(const T& value, size_t rowCount, size_t colCount)
    : Matrix(rowCount, colCount)
{
    fill(value);
}

template <MatrixElement T>
template <MatrixElement OtherT>
void Matrix<T>::copyFrom(const Matrix<OtherT>& other) requires std::convertible_to<OtherT, T>
{
    assertSizeEquality(std::source_location::current(), other);
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            _rows[i][j] = other[i][j];
}

template <MatrixElement T>
Matrix<T>::Matrix(const Matrix<T>& other)
{
    allocateMemory(other._rowCount, other._colCount);
    copyFrom(other);
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>::Matrix(const Matrix<U>& other) requires std::convertible_to<U, T>
{
    allocateMemory(other._rowCount, other._colCount);
    copyFrom(other);
}

template <MatrixElement T>
Matrix<T>::Matrix(Matrix<T>&& other) noexcept
    : MatrixBase(other._rowCount, other._colCount), _rows{std::move(other._rows)}
{
    other._rowCount = 0;
    other._colCount = 0;
}

template <MatrixElement T>
void Matrix<T>::allocateMemory(size_t rowCount, size_t colCount)
{
    _rowCount = rowCount;
    _colCount = colCount;
    _rows = std::make_shared<MatrixRow[]>(rowCount);
    for (size_t i{0}; i < rowCount; ++i)
        _rows[i].allocateMemory(colCount);
}

template <MatrixElement T>
void Matrix<T>::init(const std::initializer_list<std::initializer_list<T>>& data)
{
    size_t rowIdx{0}, colIdx{0};
    for (const auto &row : data)
    {
        for (const auto &elem : row)
            _rows[rowIdx][colIdx++] = elem;
        ++rowIdx;
        colIdx = 0;
    }
}

template <MatrixElement T>
Matrix<T>::Matrix(const std::initializer_list<std::initializer_list<T>>& data)
{
    auto listPtr{data.begin()};
    size_t rowCount{data.size()};
    size_t colCount{listPtr->size()};
    for (const auto &row : data)
        if (colCount < row.size())
            colCount = row.size();

    allocateMemory(rowCount, colCount);
    init(data);
}

template <MatrixElement T>
Matrix<T>::Matrix(T** matrix, size_t rowCount, size_t colCount)
{
    if (matrix == nullptr || *matrix == nullptr)
        throw MatrixErrorNullptr(std::source_location::current(), "Pointer to nullptr");
    allocateMemory(rowCount, colCount);
    for (size_t i{0}; i < rowCount; ++i)
        for (size_t j{0}; j < colCount; ++j)
            _rows[i][j] = matrix[i][j];
}

template <MatrixElement T>
template <std::input_iterator InIter, std::sentinel_for<InIter> Sentinel>
Matrix<T>::Matrix(InIter beg, Sentinel end, size_t rowCount, size_t colCount)
    requires std::convertible_to<typename InIter::value_type, T>
{
    allocateMemory(rowCount, colCount);
    size_t idx{0};
    for (; beg != end && idx < rowCount * colCount; ++beg)
    {
        _rows[idx / colCount][idx % colCount] = *beg;
        ++idx;
    }
}

template <MatrixElement T>
template <typename Cont>
Matrix<T>::Matrix(const Cont& container, size_t rowCount, size_t colCount)
    requires ContainerOf<Cont, T>
{
    allocateMemory(rowCount, colCount);
    size_t idx{0};
    for (auto iter{container.begin()}; iter != container.end() && idx < rowCount * colCount; ++iter)
    {
        _rows[idx / colCount][idx % colCount] = *iter;
        ++idx;
    }
}

template <MatrixElement T>
void Matrix<T>::fill(const T& value) noexcept
{
    for (size_t i{0}; i < _rowCount; ++i)
        _rows[i].fill(value);
}

template <MatrixElement T>
bool Matrix<T>::empty() const noexcept
{
    if (_rowCount == 0 || _colCount == 0)
        return true;
    return false;
}

template <MatrixElement T>
size_t Matrix<T>::size() const noexcept
{
    return _colCount * _rowCount;
}

template <MatrixElement T>
template <MatrixElement OtherT>
Matrix<T>& Matrix<T>::operator=(const Matrix<OtherT>& other) requires std::convertible_to<OtherT, T>
{
    allocateMemory(other.rowCount(), other.colCount());
    copyFrom(other);

    return *this;
}

template <MatrixElement T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other)
{
    if (std::addressof(other) == this)
        return *this;

    allocateMemory(other._rowCount, other._colCount);
    copyFrom(other);

    return *this;
}

template <MatrixElement T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other) noexcept
{
    if (std::addressof(other) == this)
        return *this;

    _rows = std::move(other._rows);
    _rowCount = other._rowCount;
    _colCount = other._colCount;

    return *this;
}

template <MatrixElement T>
typename Matrix<T>::MatrixRow& Matrix<T>::operator[](size_t idx)
{
    if (idx >= _rowCount)
        throw MatrixErrorRange(std::source_location::current(), "Index out of range");

    return _rows[idx];
}

template <MatrixElement T>
const typename Matrix<T>::MatrixRow& Matrix<T>::operator[](size_t idx) const
{
    if (idx >= _rowCount)
        throw MatrixErrorRange(std::source_location::current(), "Index out of range");

    return _rows[idx];
}

template <MatrixElement T>
T& Matrix<T>::operator()(size_t rowIdx, size_t colIdx)
{
    return _rows[rowIdx][colIdx];
}

template <MatrixElement T>
const T& Matrix<T>::operator()(size_t rowIdx, size_t colIdx) const
{
    return _rows[rowIdx][colIdx];
}

template <MatrixElement T>
MatrixIterator<T> Matrix<T>::begin() noexcept
{
    return MatrixIterator<T>(*this);
}

template <MatrixElement T>
MatrixIterator<T> Matrix<T>::end() noexcept
{
    return MatrixIterator<T>(*this, size());
}

template <MatrixElement T>
ConstMatrixIterator<T> Matrix<T>::begin() const noexcept
{
    return ConstMatrixIterator<T>(*this);
}

template <MatrixElement T>
ConstMatrixIterator<T> Matrix<T>::end() const noexcept
{
    return ConstMatrixIterator<T>(*this, size());
}

template <MatrixElement T>
ConstMatrixIterator<T> Matrix<T>::cbegin() const noexcept
{
    return ConstMatrixIterator<T>(*this);
}

template <MatrixElement T>
ConstMatrixIterator<T> Matrix<T>::cend() const noexcept
{
    return ConstMatrixIterator<T>(*this, size());
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::operator+=(const U& elem) noexcept requires AddAssignable<T, U>
{
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            _rows[i][j] += elem;
    return *this;
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::operator-=(const U& elem) noexcept requires SubAssignable<T, U>
{
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            _rows[i][j] -= elem;
    return *this;
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::operator*=(const U& elem) noexcept requires MulAssignable<T, U>
{
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            _rows[i][j] *= elem;
    return *this;
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::operator/=(const U& elem) noexcept requires DivAssignable<T, U>
{
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            _rows[i][j] /= elem;
    return *this;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::operator+(const U& elem) const requires Addable<T, U>
{
    Matrix<std::common_type_t<T, U>> resMatrix{_rowCount, _colCount};
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            resMatrix[i][j] = _rows[i][j] + elem;
    return resMatrix;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::operator-(const U& elem) const requires Subtractable<T, U>
{
    Matrix<std::common_type_t<T, U>> resMatrix{_rowCount, _colCount};
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            resMatrix[i][j] = _rows[i][j] - elem;
    return resMatrix;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::operator*(const U& elem) const requires Multipliable<T, U>
{
    Matrix<std::common_type_t<T, U>> resMatrix{_rowCount, _colCount};
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            resMatrix[i][j] = _rows[i][j] * elem;
    return resMatrix;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::operator/(const U& elem) const requires Divisible<T, U>
{
    Matrix<std::common_type_t<T, U>> resMatrix{_rowCount, _colCount};
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            resMatrix[i][j] = _rows[i][j] / elem;
    return resMatrix;
}

template <MatrixElement T, MatrixElement U>
decltype(auto) operator+(const T& elem, const Matrix<U>& matrix) requires Addable<U, T>
{
    return matrix + elem;
}

template <MatrixElement T, MatrixElement U>
decltype(auto) operator*(const T& elem, const Matrix<U>& matrix) requires Multipliable<U, T>
{
    return matrix * elem;
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::operator+=(const Matrix<U>& other) requires AddAssignable<T, U>
{
    assertSizeEquality(std::source_location::current(), other);
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            _rows[i][j] += other[i][j];
    return *this;
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::operator-=(const Matrix<U>& other) requires SubAssignable<T, U>
{
    assertSizeEquality(std::source_location::current(), other);
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            _rows[i][j] -= other[i][j];
    return *this;
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::operator*=(const Matrix<U>& other) requires MulAssignable<T, U>
{
    assertSizeEquality(std::source_location::current(), other);
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            _rows[i][j] *= other[i][j];
    return *this;
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::operator/=(const Matrix<U>& other) requires DivAssignable<T, U>
{
    assertSizeEquality(std::source_location::current(), other);
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            _rows[i][j] /= other[i][j];
    return *this;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::operator+(const Matrix<U>& other) const requires Addable<T, U>
{
    assertSizeEquality(std::source_location::current(), other);
    Matrix<std::common_type_t<T, U>> resMatrix{_rowCount, _colCount};
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            resMatrix[i][j] = _rows[i][j] + other[i][j];
    return resMatrix;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::operator*(const Matrix<U>& other) const requires Multipliable<T, U>
{
    assertSizeEquality(std::source_location::current(), other);
    Matrix<std::common_type_t<T, U>> resMatrix{_rowCount, _colCount};
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            resMatrix[i][j] = _rows[i][j] * other[i][j];
    return resMatrix;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::operator/(const Matrix<U>& other) const requires Divisible<T, U>
{
    assertSizeEquality(std::source_location::current(), other);
    Matrix<std::common_type_t<T, U>> resMatrix{_rowCount, _colCount};
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            resMatrix[i][j] = _rows[i][j] / other[i][j];
    return resMatrix;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::operator-(const Matrix<U>& other) const requires Subtractable<T, U>
{
    assertSizeEquality(std::source_location::current(), other);
    Matrix<std::common_type_t<T, U>> resMatrix{_rowCount, _colCount};
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            resMatrix[i][j] = _rows[i][j] - other[i][j];
    return resMatrix;
}

template <MatrixElement T>
Matrix<T> Matrix<T>::operator-() const noexcept requires Signed<T>
{
    Matrix<T> resMatrix{*this};
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            resMatrix[i][j] = -resMatrix[i][j];
    return resMatrix;
}

template <MatrixElement T>
Matrix<T> Matrix<T>::transposed() const
{
    Matrix<T> transposedMatrix(_colCount, _rowCount);
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            transposedMatrix[j][i] = _rows[i][j];
    return transposedMatrix;
}

template <MatrixElement T>
void Matrix<T>::transpose()
{
    Matrix<T> transposedMatrix{transposed()};
    *this = std::move(transposedMatrix);
}

template <MatrixElement T>
bool Matrix<T>::isSquare() const noexcept
{
    return _rowCount == _colCount;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::dot(const Matrix<U>& other) const
    requires SelfAddAssignable<std::common_type_t<T, U>> && Multipliable<T, U>
{
    if (_colCount != other._rowCount)
        throw MatrixErrorArgument(std::source_location::current(),
                                  "dot error: The number of columns of the first matrix does not match the number of rows of the second");

    Matrix<std::common_type_t<T, U>> res{_rowCount, other._colCount};
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < other._colCount; ++j)
            for (size_t k{0}; k < _colCount; ++k)
                res[i][j] += _rows[i][k] * other[k][j];
    return res;
}

template <MatrixElement T>
void Matrix<T>::reshape(size_t rowCount, size_t colCount)
{
    if (rowCount * colCount != size())
        throw MatrixErrorArgument(std::source_location::current(),
                                  "reshape error: the size of the resulting matrix does not match the current one");

    Matrix<T> resMatrix{rowCount, colCount};
    size_t idx{0};
    for (auto& x: resMatrix)
    {
        x = _rows[idx / _colCount][idx % _colCount];
        ++idx;
    }
    *this = std::move(resMatrix);
}

template <MatrixElement T>
void Matrix<T>::resize(size_t rowCount, size_t colCount, const T& fillElem)
{
    Matrix<T> resMatrix{rowCount, colCount};
    size_t idx{0};
    for (size_t i{0}; i < rowCount; ++i)
        for (size_t j{0}; j < colCount; ++j)
            if (i < _rowCount && j < _colCount)
                resMatrix[i][j] = _rows[i][j];
            else
                resMatrix[i][j] = fillElem;
    *this = std::move(resMatrix);
}

template <MatrixElement T>
bool Matrix<T>::isValidRowIndex(size_t idx) const noexcept
{
    if (idx < _rowCount)
        return true;
    return false;
}

template <MatrixElement T>
bool Matrix<T>::isValidColIndex(size_t idx) const noexcept
{
    if (idx < _colCount)
        return true;
    return false;
}

template <MatrixElement T>
bool Matrix<T>::isValidIndex(size_t rowIdx, size_t colIdx) const noexcept
{
    return isValidRowIndex(rowIdx) && isValidColIndex(colIdx);
}

template <MatrixElement T>
void Matrix<T>::assertRowIndexValidity(const std::source_location& info, size_t index) const
{
    if (!isValidRowIndex(index))
        throw MatrixErrorRange(info, "Row index out of range");
}

template <MatrixElement T>
void Matrix<T>::assertColIndexValidity(const std::source_location& info, size_t index) const
{
    if (!isValidColIndex(index))
        throw MatrixErrorRange(info, "Col index out of range");
}

template <MatrixElement T>
void Matrix<T>::assertIndexValidity(const std::source_location& info, size_t rowIdx, size_t colIdx) const
{
    if (!isValidRowIndex(rowIdx) || !isValidColIndex(colIdx))
        throw MatrixErrorRange(info, "Index out of range");
}

template <MatrixElement T>
void Matrix<T>::removeRow(size_t idx)
{
    assertRowIndexValidity(std::source_location::current(), idx);
    Matrix<T> resMatrix{_rowCount - 1, _colCount};
    size_t curIdx{0};
    for (size_t i{0}; i < _rowCount; ++i)
        if (i != idx)
            resMatrix[curIdx++] = std::move(_rows[i]);
    *this = std::move(resMatrix);
}

template <MatrixElement T>
void Matrix<T>::removeCol(size_t idx)
{
    assertColIndexValidity(std::source_location::current(), idx);
    Matrix<T> resMatrix{_rowCount, _colCount - 1};
    for (size_t i{0}; i < _rowCount; ++i)
    {
        size_t colIdx{0};
        for (size_t j{0}; j < _colCount; ++j)
            if (j != idx)
                resMatrix[i][colIdx++] = std::move(_rows[i][j]);
    }
    *this = std::move(resMatrix);
}

template <MatrixElement T>
void Matrix<T>::insertRow(size_t idx, const T& fillElem)
{
    assertRowInsertability(std::source_location::current(), idx);
    Matrix<T> resMatrix{_rowCount + 1, _colCount};
    size_t rowIdx{0};
    for (size_t i{0}; i < resMatrix._rowCount; ++i)
        if (i == idx)
            resMatrix[i].fill(fillElem);
        else
            resMatrix[i] = std::move(_rows[rowIdx++]);
    *this = std::move(resMatrix);
}

template <MatrixElement T>
void Matrix<T>::insertCol(size_t idx, const T& fillElem)
{
    assertColInsertability(std::source_location::current(), idx);
    Matrix<T> resMatrix{_rowCount, _colCount + 1};
    for (size_t i{0}; i < _rowCount; ++i)
    {
        size_t colIdx{0};
        for (size_t j{0}; j < resMatrix._colCount; ++j)
            if (j == idx)
                resMatrix[i][j] = fillElem;
            else
                resMatrix[i][j] = std::move(_rows[i][colIdx++]);
    }
    *this = std::move(resMatrix);
}

template <MatrixElement T>
void Matrix<T>::insertRow(size_t idx, const std::initializer_list<T>& elems)
{
    assertRowInsertability(std::source_location::current(), idx, elems);
    Matrix<T> resMatrix{_rowCount + 1, _colCount};
    size_t rowIdx{0};
    for (size_t i{0}; i < resMatrix._rowCount; ++i)
        if (i == idx)
            resMatrix[i] = elems;
        else
            resMatrix[i] = std::move(_rows[rowIdx++]);
    *this = std::move(resMatrix);
}

template <MatrixElement T>
void Matrix<T>::insertCol(size_t idx, const std::initializer_list<T>& elems)
{
    assertColInsertability(std::source_location::current(), idx, elems);
    Matrix<T> resMatrix{_rowCount, _colCount + 1};
    auto iter{elems.begin()};
    for (size_t i{0}; i < _rowCount; ++i)
    {
        size_t colIdx{0};
        for (size_t j{0}; j < resMatrix._colCount; ++j)
            if (j == idx)
                resMatrix[i][j] = *(iter++);
            else
                resMatrix[i][j] = std::move(_rows[i][colIdx++]);
    }
    *this = std::move(resMatrix);
}

template <MatrixElement T>
void Matrix<T>::assertRowInsertability(const std::source_location& info, size_t rowIdx) const
{
    if (rowIdx > _rowCount)
        throw MatrixErrorRange(info, "Row insertion index out of range");
}

template <MatrixElement T>
void Matrix<T>::assertColInsertability(const std::source_location& info, size_t colIdx) const
{
    if (colIdx > _colCount)
        throw MatrixErrorRange(info, "Col insertion index out of range");
}

template <MatrixElement T>
void Matrix<T>::assertRowInsertability(const std::source_location& info, size_t rowIdx, const std::initializer_list<T>& elems) const
{
    assertRowInsertability(info, rowIdx);
    if (elems.size() != _colCount)
        throw MatrixErrorArgument(info, "Incorrect argument size");
}

template <MatrixElement T>
void Matrix<T>::assertColInsertability(const std::source_location& info, size_t colIdx, const std::initializer_list<T>& elems) const
{
    assertColInsertability(info, colIdx);
    if (elems.size() != _rowCount)
        throw MatrixErrorArgument(info, "Incorrect argument size");
}

template <MatrixElement T>
T Matrix<T>::determinant() const requires SignedNumber<T>
{
    if (empty())
        throw MatrixErrorArgument(std::source_location::current(), "determinant error: matrix is empty");
    if (!isSquare())
        throw MatrixErrorArgument(std::source_location::current(), "determinant error: non square matrix");
    if (_rowCount == 1 && _colCount == 1)
        return _rows[0][0];

    T res;
    Matrix<T> matrixCopy{*this};
    bool zeroDet{false};
    size_t swapCounter{0};
    for (size_t j{0}; !zeroDet && j < _colCount - 1; ++j)
    {
        size_t pivotRowIdx{j};
        while (pivotRowIdx < _rowCount && matrixCopy[pivotRowIdx][j] == 0)
            ++pivotRowIdx;
        if (pivotRowIdx == _rowCount)
            zeroDet = true;
        else
        {
            if (pivotRowIdx != j)
            {
                ++swapCounter;
                std::swap(matrixCopy[pivotRowIdx], matrixCopy[j]);
            }

            for (size_t i{j + 1}; i < _rowCount; ++i)
            {
                T factor{matrixCopy[i][j] / matrixCopy[j][j]};
                for (size_t k{j}; k < _colCount; ++k)
                    matrixCopy[i][k] -= matrixCopy[j][k] * factor;
            }

            if (j == 0)
                res = matrixCopy[0][0];
            else
                res *= matrixCopy[j][j];
        }
    }
    res *= matrixCopy[_rowCount - 1][_colCount - 1];

    if (zeroDet)
        res = T();
    if (swapCounter % 2 == 1)
        res = -res;
    return res;
}

template <MatrixElement T>
Matrix<T> Matrix<T>::additionalMinor(size_t rowIdx, size_t colIdx) const
    requires SignedNumber<T>
{
    assertIndexValidity(std::source_location::current(), rowIdx, colIdx);

    Matrix<T> res{_rowCount - 1, _colCount - 1};
    size_t resRowIdx{0};
    for (size_t i{0}; i < _rowCount; ++i)
        if (i != rowIdx)
        {
            size_t resColIdx{0};
            for (size_t j{0}; j < _colCount; ++j)
                if (j != colIdx)
                    res[resRowIdx][resColIdx++] = _rows[i][j];
            ++resRowIdx;
        }

    return res;
}

template <MatrixElement T>
T Matrix<T>::algebraicExtension(size_t rowIdx, size_t colIdx) const
    requires SignedNumber<T>
{
    if (empty())
        throw MatrixErrorArgument(std::source_location::current(), "algebraicExtension error: matrix is empty");
    if (!isSquare())
        throw MatrixErrorArgument(std::source_location::current(), "algebraicExtension error: non square matrix");

    Matrix<T> minor{additionalMinor(rowIdx, colIdx)};
    T res{minor.determinant()};
    res = (rowIdx + colIdx) % 2 == 0 ? res : -res;
    return res;
}

template <MatrixElement T>
Matrix<T> Matrix<T>::inverse() const requires SignedNumber<T>
{
    if (empty())
        throw MatrixErrorArgument(std::source_location::current(), "inverse error: matrix is empty");
    if (!isSquare())
        throw MatrixErrorArgument(std::source_location::current(), "inverse error: non square matrix");

    T det{determinant()};
    if (det == T())
        throw MatrixErrorArgument(std::source_location::current(), "inverse error: zero determinant");

    Matrix<T> resMatrix{_rowCount, _colCount};
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            resMatrix[i][j] = algebraicExtension(j, i);

    resMatrix *= 1 / det;

    return resMatrix;
}

template <MatrixElement T>
bool Matrix<T>::isZero() const noexcept
{
    T zero = T();
    for (const auto& x: *this)
        if (x != zero)
            return false;
    return true;
}

template <MatrixElement T>
bool Matrix<T>::isIdentity() const requires SignedNumber<T>
{
    if (empty())
        throw MatrixErrorArgument(std::source_location::current(), "isIdentity error: matrix is empty");
    if (!isSquare())
        throw MatrixErrorArgument(std::source_location::current(), "isIdentity error: non square matrix");

    T zero = T();
    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            if (_rows[i][j] != zero && i != j || _rows[i][j] != 1 && i == j)
                return false;
    return true;
}

template <MatrixElement T>
auto Matrix<T>::operator<=>(const Matrix<T>& other) const noexcept requires ThreeWayComparable<T>
{
    if (auto cmp = _rowCount <=> other._rowCount; cmp != 0)
        return cmp;
    if (auto cmp = _colCount <=> other._colCount; cmp != 0)
        return cmp;

    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            if (auto cmp = _rows[i][j] <=> other[i][j]; cmp != 0)
                return cmp;

    return std::strong_ordering::equal;
}

template <MatrixElement T>
bool Matrix<T>::operator==(const Matrix<T>& other) const noexcept
{
    if (_rowCount != other._rowCount || _colCount != other._colCount)
        return false;

    for (size_t i{0}; i < _rowCount; ++i)
        for (size_t j{0}; j < _colCount; ++j)
            if (_rows[i][j] != other[i][j])
                return false;
    return true;
}

template <MatrixElement T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) noexcept
{
    for (size_t i{0}; i < matrix.rowCount(); ++i)
    {
        for (size_t j{0}; j < matrix.colCount(); ++j)
            os << matrix[i][j] << " ";
        os << "\n";
    }

    return os;
}

template <MatrixElement T>
Matrix<T> Matrix<T>::identity(size_t size) requires SignedNumber<T>
{
    Matrix<T> identityMatrix{size, size};
    for (size_t i{0}; i < size; ++i)
        identityMatrix[i][i] = 1;
    return identityMatrix;
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::addAssign(const U& elem) noexcept requires AddAssignable<T, U>
{
    return *this += elem;
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::subAssign(const U& elem) noexcept requires SubAssignable<T, U>
{
    return *this -= elem;
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::divAssign(const U& elem) noexcept requires DivAssignable<T, U>
{
    return *this /= elem;
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::multAssign(const U& elem) noexcept requires MulAssignable<T, U>
{
    return *this *= elem;
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::addAssign(const Matrix<U>& other) requires AddAssignable<T, U>
{
    return *this += other;
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::subAssign(const Matrix<U>& other) requires SubAssignable<T, U>
{
    return *this -= other;
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::divAssign(const Matrix<U>& other) requires DivAssignable<T, U>
{
    return *this /= other;
}

template <MatrixElement T>
template <MatrixElement U>
Matrix<T>& Matrix<T>::multAssign(const Matrix<U>& other) requires MulAssignable<T, U>
{
    return *this *= other;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::add(const Matrix<U>& other) const requires Addable<T, U>
{
    return *this + other;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::mult(const Matrix<U>& other) const requires Multipliable<T, U>
{
    return *this * other;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::div(const Matrix<U>& other) const requires Divisible<T, U>
{
    return *this / other;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::sub(const Matrix<U>& other) const requires Subtractable<T, U>
{
    return *this - other;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::add(const U& elem) const requires Addable<T, U>
{
    return *this + elem;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::sub(const U& elem) const requires Subtractable<T, U>
{
    return *this - elem;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::div(const U& elem) const requires Divisible<T, U>
{
    return *this / elem;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::mult(const U& elem) const requires Multipliable<T, U>
{
    return *this * elem;
}

template <MatrixElement T>
Matrix<T> Matrix<T>::neg() const noexcept requires Signed<T>
{
    return -(*this);
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::matDiv(const Matrix<U>& other) const
    requires SelfAddAssignable<std::common_type_t<T, U>> && Multipliable<T, U> && SignedNumber<U>
{
    Matrix<T> matrix{other.inverse()};
    return dot(matrix);
}

template <MatrixElement T>
Matrix<T>& Matrix<T>::operator=(const std::initializer_list<std::initializer_list<T>>& elems)
{
    Matrix<T> matrix{elems};
    *this = std::move(matrix);
    return *this;
}

template <MatrixElement T>
template <MatrixElement U>
decltype(auto) Matrix<T>::operator&(const Matrix<U>& other) const
    requires SelfAddAssignable<std::common_type_t<T, U>> && Multipliable<T, U>
{
    return dot(other);
}
