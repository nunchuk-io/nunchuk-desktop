// core/viewmodel/BaseViewModel.h
#pragma once
#include "QWarningMessage.h"
#include "core/common/datatypes/NunchukMetaType.hpp"
#include "core/viewmodel/ViewModelContext.h"
#include <QObject>

namespace core::viewmodels {
class BaseViewModel : public QObject {
    Q_OBJECT
  public:
    explicit BaseViewModel(QObject *parent = nullptr);
    void initialize();
  public slots:
    virtual void cancel();
    virtual void back();
    virtual void next();
    virtual void close();
    void closeToWalletTab();
    void closeToChatTab();
    void closeToServiceTab();

  public:
    virtual int backToScreen() {
        return -1;
    }

  protected:
    ViewModelContext *ctx() const;
    virtual void onInit() {}
    void baseConnectSignals();
  signals:
    void showToast(int code, const QString &what, EWARNING::WarningType type);

  private:
    ViewModelContext *m_ctx = nullptr;
    bool m_initialized{false};
};
} // namespace core::viewmodels
#include "QOutlog.h"