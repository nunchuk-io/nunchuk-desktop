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

#ifndef MATRIXLISTENER_H
#define MATRIXLISTENER_H

#include "matrixbrigde.h"
// event id
std::string SendFunction(const string &roomid,
                         const std::string& eventtype,
                         const std::string& eventContent,
                         bool ignore_error);

bool consume_sync_event_listener(int percent);

Q_DECLARE_METATYPE(const char*);
std::string UploadFileFunc (const std::string& file_name,
                            const std::string&  mine_type,
                            const std::string& json_info ,
                            const char* data ,
                            size_t data_length);

std::vector<unsigned char>  DownloadFileFunc  (const std::string& file_name ,
                                               const std::string& mine_type,
                                               const std::string& json_info ,
                                               const std::string& mxc_uri);

bool DownloadFileProgress(int percent);

#endif // MATRIXLISTENER_H
