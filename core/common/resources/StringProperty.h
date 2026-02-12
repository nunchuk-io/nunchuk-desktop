#pragma once

#define DEFINE_STRING_PROPERTY(name, value) \
    Q_PROPERTY(QString name READ name CONSTANT) \
public: \
    QString name() const { return tr(value); }
