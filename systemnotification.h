#ifndef SYSTEMNOTIFICATION_H
#define SYSTEMNOTIFICATION_H

#include <QObject>

class DuktoWindow;

class SystemNotification : public QObject
{
    Q_OBJECT
public:
    explicit SystemNotification(DuktoWindow *window, QObject *parent = nullptr);
    ~SystemNotification();

    void notifyTransferringStarted(bool inbound, const QString &targetName, const QString &targetIp);
    void notifyTransferringCompleted(bool inbound);
    void notifyTransferringError(bool inbound, const QString &error);

    void notifyTransferringProgress(double percent);
    void notifyTransferringItem(const QString &desc);

    void resetProgress();

public Q_SLOTS:
    void fileReceived(const QString &name, const QString &path, qint64 size);
    void folderReceived(const QString &name, const QString &path);
    void textReceived(const QString &text);

private:
    void notify(const QString &title, const QString &body);

#ifdef Q_OS_WIN
    void showTaskbarProgress(uint percent);
    void hideTaskbarProgress();
    void stopTaskbarProgress();
#endif

    DuktoWindow *window;
};

#endif // SYSTEMNOTIFICATION_H
