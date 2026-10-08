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

import QtQuick 2.4
import NUNCHUCKTYPE 1.0

QtObject {
    function formatAmount(value) {
        if (value === undefined || value === null)
            return ""

        // 1. Convert sang string
        var str = String(value)

        // 2. Nếu đã đúng format rồi → return luôn (IDEMPOTENT)
        if (/^-?\d{1,3}(,\d{3})*$/.test(str))
            return str

        // 3. Remove TẤT CẢ ký tự không phải số và dấu -
        var clean = str.replace(/[^0-9-]/g, "")
        if (clean === "" || clean === "-")
            return ""

        // 4. Handle dấu âm
        var negative = clean[0] === "-"
        if (negative)
            clean = clean.slice(1)

        // 5. Remove leading zeros
        clean = clean.replace(/^0+(?=\d)/, "")

        // 6. Format từ PHẢI sang TRÁI (cách an toàn nhất)
        var result = ""
        for (var i = clean.length - 1, c = 0; i >= 0; i--, c++) {
            if (c > 0 && c % 3 === 0)
                result = "," + result
            result = clean[i] + result
        }

        return negative ? "-" + result : result
    }
}