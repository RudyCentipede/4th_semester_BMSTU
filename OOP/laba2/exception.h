#pragma once

#include <exception>
#include <source_location>

constexpr size_t msgLen{512};

class MatrixException: public std::exception
{
public:
    MatrixException(const std::source_location& info, const char* data = "") noexcept;
    virtual ~MatrixException() noexcept = default;
    virtual const char *what() const noexcept override;

protected:
    char _msg[msgLen];
};

class MatrixIteratorException: public MatrixException
{
public:
    using MatrixException::MatrixException;
};

class MatrixIteratorRangeError: public MatrixIteratorException
{
public:
    using MatrixIteratorException::MatrixIteratorException;
};

class MatrixIteratorExpiredDataError: public MatrixIteratorException
{
public:
    using MatrixIteratorException::MatrixIteratorException;
};

class MatrixErrorArgument: public MatrixException
{
public:
    using MatrixException::MatrixException;
};

class MatrixErrorRange: public MatrixException
{
public:
    using MatrixException::MatrixException;
};

class MatrixErrorNullptr: public MatrixException
{
public:
    using MatrixException::MatrixException;
};


