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
#ifndef QPDFPRINTER_H
#define QPDFPRINTER_H

#include <QObject>
#include <QPrinter>
#include <QQuickItem>
#include <QImage>
#include <QPainter>


class QPDFPrinter : public QObject
{
    Q_OBJECT
public:
    explicit QPDFPrinter(QObject *parent = nullptr);
    // Singleton instance
    static QPDFPrinter *instance();
    QPDFPrinter(QPDFPrinter &other) = delete;
    QPDFPrinter(QPDFPrinter const &other) = delete;
    void operator=(const QPDFPrinter &other) = delete;

    // using printer to print pdf and expose qml
    Q_INVOKABLE void printQRCodeToPdf(const QString &pdfPath, QVariant objects);
    Q_INVOKABLE void printInvoiceToPdf(const QString &pdfPath, QVariant object);    // For one invoice

    // invoices
    void printInvoicesToPdf(const QString &pdfPath, QList<QVariant> objects);   // For multiple invoices

signals:
    void progressChanged(int index, int total);
    void finished();

private:
    void printImageToPdf(const QString &pdfPath, QImage image);
    void printImagesToPdf(const QString &pdfPath, const QList<QImage> &images);


private:
    QPrinter *m_printer;
};

#endif // QPDFPRINTER_H
