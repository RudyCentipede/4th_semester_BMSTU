#pragma once

#include "MatrixBase.h"
#include "MatrixIteratorBase.h"
#include "MatrixIterator.h"
#include "ConstMatrixIterator.h"
#include "concept.h"

#include <memory>
#include <initializer_list>

template <MatrixElement T>
class Matrix: public MatrixBase
{

#pragma region friends

    friend MatrixIteratorBase<MatrixIterator<T>, T>;
    friend MatrixIteratorBase<ConstMatrixIterator<T>, T>;
    friend MatrixIterator<T>;
    friend ConstMatrixIterator<T>;

#pragma endregion friends

    class MatrixRow;
public:
#pragma region type_aliases

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = MatrixIterator<T>;
    using const_iterator = ConstMatrixIterator<T>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

#pragma endregion type_aliases

#pragma region constructors

    Matrix() = delete;
    Matrix(size_type rowCount, size_type colCount);

    template <typename Cont>
    Matrix(const Cont& container, size_type rowCount, size_type colCount) requires ContainerOf<Cont, T>;

    Matrix(const_reference value, size_type rowCount, size_type colCount);
    Matrix(const std::initializer_list<std::initializer_list<T>>& elems);

    template <std::input_iterator InIter, std::sentinel_for<InIter> Sentinel>
    Matrix(InIter beg, Sentinel end, size_type rowCount, size_type colCount)
        requires std::convertible_to<typename InIter::value_type, T>;

    Matrix(value_type** matrix, size_type rowCount, size_type colCount);

    template <MatrixElement OtherT>
    explicit Matrix(const Matrix<OtherT>& other) requires std::convertible_to<OtherT, T>;
    explicit Matrix(const Matrix<T>& other);

    Matrix(Matrix<T>&& other) noexcept;
    ~Matrix() noexcept override = default;

#pragma endregion constructors

#pragma region cmp_operators

    auto operator<=>(const Matrix<T>& other) const noexcept requires ThreeWayComparable<T>;
    bool operator==(const Matrix<T>& other) const noexcept;

#pragma endregion cmp_operators

#pragma region assign_operators

    Matrix<T>& operator=(const std::initializer_list<std::initializer_list<T>>& elems);
    template <MatrixElement OtherT>
    Matrix<T>& operator=(const Matrix<OtherT>& other) requires std::convertible_to<OtherT, T>;
    Matrix<T>& operator=(const Matrix<T>& other);
    Matrix<T>& operator=(Matrix<T>&& other) noexcept;

#pragma endregion assign_operators

#pragma region compound_assign_operators

    template <MatrixElement OtherT>
    Matrix<T>& operator+=(const Matrix<OtherT>& other) requires AddAssignable<T, OtherT>;
    template <MatrixElement OtherT>
    Matrix<T>& operator+=(const OtherT& elem) noexcept requires AddAssignable<T, OtherT>;
    template <MatrixElement OtherT>
    Matrix<T>& addAssign(const Matrix<OtherT>& other) requires AddAssignable<T, OtherT>;
    template <MatrixElement OtherT>
    Matrix<T>& addAssign(const OtherT& elem) noexcept requires AddAssignable<T, OtherT>;


    template <MatrixElement OtherT>
    Matrix<T>& operator-=(const Matrix<OtherT>& other) requires SubAssignable<T, OtherT>;
    template <MatrixElement OtherT>
    Matrix<T>& operator-=(const OtherT& elem) noexcept requires SubAssignable<T, OtherT>;
    template <MatrixElement OtherT>
    Matrix<T>& subAssign(const Matrix<OtherT>& other) requires SubAssignable<T, OtherT>;
    template <MatrixElement OtherT>
    Matrix<T>& subAssign(const OtherT& elem) noexcept requires SubAssignable<T, OtherT>;


    template <MatrixElement OtherT>
    Matrix<T>& operator/=(const Matrix<OtherT>& other) requires DivAssignable<T, OtherT>;
    template <MatrixElement OtherT>
    Matrix<T>& operator/=(const OtherT& elem) noexcept requires DivAssignable<T, OtherT>;
    template <MatrixElement OtherT>
    Matrix<T>& divAssign(const Matrix<OtherT>& other) requires DivAssignable<T, OtherT>;
    template <MatrixElement OtherT>
    Matrix<T>& divAssign(const OtherT& elem) noexcept requires DivAssignable<T, OtherT>;


