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

#include "QLoadingOverlay.h"
#if ENABLE_WEBVIEW_SIGIN
#include "QOutlog.h"

QLoadingOverlay::QLoadingOverlay(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("background-color: rgba(89, 89, 89, 0);"); // nền xám nhẹ
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    QLabel* spinnerLabel = new QLabel(this);
    spinnerLabel->setFixedSize(90, 90);
    QMovie* spinnerMovie = new QMovie(":/Images/Images/spinner.gif");
    if (!spinnerMovie->isValid()) {
        DBG_INFO << "Không thể load GIF spinner!";
    }
    spinnerMovie->setScaledSize(QSize(90, 90));
    spinnerLabel->setMovie(spinnerMovie);
    spinnerMovie->start();

    layout->addWidget(spinnerLabel);
    setLayout(layout);
    hide();
}
#endif
