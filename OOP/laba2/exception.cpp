#include "exception.h"
#include <ctime>
#include <cstdio>

MatrixException::MatrixException(const std::source_location& info, const char* data) noexcept
{
    auto curTime = time(nullptr);
    snprintf(_msg, msgLen, "\nFile: %s\nFunction: %s\nLine: %d\nColumn: %d\nTime: %s\n%s",
             info.file_name(), info.function_name(), info.line(), info.column(), ctime(&curTime), data);
}

const char* MatrixException::what() const noexcept
{
    return _msg;
}
