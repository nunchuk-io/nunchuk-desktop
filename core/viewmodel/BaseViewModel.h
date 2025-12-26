// core/viewmodel/BaseViewModel.h
#pragma once
#include <QObject>
#include "core/viewmodel/ViewModelContext.h"
#include "QWarningMessage.h"

namespace core::viewmodels {
class BaseViewModel : public QObject {
    Q_OBJECT
  public:
    explicit BaseViewModel(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~BaseViewModel() {
        if (m_initialized) {
            onDispose();
        }
    }

  public slots:
    virtual void cancel();
    virtual void back();
    virtual void next();
    virtual void close();

    void attachContext(ViewModelContext *ctx);

    void notifyAppear() {
        Q_ASSERT(m_initialized);
        onAppear();
    }

  protected:
    ViewModelContext *ctx() const {
        Q_ASSERT(m_ctx);
        return m_ctx;
    }
    virtual void onInit() {}
    virtual void onAppear() {}
    virtual void onDispose() {}
    void baseConnectSignals();
  signals:
    void showToast(int code, const QString &what, EWARNING::WarningType type);
  private:
    ViewModelContext *m_ctx = nullptr;
    bool m_initialized{false};
};
} // namespace core::viewmodels
