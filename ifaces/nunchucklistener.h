#ifndef NUNCHUCKLISTENER_H
#define NUNCHUCKLISTENER_H

#ifndef USING_STUB_API
#include <nunchuk.h>

void balance_listener(std::string id, nunchuk::Amount value);
void devices_listener(std::string fingerprint, bool connected);
void transaction_listener(std::string tx_id, nunchuk::TransactionStatus status);
void block_listener(int height, std::string hex_header);
bool cache_master_signer_XPub(int progress);
bool create_master_signer_listener(int progress);
void blockchain_connection_listener(nunchuk::ConnectionStatus status, int percent);
bool create_software_signer_listener(int progress);

#endif // USING_STUB_API

#endif // NUNCHUCKLISTENER_H
