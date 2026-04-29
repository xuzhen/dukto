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

#include "duktowindow.h"
#include "guibehind.h"
#include "platform.h"
#include "settings.h"

#ifdef Q_OS_WIN
#include "ecwin7.h"
#endif

#include <QQmlEngine>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>

#ifdef Q_OS_MAC
#include <objc/objc.h>
#include <objc/message.h>
static DuktoWindow *instance = nullptr;
#endif

#ifdef DESKTOP_APP
#include <QCoreApplication>
#include <QDir>
#endif

DuktoWindow::DuktoWindow(GuiBehind *gb, QQuickWidget *parent) :
    QQuickWidget(parent), mGuiBehind(gb)
{
    // Configure window
#ifdef DESKTOP_APP
    setAcceptDrops(true);
    setWindowTitle("Dukto");
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    setMaximumWidth(350);
    setMinimumSize(350, 520);
    setWindowIcon(QIcon(":/dukto.png"));
#endif

#ifdef Q_OS_ANDROID
    // workaround window size bug
    setMinimumSize(1, 1);
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
    setWindowFlag(Qt::ExpandedClientAreaHint, true);
#elif QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    setWindowFlag(Qt::MaximizeUsingFullscreenGeometryHint, true);
#endif
#endif

    setResizeMode(QQuickWidget::SizeRootObjectToView);
    connect(engine(), &QQmlEngine::quit, this, &DuktoWindow::close);

#ifdef Q_OS_MAC
    instance = this;
    setupDockHandler();
#endif

#ifdef Q_OS_ANDROID
    connect(this, &DuktoWindow::statusChanged, this, [this](QQuickWidget::Status status) {
        if (status == QQuickWidget::Ready) {
            mGuiBehind->updateScreenPadding();
        }
    });
#endif

#ifdef DESKTOP_APP
    // QtQuick module failed to load on older Qt5 versions
    QDir qmlDir(qApp->applicationDirPath());
    if (qmlDir.cd("qml")) {
        engine()->addImportPath(qmlDir.path());
    }
#endif

    mObserver = new PlatformObserver();
    connect(mObserver, &PlatformObserver::colorSchemeChanged, mGuiBehind, &GuiBehind::updateColorScheme);
}

DuktoWindow::~DuktoWindow() {
#ifdef Q_OS_WIN
    delete mWin7;
#endif
    delete mObserver;
}

void DuktoWindow::showTaskbarProgress(uint percent) {
#ifdef Q_OS_WIN
    if (mWin7 != nullptr) {
        mWin7->setProgressState(EcWin7::Normal);
        mWin7->setProgressValue(percent, 100);
    }
#else
    Q_UNUSED(percent)
#endif
}

void DuktoWindow::hideTaskbarProgress() {
#ifdef Q_OS_WIN
    if (mWin7 != nullptr) {
        mWin7->setProgressState(EcWin7::NoProgress);
    }
#endif
}

void DuktoWindow::stopTaskbarProgress() {
#ifdef Q_OS_WIN
    if (mWin7 != nullptr) {
        mWin7->setProgressState(EcWin7::Error);
    }
#endif
}

void DuktoWindow::activateWindow() {
    showNormal();
    raise();
    QQuickWidget::activateWindow();
}

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool DuktoWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result) {
#else
bool DuktoWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {
#endif
    Q_UNUSED(eventType)
#ifdef Q_OS_WIN
    if (mWin7 != nullptr && mWin7->winEvent(reinterpret_cast<MSG*>(message), result)) {
        return true;
    }
    if (mObserver->winEvent(reinterpret_cast<MSG*>(message), result)) {
        return true;
    }
#endif
    return false;
}
#endif

void DuktoWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Back) {
        QString state = mGuiBehind->overlayState();
        if (state == QStringLiteral("progress")) {
            event->accept();
            return;
        } else if (state == QStringLiteral("message") && mGuiBehind->messagePageBackState() == QStringLiteral("send")) {
            emit mGuiBehind->gotoSendPage();
            event->accept();
            return;
        } else if (!state.isEmpty() && state != QStringLiteral("termspage") && state != QStringLiteral("initerr")) {
            emit mGuiBehind->hideAllOverlays();
            event->accept();
            return;
        }
    }
    QQuickWidget::keyPressEvent(event);
}

void DuktoWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls() && mGuiBehind->canAcceptDrop())
        event->acceptProposedAction();
}

void DuktoWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DuktoWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void DuktoWindow::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    if (!mimeData->hasUrls()) return;

    QStringList files;
    const QList<QUrl> urlList = mimeData->urls();
    for(QList<QUrl>::const_iterator url = urlList.constBegin(); url != urlList.constEnd(); ++url)
        files.append(url->toLocalFile());

    event->acceptProposedAction();
    mGuiBehind->sendDroppedFiles(&files);
}

void DuktoWindow::closeEvent(QCloseEvent *event)
{
#ifdef DESKTOP_APP
    if (isVisible() && gSettings->closeToTrayEnabled()) {
        event->ignore();
        hide();
    } else {
        gSettings->saveWindowGeometry(saveGeometry());
        event->accept();
    }
#else
    event->accept();
#endif
}

void DuktoWindow::showEvent(QShowEvent *event) {
    QQuickWidget::showEvent(event);
    if (!debuted) {
#if defined(Q_OS_WIN)
        QWindow *win = windowHandle();
        // Taskbar integration with Win7+
        mWin7 = new EcWin7(win);
        // Title bar color scheme
        if (gSettings->autoMode()) {
            Platform::setNonClientAreaMode(win, Platform::isDarkTheme());
        } else {
            Platform::setNonClientAreaMode(win, gSettings->darkMode());
        }
#elif defined(Q_OS_ANDROID)
        if (gSettings->autoMode()) {
            Platform::setNonClientAreaMode(nullptr, Platform::isDarkTheme());
        } else {
            Platform::setNonClientAreaMode(nullptr, gSettings->darkMode());
        }
#endif
        debuted = true;
    }
}

void DuktoWindow::resizeEvent(QResizeEvent *event) {
    QQuickWidget::resizeEvent(event);
#ifdef Q_OS_ANDROID
    // As of Qt 6.10.0, QScreen::orientationChanged and other signals won't be
    // emitted after rotation between Landscape & InvertedLandscape
    mGuiBehind->updateScreenPadding();
#endif
}

#ifdef Q_OS_MAC
bool dockHasVisibleWindows(id self, SEL _cmd, id sender, bool flag) {
    Q_UNUSED(self)
    Q_UNUSED(_cmd)
    Q_UNUSED(sender)
    if (!flag) {
        // window is minimized to tray (hidden)
        instance->activateWindow();
        return false;
    }
    return true;
}

void DuktoWindow::setupDockHandler() {
    Class appClass = objc_getClass("NSApplication");
    id appInst = reinterpret_cast<id(*)(Class, SEL)>(objc_msgSend)(appClass, sel_registerName("sharedApplication"));
    if (appInst != nullptr) {
        id delegate = reinterpret_cast<id(*)(id, SEL)>(objc_msgSend)(appInst, sel_registerName("delegate"));
        if (delegate != nullptr) {
            Class delClass = reinterpret_cast<Class(*)(id, SEL)>(objc_msgSend)(delegate, sel_registerName("class"));
            if (delClass != nullptr) {
                SEL methodSelector = sel_registerName("applicationShouldHandleReopen:hasVisibleWindows:");
                class_replaceMethod(delClass, methodSelector, reinterpret_cast<IMP>(dockHasVisibleWindows), "B@:@B");
            }
        }
    }
}
#endif
