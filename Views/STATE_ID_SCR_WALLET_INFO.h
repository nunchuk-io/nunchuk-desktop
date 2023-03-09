
#ifndef STATE_ID_SCR_WALLET_INFO_H
#define STATE_ID_SCR_WALLET_INFO_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_WALLET_INFO_Entry(QVariant msg);
void SCR_WALLET_INFO_Exit(QVariant msg);
void EVT_WALLET_INFO_EDIT_NAME_HANDLER(QVariant msg);
void EVT_WALLET_INFO_REMOVE_HANDLER(QVariant msg);
void EVT_WALLET_INFO_EXPORT_DB_HANDLER(QVariant msg);
void EVT_WALLET_INFO_EXPORT_CSV_HANDLER(QVariant msg);
void EVT_WALLET_INFO_EXPORT_DESCRIPTOR_HANDLER(QVariant msg);
void EVT_WALLET_INFO_EXPORT_COLDCARD_HANDLER(QVariant msg);
void EVT_WALLET_INFO_EXPORT_QRCODE_HANDLER(QVariant msg);
void EVT_WALLET_INFO_SIGNER_INFO_REQUEST_HANDLER(QVariant msg);
void EVT_WALLET_INFO_EDIT_DESCRIPTION_HANDLER(QVariant msg);
void EVT_WALLET_INFO_IMPORT_PSBT_HANDLER(QVariant msg);
void EVT_WALLET_INFO_REFRESH_WALLET_REQUEST_HANDLER(QVariant msg);
void EVT_WALLET_INFO_BACK_REQUEST_HANDLER(QVariant msg);
void EVT_WALLET_INFO_UTXOS_REQUEST_HANDLER(QVariant msg);
void EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST_HANDLER(QVariant msg);
void EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST_HANDLER(QVariant msg);
void EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_WALLET_INFO_H