    template <MatrixElement OtherT>
    Matrix<T>& operator*=(const Matrix<OtherT>& other) requires MulAssignable<T, OtherT>;
    template <MatrixElement OtherT>
    Matrix<T>& operator*=(const OtherT& elem) noexcept requires MulAssignable<T, OtherT>;
    template <MatrixElement OtherT>
    Matrix<T>& multAssign(const Matrix<OtherT>& other) requires MulAssignable<T, OtherT>;
    template <MatrixElement OtherT>
    Matrix<T>& multAssign(const OtherT& elem) noexcept requires MulAssignable<T, OtherT>;

#pragma endregion compound_assign_operators

#pragma region arithmetic_operators

    template <MatrixElement OtherT>
    decltype(auto) operator+(const Matrix<OtherT>& other) const requires Addable<T, OtherT>;
    template <MatrixElement OtherT>
    decltype(auto) operator+(const OtherT& elem) const requires Addable<T, OtherT>;
    template <MatrixElement OtherT>
    decltype(auto) add(const Matrix<OtherT>& other) const requires Addable<T, OtherT>;
    template <MatrixElement OtherT>
    decltype(auto) add(const OtherT& elem) const requires Addable<T, OtherT>;


    template <MatrixElement OtherT>
    decltype(auto) operator-(const Matrix<OtherT>& other) const requires Subtractable<T, OtherT>;
    template <MatrixElement OtherT>
    decltype(auto) operator-(const OtherT& elem) const requires Subtractable<T, OtherT>;
    template <MatrixElement OtherT>
    decltype(auto) sub(const Matrix<OtherT>& other) const requires Subtractable<T, OtherT>;
    template <MatrixElement OtherT>
    decltype(auto) sub(const OtherT& elem) const requires Subtractable<T, OtherT>;


    template <MatrixElement OtherT>
    decltype(auto) operator*(const Matrix<OtherT>& other) const requires Multipliable<T, OtherT>;
    template <MatrixElement OtherT>
    decltype(auto) operator*(const OtherT& elem) const requires Multipliable<T, OtherT>;
    template <MatrixElement OtherT>
    decltype(auto) mult(const Matrix<OtherT>& other) const requires Multipliable<T, OtherT>;
    template <MatrixElement OtherT>
    decltype(auto) mult(const OtherT& elem) const requires Multipliable<T, OtherT>;


    template <MatrixElement OtherT>
    decltype(auto) operator/(const Matrix<OtherT>& other) const requires Divisible<T, OtherT>;
    template <MatrixElement OtherT>
    decltype(auto) operator/(const OtherT& elem) const requires Divisible<T, OtherT>;
    template <MatrixElement OtherT>
    decltype(auto) div(const Matrix<OtherT>& other) const requires Divisible<T, OtherT>;
    template <MatrixElement OtherT>
    decltype(auto) div(const OtherT& elem) const requires Divisible<T, OtherT>;


    Matrix<T> operator-() const noexcept requires Signed<T>;
    Matrix<T> neg() const noexcept requires Signed<T>;

#pragma endregion arithmetic_operators

#pragma region element_access_operators

    MatrixRow& operator[](size_type idx);
    const MatrixRow& operator[](size_type idx) const;
    reference operator()(size_type rowIdx, size_type colIdx);
    const_reference operator()(size_type rowIdx, size_type colIdx) const;

#pragma endregion element_access_operators

#pragma region iterators

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

#pragma endregion iterators

#pragma region resize_methods

    void removeRow(size_type idx);
    void removeCol(size_type idx);
    void insertRow(size_type idx, const_reference fillElem = T());
    void insertCol(size_type idx, const_reference fillElem = T());
    void insertRow(size_type idx, const std::initializer_list<T>& elems);
    void insertCol(size_type idx, const std::initializer_list<T>& elems);
    void reshape(size_type rowCount, size_type colCount);
    void resize(size_type rowCount, size_type colCount, const_reference fillElem = T());

#pragma endregion resize_methods

#pragma region math_methods

