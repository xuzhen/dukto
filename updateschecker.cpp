/* DUKTO - A simple, fast and multi-platform file transfer tool for LAN users
 * Copyright (C) 2011 Emanuele Colombo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef UPDATER
#include "updateschecker.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>

#include "version.h"

UpdatesChecker::UpdatesChecker(QObject *parent) : QObject(parent)
{
}

void UpdatesChecker::check()
{
    // Send check request
    QNetworkRequest request(QUrl("https://api.github.com/repos/xuzhen/dukto/releases/latest"));
    request.setRawHeader("Accept", "application/vnd.github+json");
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::SameOriginRedirectPolicy);
#elif QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
#endif
    QNetworkAccessManager *nam = new QNetworkAccessManager();
    connect(nam, &QNetworkAccessManager::finished, this, &UpdatesChecker::updatedDataReady);
    connect(this, &UpdatesChecker::checkEnd, nam, &QNetworkAccessManager::deleteLater);
    nam->get(request);
}

// Read release results
void UpdatesChecker::updatedDataReady(QNetworkReply *reply)
{
    bool success = false;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonParseError error;
        QJsonDocument release = QJsonDocument::fromJson(data, &error);
        if (error.error == QJsonParseError::NoError && release.isObject()) {
            QString tag = release.object().value("tag_name").toString();
            if (!tag.isEmpty()) {
                const QStringList list = tag.remove(QChar('v')).split(QChar('.'));
                QList<uint> ver;
                for (const QString &s : list) {
                    bool ok;
                    ver.append(s.toUInt(&ok));
                    if (!ok) {
                        break;
                    }
                }
                while (ver.size() < 3) {
                    ver.append(0);
                }
                if (ver[0] > VERSION_MAJOR || (ver[0] == VERSION_MAJOR && (ver[1] > VERSION_MINOR || (ver[1] == VERSION_MINOR && ver[2] > VERSION_PATCH)))) {
                    emit updatesAvailable(tag);
                }
                success = true;
            }
        }
    }
    emit checkEnd(success);
    reply->manager()->deleteLater();
}
#endif
