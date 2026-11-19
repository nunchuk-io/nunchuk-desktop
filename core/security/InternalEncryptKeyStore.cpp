/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
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

#include "InternalEncryptKeyStore.h"

#include <QByteArray>
#include <QEventLoop>
#include <QRandomGenerator>
#include <cstring>
#ifdef USE_KEYCHAIN
#include <qt6keychain/keychain.h>
#endif
#include "QOutlog.h"

namespace {
const char KEYCHAIN_SERVICE[] = "NunchukInternalEncryptKey";
const char KEYCHAIN_KEY[] = "internal_encrypt_key";
}  // namespace

namespace core::security {

bool InternalEncryptKeyStore::getOrCreateKey(std::vector<uint8_t> &key)
{
    key.clear();
#ifndef USE_KEYCHAIN
    DBG_ERROR << "Keychain support is not built into this binary, cannot store the internal encrypt key";
    return false;
#else
    QKeychain::ReadPasswordJob readJob{QLatin1String(KEYCHAIN_SERVICE)};
    readJob.setAutoDelete(false);
    readJob.setKey(QLatin1String(KEYCHAIN_KEY));
    QEventLoop readLoop;
    QKeychain::ReadPasswordJob::connect(&readJob, &QKeychain::Job::finished, &readLoop, &QEventLoop::quit);
    readJob.start();
    readLoop.exec();

    if (readJob.error() == QKeychain::Error::NoError) {
        const QByteArray existing = readJob.binaryData();
        if (existing.size() >= KEY_SIZE) {
            key.assign(existing.begin(), existing.end());
            return true;
        }
        DBG_ERROR << "Internal encrypt key found in the keychain is shorter than" << KEY_SIZE << "bytes";
        return false;
    }
    if (readJob.error() != QKeychain::Error::EntryNotFound) {
        DBG_ERROR << "Could not read the internal encrypt key from the keychain:" << readJob.errorString();
        return false;
    }

    QByteArray generated(KEY_SIZE, Qt::Uninitialized);
    for (int i = 0; i < KEY_SIZE; i += 4) {
        const quint32 word = QRandomGenerator::system()->generate();
        generated[i]     = static_cast<char>(word & 0xFF);
        generated[i + 1] = static_cast<char>((word >> 8) & 0xFF);
        generated[i + 2] = static_cast<char>((word >> 16) & 0xFF);
        generated[i + 3] = static_cast<char>((word >> 24) & 0xFF);
    }

    QKeychain::WritePasswordJob writeJob{QLatin1String(KEYCHAIN_SERVICE)};
    writeJob.setAutoDelete(false);
    writeJob.setKey(QLatin1String(KEYCHAIN_KEY));
    writeJob.setBinaryData(generated);
    QEventLoop writeLoop;
    QKeychain::WritePasswordJob::connect(&writeJob, &QKeychain::Job::finished, &writeLoop, &QEventLoop::quit);
    writeJob.start();
    writeLoop.exec();

    if (writeJob.error() != QKeychain::Error::NoError) {
        DBG_ERROR << "Could not save the newly generated internal encrypt key to the keychain:" << writeJob.errorString();
        generated.fill(0);
        return false;
    }

    key.assign(generated.begin(), generated.end());
    generated.fill(0);
    return true;
#endif
}

void InternalEncryptKeyStore::wipe(std::vector<uint8_t> &key)
{
    if (!key.empty()) {
        std::memset(key.data(), 0, key.size());
    }
    key.clear();
}

}  // namespace core::security
