#ifndef OPERATION_RESULT_HPP
#define OPERATION_RESULT_HPP

#include <QString>

#include <utility>

template<typename T>
struct OperationResult
{
    bool ok = false;
    QString message;
    T value{};

    static OperationResult success(T value, QString message = {})
    {
        return {true, std::move(message), std::move(value)};
    }

    static OperationResult failed(QString message = {}) { return {false, std::move(message), T{}}; }

    static OperationResult failed(T value, QString message)
    {
        return {false, std::move(message), std::move(value)};
    }
};

#endif // OPERATION_RESULT_HPP
