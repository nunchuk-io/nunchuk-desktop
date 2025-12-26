#pragma once
#include <QVariant>

namespace helper::transaction {

// Sign a transaction with QVariant input
void signRequest(const QVariant& msg);

// Import a transaction from hex or JSON using QVariant
void importFile(const QVariant& msg);

// Export a transaction to hex or JSON using QVariant
void exportFile(const QVariant& msg);

// Broadcast a transaction to the network using QVariant
void broadcast(const QVariant& msg);

// Replace-By-Fee (RBF) using QVariant
void replaceByFee(const QVariant& msg);

// Set or update a memo/label for a transaction using QVariant
void setMemo(const QVariant& msg);

// Scan a device using QVariant
void scanDevice(const QVariant& msg);

// Remove a transaction using QVariant
void remove(const QVariant& msg);

// Export a transaction as QR content using QVariant
void exportQr(const QVariant& msg);

// Import a transaction from QR content using QVariant
void importQr(const QVariant& msg);

// Verify an address using QVariant
void verifyAddress(const QVariant& msg);

// Cancel a pending request using QVariant
void cancelRequest(const QVariant& msg);

} // namespace helper::transaction