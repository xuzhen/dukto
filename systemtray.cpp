/* DUKTO - A simple, fast and multi-platform file transfer tool for LAN users
 * Copyright (C) 2011 Emanuele Colombo
 * Copyright (C) 2015 Arthur Zamarin
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

#include <QtGlobal>
#include "systemtray.h"
#include <QMenu>
#include <QAction>
#include <QApplication>

SystemTray::SystemTray(DuktoWindow *window, QObject* parent) :
    QSystemTrayIcon(parent),
    window(window)
{
    QIcon icon(":/dukto.png");
#if defined(Q_OS_MAC) && QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    icon.setIsMask(true);
#endif
    setIcon(icon);
#ifndef Q_OS_MAC
    connect(this, &SystemTray::activated, this, &SystemTray::on_activated);
#endif
    
    QMenu *trayMenu = new QMenu(window);
    QAction *ShowHide = new QAction(QString("Show/Hide"), trayMenu);
    connect(ShowHide, &QAction::triggered, this, [=]() { on_activated(QSystemTrayIcon::Trigger); });
    trayMenu->addAction(ShowHide);
    QAction *Exit = new QAction(QString("Exit"), trayMenu);
    connect(Exit, &QAction::triggered, this, [=]() { on_activated(QSystemTrayIcon::MiddleClick); });
    trayMenu->addAction(Exit);
    this->setContextMenu(trayMenu);
    connect(trayMenu, &QMenu::aboutToShow, this, [window, ShowHide]() { ShowHide->setText((window->isHidden() || window->isMinimized()) ? "Show" : "Hide"); });
}

void SystemTray::on_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        case QSystemTrayIcon::Trigger:
            if (window->isHidden() || window->isMinimized()) {
                window->activateWindow();
            } else {
                window->hide();
            }
            break;
        case QSystemTrayIcon::MiddleClick:
            window->hide();
            window->close();
            QApplication::quit();
            break;
        case QSystemTrayIcon::Context:
            this->contextMenu()->exec();
            break;
        default:
            break;
    }
}

void SystemTray::notify(const QString &title, const QString &body)
{
    this->showMessage(title, body);
}
