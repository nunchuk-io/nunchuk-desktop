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
#include <QQmlEngine>
#include <QQuickItemGrabResult>
#include <QtConcurrent/qtconcurrentrun.h>
#include "QPDFPrinter.h"
#include "QtGui/qpainter.h"
#include "QOutlog.h"
#include "qUtils.h"

#if defined (Q_OS_WIN)
#include <windows.h>
#endif

QPDFPrinter::QPDFPrinter(QObject *parent) : QObject{parent},
    m_printer{new QPrinter(QPrinter::HighResolution)}
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    m_printer->setOutputFormat(QPrinter::PdfFormat);
    m_printer->setPageSize(QPageSize(QPageSize::A4));
}

QPDFPrinter *QPDFPrinter::instance()
{
    static QPDFPrinter mInstance;
    return &mInstance;
}

void QPDFPrinter::printQRCodeToPdf(const QString &pdfPath, QVariant objects)
{
    QList<QVariant> list = objects.toList();
    QList<QImage> images;
    images.clear();

    int index = 0;
    int total = list.count();
    for (auto item : list)
    {
        emit progressChanged(index, total);
        QObject *obj = item.value<QObject *>();
        if (obj)
        {
            QQuickItem *grabItem = qobject_cast<QQuickItem *>(obj);
            if (grabItem)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Recommended for component completed
                QSize targetSize = QSize(grabItem->width()*2, grabItem->height()*2);
                auto grabResult = grabItem->grabToImage(targetSize);
                QEventLoop pdfLoop;
                QObject::connect(grabResult.data(),   &QQuickItemGrabResult::ready,   &pdfLoop, &QEventLoop::quit);
                pdfLoop.exec();
                QImage image = grabResult->image();
                images.append(image);
                DBG_INFO << image.width() << " " << image.height() << images.count();
            }
        }
    }
    printImagesToPdf(pdfPath, images);
    emit progressChanged(total, total);
    emit finished();
}

void QPDFPrinter::printInvoiceToPdf(const QString &pdfPath, QVariant object)
{
    QObject *obj = object.value<QObject *>();
    if (obj)
    {
        QQuickItem *grabItem = qobject_cast<QQuickItem *>(obj);
        if (grabItem)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Recommended for component completed
            QSize targetSize = QSize(grabItem->width()*2, grabItem->height()*2);
            auto grabResult = grabItem->grabToImage(targetSize);
            QEventLoop pdfLoop;
            QObject::connect(grabResult.data(),   &QQuickItemGrabResult::ready,   &pdfLoop, &QEventLoop::quit);
            pdfLoop.exec();
            QImage image = grabResult->image();
            printImageToPdf(pdfPath, image);
        }
    }
}

void QPDFPrinter::printInvoicesToPdf(const QString &pdfPath, QList<QVariant> objects)
{
    QtConcurrent::run([=, this]() {
        QList<QImage> images;
        images.clear();
        int index = 0;
        int total = objects.count();
        for (auto item : objects)
        {
            emit progressChanged(index, total);
            QObject *obj = item.value<QObject *>();
            if (obj)
            {
                QQuickItem *grabItem = qobject_cast<QQuickItem *>(obj);
                if (grabItem)
                {
#if defined (Q_OS_WIN)
                    if(index == 0){
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Recommended for component completed
                    }
#else
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Recommended for component completed
#endif
                    QSize targetSize = QSize(grabItem->width()*2, grabItem->height()*2);
                    auto grabResult = grabItem->grabToImage(targetSize);
                    QEventLoop pdfLoop;
                    QObject::connect(grabResult.data(),   &QQuickItemGrabResult::ready,   &pdfLoop, &QEventLoop::quit);
                    pdfLoop.exec();
                    QImage image = grabResult->image();
                    images.append(image);
                    DBG_INFO << image.width() << " " << image.height() << images.count();
                }
            }
            index++;
        }
        printImagesToPdf(pdfPath, images);
        emit progressChanged(total, total);
        emit finished();
    });
}

void QPDFPrinter::printImageToPdf(const QString &pdfPath, QImage image)
{
    if(m_printer){
        QString file_path = qUtils::QGetFilePath(pdfPath);
        m_printer->setOutputFileName(file_path);
        QPainter painter;
        if (!painter.begin(m_printer)) {
            DBG_INFO << "Failed to open file, is it writable?";
        }
        else {
            QRect printableArea = m_printer->pageLayout().paintRectPixels(m_printer->resolution());

            QSize imageSize = image.size();
            imageSize.scale(printableArea.size(), Qt::KeepAspectRatio);

            // Center the image on the page
            int xOffset = (printableArea.width() - imageSize.width()) / 2;
            int yOffset = (printableArea.height() - imageSize.height()) / 2;
            QRect centeredRect(xOffset, yOffset, imageSize.width(), imageSize.height());

            // Draw the image
            painter.drawImage(centeredRect, image);
        }
        painter.end();
    }
}

void QPDFPrinter::printImagesToPdf(const QString &pdfPath, const QList<QImage> &images)
{
    if (m_printer) {
        QString file_path = qUtils::QGetFilePath(pdfPath);
        m_printer->setOutputFileName(file_path);
        QPainter painter;
        if (!painter.begin(m_printer)) {
            DBG_INFO << "Failed to open file, is it writable?";
            return;
        }
        QRect printableArea = m_printer->pageLayout().paintRectPixels(m_printer->resolution());

        // Iterate through each image in the list
        for (int i = 0; i < images.size(); ++i) {
            QImage image = images[i];
            QSize imageSize = image.size();
            imageSize.scale(printableArea.size(), Qt::KeepAspectRatio);
            // Center the image on the page
            int xOffset = (printableArea.width() - imageSize.width()) / 2;
            int yOffset = (printableArea.height() - imageSize.height()) / 2;
            QRect centeredRect(xOffset, yOffset, imageSize.width(), imageSize.height());
            // Draw the image
            painter.drawImage(centeredRect, image);
            // If not the last image, we need to start a new page
            if (i < images.size() - 1) {
                m_printer->newPage();
            }
        }
        painter.end();
    }
}

