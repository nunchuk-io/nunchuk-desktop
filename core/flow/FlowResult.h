// core/flow/FlowEvent.h
#pragma once
#include <QVariant>

namespace core::flow {
enum class FlowResult {
    Next,
    Back,
    Complete,
    Fail,
    Custom
};
} // namespace core::flow
