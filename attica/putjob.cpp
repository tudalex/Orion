/*
    This file is part of KDE.

    Copyright (c) 2011 Laszlo Papp <djszapi@archlinux.us>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "putjob.h"

#include <QXmlStreamReader>
#include <QDebug>

#include <QtNetwork/QNetworkAccessManager>

#include "platformdependent_v2.h"


using namespace Attica;


PutJob::PutJob(PlatformDependent* internals, const QNetworkRequest& request, QIODevice* iodevice)
    : BaseJob(internals), m_ioDevice(iodevice), m_request(request)
{
}

Attica::PutJob::PutJob(PlatformDependent* internals, const QNetworkRequest& request, const QByteArray& byteArray)
    : BaseJob(internals), m_ioDevice(0) , m_byteArray(byteArray), m_request(request)
{
}

PutJob::PutJob(PlatformDependent* internals, const QNetworkRequest& request, const StringMap& parameters)
    : BaseJob(internals), m_ioDevice(0), m_request(request)
{
    // Create put data
    int j = 0;
    for(StringMap::const_iterator i = parameters.begin(); i != parameters.end(); ++i) {
        if (j++ > 0) {
            m_byteArray.append('&');
        }
        m_byteArray.append(QUrl::toPercentEncoding(i.key()));
        m_byteArray.append('=');
        m_byteArray.append(QUrl::toPercentEncoding(i.value()));
    }
}

QNetworkReply* PutJob::executeRequest()
{
    Attica::PlatformDependentV2 *platformDependentV2 = dynamic_cast<Attica::PlatformDependentV2*>(internals());
    if (!platformDependentV2)
        return 0;

    if (m_ioDevice) {
        return platformDependentV2->put(m_request, m_ioDevice);
    } else {
        return platformDependentV2->put(m_request, m_byteArray);
    }
}


void PutJob::parse(const QString& xmlString)
{
    qDebug() << "PutJob::parse" << xmlString;
    QXmlStreamReader xml( xmlString );
    Metadata data;
    while (!xml.atEnd()) {
        xml.readNext();

        if (xml.isStartElement()) {
            if (xml.name() == "meta") {
                while ( !xml.atEnd() ) {
                    xml.readNext();
                    if (xml.isEndElement() && xml.name() == "meta") {
                        break;
                    } else if (xml.isStartElement()) {
                        if (xml.name() == "status") {
                            data.setStatusString(xml.readElementText());
                        } else if (xml.name() == "statuscode") {
                            data.setStatusCode(xml.readElementText().toInt());
                        } else if (xml.name() == "message") {
                            data.setMessage(xml.readElementText());
                        } else if (xml.name() == "totalitems") {
                            data.setTotalItems(xml.readElementText().toInt());
                        } else if (xml.name() == "itemsperpage") {
                            data.setItemsPerPage(xml.readElementText().toInt());
                        }
                    }
                }
            } else if (xml.name() == "data") {
                while ( !xml.atEnd() ) {
                    xml.readNext();
                    if (xml.isEndElement() && xml.name() == "data") {
                        break;
                    } else if (xml.isStartElement()) {
                        if (xml.name() == "projectid") {
                            data.setResultingId(xml.readElementText());
                        } if (xml.name() == "buildjobid") {
                            data.setResultingId(xml.readElementText());
                        }

                    }
                }
            }
        }
    }
    setMetadata(data);
}


#include "putjob.moc"
