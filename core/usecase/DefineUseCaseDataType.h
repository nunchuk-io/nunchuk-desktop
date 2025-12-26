// core/usecase/DefineUseCaseInput.h
#pragma once
#include "UseCaseInput.h"
#include "UseCaseResult.h"
#include "UseCaseDataTypeMacros.h"

#define DEFINE_USECASE_INPUT(TypeName)                   \
struct TypeName : public core::usecase::UseCaseInput {  \
    FIELDS_INPUT(UC_DECLARE_FIELD)                       \
    QJsonObject toJson() const override {                \
        QJsonObject obj;                                 \
        FIELDS_INPUT(UC_JSON_FIELD)                      \
        return obj;                                      \
    }                                                    \
};

#define DEFINE_USECASE_RESULT(TypeName)                   \
struct TypeName : public core::usecase::UseCaseResult {  \
    FIELDS_RESULT(UC_DECLARE_FIELD)                      \
    QJsonObject toJson() const override {                \
        QJsonObject obj;                                 \
        FIELDS_RESULT(UC_JSON_FIELD)                     \
        return obj;                                      \
    }                                                    \
};
