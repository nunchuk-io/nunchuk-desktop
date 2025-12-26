#pragma once
#include <QString>

namespace core::usecase {
template <typename T> class Result {
  public:
    Result() = default;

    static Result success(T data) {
        return Result(true, std::move(data), {});
    }

    static Result failure(QString error) {
        return Result(false, {}, std::move(error));
    }

    bool isSuccess() const {
        return m_ok;
    }
    bool isFailure() const {
        return !m_ok;
    }

    const T &value() const {
        return m_value;
    }
    const QString &error() const {
        return m_error;
    }

    QString toLogString() const;

  private:
    Result(bool ok, T value, QString error) : m_ok(ok), m_value(std::move(value)), m_error(std::move(error)) {}

    bool m_ok{false};
    T m_value{};
    QString m_error;
};
} // namespace core::usecase

#include "Result.inl"