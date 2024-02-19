/************************************************************************* 
* This file is part of the Nunchuk software (https://nunchuk.io/)        * 
* Copyright (C) 2020-2022 Enigmo                                         * 
* Copyright (C) 2022 Nunchuk                                             * 
*                                                                        * 
* This program is free software; you can redistribute it and/or          * 
* modify it under the terms of the GNU General Public License            * 
* as published by the Free Software Foundation; either version 3         * 
* of the License, or (at your option) any later version.                 * 
*                                                                        * 
* This program is distributed in the hope that it will be useful,        * 
* but WITHOUT ANY WARRANTY; without even the implied warranty of         * 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          * 
* GNU General Public License for more details.                           * 
*                                                                        * 
* You should have received a copy of the GNU General Public License      * 
* along with this program.  If not, see <http://www.gnu.org/licenses/>.  * 
*                                                                        * 
**************************************************************************/


#include "STATE_ID_SCR_RECURRING_PAYMENTS.h"
#include "AppModel.h"
#include "Premiums/QRecurringPayment.h"
#include "Premiums/QGroupDashboard.h"

void SCR_RECURRING_PAYMENTS_Entry(QVariant msg) {

}

void SCR_RECURRING_PAYMENTS_Exit(QVariant msg) {

}

void EVT_RECURRING_PAYMENTS_ENTER_HANDLER(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    DBG_INFO << type;
    if (auto w = AppModel::instance()->walletInfoPtr()){
        if (auto payment = w->recurringPaymentPtr()) {
            if (type == "request-back") {
                payment->backFlow();
            }
            else if (type == "add-recurring-payment") {
                payment->setPaymentCurrent(payment->DefaultPayment());
                payment->addFlow((int)PaymentEnum::Enum_t::ENTER_NAME_PAYMENT);
            }
            else if (type == "payment-name") {
                QString name = maps["name"].toString();
                payment->EditPayment({{"name", name}});
                payment->addFlow((int)PaymentEnum::Enum_t::ENTER_FIXED_AMOUNT);
            }
            else if (type == "use-percentage") {
                payment->replaceFlow((int)PaymentEnum::Enum_t::ENTER_PERCENTAGE);
            }
            else if (type == "payment-amount") {
                QString payment_type = maps["payment_type"].toString();
                QString currency = maps["currency"].toString();
                double amount = maps["amount"].toDouble();
                QJsonObject payment_payload = payment->paymentCurrentJs()["payment_payload"].toObject();
                payment_payload["value"] = amount;
                payment_payload["currency"] = currency;
                payment->EditPayment({{"payment_type", payment_type},
                                      {"payment_payload", payment_payload}});
                payment->addFlow((int)PaymentEnum::Enum_t::SELECT_DESTINATION_PAYMENT);
            }
            else if (type == "use-fixed-amount") {
                payment->replaceFlow((int)PaymentEnum::Enum_t::ENTER_FIXED_AMOUNT);
            }
            else if (type == "payment-percentage" || type == "select-percentage") {
                QString payment_type = maps["payment_type"].toString();
                QJsonObject payment_payload = payment->paymentCurrentJs()["payment_payload"].toObject();
                if (maps.contains("percent")) {
                    double percent = maps["percent"].toDouble();
                    payment_payload["valuePercent"] = percent;
                }
                if (maps.contains("calculation_method")) {
                    QString calculation_method = maps["calculation_method"].toString();
                    payment_payload["calculation_method"] = calculation_method;
                }
                payment->EditPayment({{"payment_type", payment_type},
                                      {"payment_payload", payment_payload}});
                if (type == "payment-percentage") {
                    payment->addFlow((int)PaymentEnum::Enum_t::SELECT_PERCENTAGE_CALCULATED);
                }
                else if (type == "select-percentage") {
                    payment->addFlow((int)PaymentEnum::Enum_t::SELECT_DESTINATION_PAYMENT);
                }
            }
            else if (type == "select-destination") {
                QString destination_type = maps["destination_type"].toString();
                payment->EditPayment({{"destination_type", destination_type}});
                if (destination_type == "DESTINATION_WALLET") {
                    payment->addFlow((int)PaymentEnum::Enum_t::SELECT_IMPORT_FORMAT);
                } else if (destination_type == "WHITELISTED_ADDRESSES") {
                    payment->addFlow((int)PaymentEnum::Enum_t::ENTER_WHITELISTED_ADDRESS);
                }
            }
            else if (type == "select-import-format") {
                QString file = maps["filePath"].toString();
                QString file_path = qUtils::QGetFilePath(file);
                QString format = maps["format"].toString();
                bool ret {format == "qr_code"};
                if(file_path != ""){
                    if (format == "bsms") {
                        ret = payment->ImportBSMSWallet(file_path);
                    }
                    else if (format == "coldcard-nfc") {
                        ret = payment->ImportColdcardNFCWallet(file_path);
                    }
                }
                else if (format == "existing-wallet") {
                    QString wallet_id = maps["wallet_id"].toString();
                    ret = payment->ImportWallet(wallet_id);
                }
                if (ret) {
                    payment->addFlow((int)PaymentEnum::Enum_t::IMPORT_FIRST_ADDRESS_OF_WALLET);
                }
            }
            else if (type == "import-address-list") {
                QStringList address_list = maps["import_address_list"].toStringList();
                if (payment->ImportAddressesWallet(address_list))
                {
                    payment->addFlow((int)PaymentEnum::Enum_t::ENTER_FREQUENCY_OF_PAYMENT);
                }
            }
            else if (type == "imported-first-address") {
                payment->addFlow((int)PaymentEnum::Enum_t::ENTER_FREQUENCY_OF_PAYMENT);
            }
            else if (type == "setup-frequency-of-payment") {
                QString frequency = maps["frequency"].toString();
                QString start_date = maps["start_date"].toString();
                QString end_date = maps["end_date"].toString();
                payment->EditPayment({{"frequency", frequency},
                                      {"start_date", start_date},
                                      {"end_date", end_date}});
                payment->addFlow((int)PaymentEnum::Enum_t::SELECT_FEE_RATE);
            }
            else if (type == "select-fee-rate") {
                QString fee_rate = maps["fee_rate"].toString();
                payment->EditPayment({{"fee_rate", fee_rate}});
                if (payment->hasServerKey()) {
                    payment->addFlow((int)PaymentEnum::Enum_t::SELECT_PLATFORM_KEY_CO_SIGN);
                }
                else {
                    payment->addFlow((int)PaymentEnum::Enum_t::ENTER_TRANSACTION_NOTE);
                }
            }
            else if (type == "select-allow-platform-key-co-sign") {
                bool allow_cosigning = maps["allow_cosigning"].toBool();
                payment->EditPayment({{"allow_cosigning", allow_cosigning}});
                payment->addFlow((int)PaymentEnum::Enum_t::ENTER_TRANSACTION_NOTE);
            }
            else if (type == "apply-transaction-note") {
                QString transaction_note = maps["transaction_note"].toString();
                payment->EditPayment({{"transaction_note", transaction_note}});
                payment->addFlow((int)PaymentEnum::Enum_t::RECURRING_PAYMENTS_REVIEW);
            }
            else if (type == "create-recurring-payment") {
                if (payment->CreateRecurringPayment()) {
                    if (auto dash = w->dashboard()) {
                        dash->setFlow((int)AlertEnum::E_Alert_t::CREATE_RECURRING_PAYMENT);
                        QQuickViewer::instance()->notifySendEvent(E::EVT_HEALTH_CHECK_STARTING_REQUEST);
                    }
                }
            }
            else if (type == "cancel-recurring-payment") {
                if (payment->CancelRecurringPayment()) {
                    if (auto dash = w->dashboard()) {
                        dash->setFlow((int)AlertEnum::E_Alert_t::CANCEL_RECURRING_PAYMENT);
                        QQuickViewer::instance()->notifySendEvent(E::EVT_HEALTH_CHECK_STARTING_REQUEST);
                    }
                }
            }
            else if (type == "approve-recurring-payment") {
                QQuickViewer::instance()->notifySendEvent(E::EVT_HEALTH_CHECK_STARTING_REQUEST);
            }
            else if (type == "deny-recurring-payment") {
                if (payment->CancelPaymentCancellationPending()) {
                    if (auto dashboard = payment->dashBoardPtr()) {
                        dashboard->GetAlertsInfo();
                    }
                    QQuickViewer::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
                    QString msg_cancel = "Recurring payment denied";
                    AppModel::instance()->showToast(0, msg_cancel, EWARNING::WarningType::SUCCESS_MSG);
                }
            }
            else if (type == "select-recurring-payment") {
                QString recurring_payment_id = maps["recurring_payment_id"].toString();
                payment->setPaymentCurrent(recurring_payment_id);
                payment->addFlow((int)PaymentEnum::Enum_t::VIEWING_RECURRING_PAYMENTS);
            }
            else if (type == "recurring-payment-sort") {
                QString sortOption = maps["sortOption"].toString();
                payment->setSortOption(sortOption);
            }
            else if (type == "recurring-payment-clear-all") {
                payment->setSortOption("newest");
            }
        }
    }
}