    bool isZero() const noexcept;
    bool isIdentity() const requires SignedNumber<T>;
    Matrix<T> additionalMinor(size_type rowIdx, size_type colIdx) const requires SignedNumber<T>;
    value_type algebraicExtension(size_type rowIdx, size_type colIdx) const requires SignedNumber<T>;
    Matrix<T> inverse() const requires SignedNumber<T>;
    value_type determinant() const requires SignedNumber<T>;

    template <MatrixElement OtherT>
    decltype(auto) operator&(const Matrix<OtherT>& other) const
        requires SelfAddAssignable<std::common_type_t<T, OtherT>> && Multipliable<T, OtherT>;
    template <MatrixElement OtherT>
    decltype(auto) dot(const Matrix<OtherT>& other) const
        requires SelfAddAssignable<std::common_type_t<T, OtherT>> && Multipliable<T, OtherT>;

    template <MatrixElement OtherT>
    decltype(auto) matDiv(const Matrix<OtherT>& other) const
        requires SelfAddAssignable<std::common_type_t<T, OtherT>> && Multipliable<T, OtherT> && SignedNumber<OtherT>;

    bool isSquare() const noexcept;
    void transpose();
    Matrix<T> transposed() const;
    static Matrix<T> identity(size_type size) requires SignedNumber<T>;

#pragma endregion math_methods

#pragma region common_methods

    bool isValidRowIndex(size_type idx) const noexcept;
    bool isValidColIndex(size_type idx) const noexcept;
    bool isValidIndex(size_type rowIdx, size_type colIdx) const noexcept;
    bool empty() const noexcept;
    size_type size() const noexcept;
    void fill(const_reference value) noexcept;

#pragma endregion common_methods

private:
    std::shared_ptr<MatrixRow[]> _rows;

#pragma region auxiliary_methods

    template <MatrixElement OtherT>
    void copyFrom(const Matrix<OtherT>& other) requires std::convertible_to<OtherT, T>;
    void init(const std::initializer_list<std::initializer_list<T>>& data);
    void allocateMemory(size_type rowCount, size_type colCount);
    void assertRowIndexValidity(const std::source_location& info, size_type index) const;
    void assertColIndexValidity(const std::source_location& info, size_type index) const;
    void assertIndexValidity(const std::source_location& info, size_type rowIdx, size_type colIdx) const;
    void assertRowInsertability(const std::source_location& info, size_type rowIdx) const;
    void assertColInsertability(const std::source_location& info, size_type colIdx) const;
    void assertRowInsertability(const std::source_location& info, size_type rowIdx, const std::initializer_list<T>& elems) const;
    void assertColInsertability(const std::source_location& info, size_type colIdx, const std::initializer_list<T>& elems) const;
    template <MatrixElement OtherT>
    void assertSizeEquality(const std::source_location& info, const Matrix<OtherT>& other) const requires std::convertible_to<OtherT, T>;

#pragma endregion auxiliary_methods

#pragma region row_class

    class MatrixRow
    {
        friend Matrix;
    public:
        MatrixRow() noexcept;
        MatrixRow(value_type* elems, size_type len) noexcept;
        explicit MatrixRow(const MatrixRow& other);
        MatrixRow(MatrixRow&& other) noexcept;
        ~MatrixRow() noexcept = default;

        MatrixRow& operator=(const std::initializer_list<T>& elems);
        MatrixRow& operator=(const MatrixRow& other);
        MatrixRow& operator=(MatrixRow&& other) noexcept;

        reference operator[](size_type idx);
        const_reference operator[](size_type idx) const;

        void fill(const_reference value) noexcept;

    private:
        void allocateMemory(size_type len);

        size_type _len;
        std::shared_ptr<T[]> _elems;
    };

#pragma endregion row_class
};

template <MatrixElement T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) noexcept requires Printable<T>;

template <MatrixElement T, MatrixElement OtherT>
decltype(auto) operator+(const T& elem, const Matrix<OtherT>& matrix) requires Addable<OtherT, T>;

template <MatrixElement T, MatrixElement OtherT>
decltype(auto) operator*(const T& elem, const Matrix<OtherT>& matrix) requires Multipliable<OtherT, T>;

#include "Matrix.hpp"


