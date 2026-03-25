#pragma once
#include <QObject>
#include <QString>

#define DEFINE_QT_PROPERTY(Type, Name)                                          \
private:                                                                        \
    Type m_##Name;                                                              \
    Q_PROPERTY(Type Name READ Name WRITE set##Name NOTIFY Name##Changed)        \
public:                                                                         \
    Type Name() const { return m_##Name; }                                      \
    void set##Name(const Type& value) {                                         \
        if (m_##Name == value) return;                                          \
        m_##Name = value;                                                       \
        emit Name##Changed();                                                   \
    }                                                                           \
Q_SIGNALS:                                                                      \
    void Name##Changed();


#define DEFINE_SET_GET(Type, Name)                                              \
private:                                                                        \
    Type m_##Name;                                                              \
public:                                                                         \
    Type Name() const { return m_##Name; }                                      \
    void set##Name(const Type& value) {                                         \
        m_##Name = value;                                                       \
    }


#define DEFINE_QT_PROPERTY_PTR(Type, Name)                                      \
private:                                                                        \
    Q##Type##Ptr m_##Name;                                                      \
    Q_PROPERTY(Type* Name READ Name NOTIFY Name##Changed)                       \
public:                                                                         \
    Type* Name() const { return m_##Name.data(); }                              \
    void set##Name(const Q##Type##Ptr& value) {                                 \
        if (m_##Name == value) return;                                          \
        m_##Name = value;                                                       \
        emit Name##Changed();                                                   \
    }                                                                           \
Q_SIGNALS:                                                                      \
    void Name##Changed();