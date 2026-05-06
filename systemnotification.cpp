#ifdef DESKTOP_APP
#ifdef NOTIFY_LIBNOTIFY
// put this on top, leave glib's mess behind
#include <libnotify/notify.h>
#else
#include "systemtray.h"
#endif
#endif

#ifdef Q_OS_WIN
#include "ecwin7.h"
#endif

#include "systemnotification.h"
#include "settings.h"
#include "duktowindow.h"


#ifdef Q_OS_ANDROID
#include "androidutils.h"
#endif

SystemNotification::SystemNotification(DuktoWindow *window, QObject *parent) : QObject(parent), window(window) {
#ifdef NOTIFY_LIBNOTIFY
    notify_init("Dukto");
#endif
}

SystemNotification::~SystemNotification() {
#ifdef NOTIFY_LIBNOTIFY
    notify_uninit();
#endif
}

void SystemNotification::notifyFileReceived(const QString &name, const QString &path, qint64 size) {
    Q_UNUSED(size)
    Q_UNUSED(path)
    notify("Recieved File", name);
}

void SystemNotification::notifyFolderReceived(const QString &name, const QString &path) {
    Q_UNUSED(path)
    notify("Recieved Folder", name);
}

void SystemNotification::notifyTextReceived(const QString &text) {
    notify("Recieved Text Snippet", text);
}

void SystemNotification::notifyTransferringStarted(bool inbound, const QString &buddyName, const QString &buddyIp) {
    Q_UNUSED(buddyIp)
#ifdef Q_OS_WIN
    showTaskbarProgress(0);
#endif
    if (!gSettings->notificationEnabled()) {
        return;
    }
#ifdef Q_OS_ANDROID
    if (inbound) {
        AndroidNotification::start("Receiving from " + buddyName, true);
    } else {
        AndroidNotification::start("Sending to " + buddyName, false);
    }
#else
    Q_UNUSED(inbound)
    Q_UNUSED(buddyName)
#endif
}

void SystemNotification::notifyTransferringCompleted(bool inbound) {
#ifdef Q_OS_WIN
    hideTaskbarProgress();
#endif
    if (!gSettings->notificationEnabled()) {
        return;
    }
#ifdef Q_OS_ANDROID
    if (inbound) {
        AndroidNotification::setDone("All data has been received!");
    } else {
        AndroidNotification::setDone("All data has been sent!");
    }
#else
    Q_UNUSED(inbound)
#endif
}

void SystemNotification::notifyTransferringError(bool inbound, const QString &error) {
    Q_UNUSED(inbound)
#ifdef Q_OS_WIN
    stopTaskbarProgress();
#endif
    if (!gSettings->notificationEnabled()) {
        return;
    }
#ifdef Q_OS_ANDROID
    if (error.isEmpty() == false) {
        AndroidNotification::setError(error);
    } else {
        // no reason, cancelled by user
        AndroidNotification::cancel();
    }
#else
    Q_UNUSED(error)
#endif
}

void SystemNotification::notifyTransferringProgress(double percent) {
#ifdef Q_OS_WIN
    showTaskbarProgress(percent);
#endif
    if (!gSettings->notificationEnabled()) {
        return;
    }
#ifdef Q_OS_ANDROID
    AndroidNotification::setProgress(percent);
#else
    Q_UNUSED(percent)
#endif
}

void SystemNotification::notifyTransferringItem(const QString &desc) {
    if (!gSettings->notificationEnabled()) {
        return;
    }
    // desc = "(<current> / <total>)  <name>"
#ifdef Q_OS_ANDROID
    AndroidNotification::setText(desc);
#else
    Q_UNUSED(desc)
#endif
}

void SystemNotification::notify(const QString &title, const QString &text) {
    if (!gSettings->notificationEnabled()) {
        return;
    }
#ifdef DESKTOP_APP
#ifdef NOTIFY_LIBNOTIFY
    QByteArray titleBytes = title.toUtf8();
    QByteArray textBytes = text.toUtf8();
    NotifyNotification* msg = notify_notification_new(titleBytes.constData(), textBytes.constData(), nullptr);
    notify_notification_show (msg, nullptr);
    g_object_unref(G_OBJECT(msg));
#else
    window->getTray()->notify(title, text);
#endif
#else
    Q_UNUSED(title)
    Q_UNUSED(text)
#endif
}

void SystemNotification::resetVisualEffects() {
#ifdef Q_OS_WIN
    hideTaskbarProgress();
#endif
}

#ifdef Q_OS_WIN

void SystemNotification::showTaskbarProgress(uint percent) {
    EcWin7 *taskbar = window->getTaskBar();
    if (taskbar != nullptr) {
        taskbar->setProgressState(EcWin7::Normal);
        taskbar->setProgressValue(percent, 100);
    }
}

void SystemNotification::hideTaskbarProgress() {
    EcWin7 *taskbar = window->getTaskBar();
    if (taskbar != nullptr) {
        taskbar->setProgressState(EcWin7::NoProgress);
    }
}

void SystemNotification::stopTaskbarProgress() {
    EcWin7 *taskbar = window->getTaskBar();
    if (taskbar != nullptr) {
        taskbar->setProgressState(EcWin7::Error);
    }
}

#endif
