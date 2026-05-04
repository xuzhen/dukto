package com.github.xuzhen.dukto;

import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Context;
import android.os.Build;
import androidx.core.app.NotificationCompat;

public class NotificationHelper {
    private static final String CHANNEL_ID = "DuktoChannel";
    private static final int NOTIFY_ID = 1001;
    private static NotificationManager notificationManager;
    private static NotificationCompat.Builder builder;
    private static boolean receiving;
    private static int lastProgress;

    public static void init(Context context) {
        notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID, "Transferring Progress", NotificationManager.IMPORTANCE_LOW);
            notificationManager.createNotificationChannel(channel);
        }
        builder = new NotificationCompat.Builder(context, CHANNEL_ID);
        builder.setOnlyAlertOnce(true);
    }

    public static void start(String title, boolean isDownload) {
        if (builder != null && notificationManager != null) {
            receiving = isDownload;
            builder.setContentTitle(title)
                   .setContentText("")
                   .setProgress(100, 0, false)
                   .setOngoing(true);
            if (receiving) {
                builder.setSmallIcon(android.R.drawable.stat_sys_download);
            } else {
                builder.setSmallIcon(android.R.drawable.stat_sys_upload);
            }
            lastProgress = 0;
            notificationManager.notify(NOTIFY_ID, builder.build());
        }
    }

    public static void setProgress(int progress) {
        if (builder != null && notificationManager != null) {
            if (progress != lastProgress) {
                builder.setProgress(100, progress, false);
                notificationManager.notify(NOTIFY_ID, builder.build());
                lastProgress = progress;
            }
        }
    }

    public static void setText(String text) {
        if (builder != null && notificationManager != null) {
            builder.setContentText(text);
            notificationManager.notify(NOTIFY_ID, builder.build());
        }
    }

    public static void setTitle(String title) {
        if (builder != null && notificationManager != null) {
            builder.setContentTitle(title);
            notificationManager.notify(NOTIFY_ID, builder.build());
        }
    }

    public static void setDone(String text) {
        if (builder != null && notificationManager != null) {
            if (receiving) {
                builder.setSmallIcon(android.R.drawable.stat_sys_download_done);
            } else {
                builder.setSmallIcon(android.R.drawable.stat_sys_upload_done);
            }
            builder.setContentText(text)
                   .setProgress(0, 0, false)
                   .setOngoing(false);
            lastProgress = -1;
            notificationManager.notify(NOTIFY_ID, builder.build());
        }
    }

    public static void setError(String error) {
        if (builder != null && notificationManager != null) {
            builder.setSmallIcon(android.R.drawable.stat_notify_error)
                   .setContentText(error)
                   .setOngoing(false);
            // wait a bit to ensure UI refreshing
            try {
                Thread.sleep(100);
            } catch(InterruptedException e) {
                e.printStackTrace();
            }
            notificationManager.notify(NOTIFY_ID, builder.build());
        }
    }

    public static void cancel() {
        if (notificationManager != null) {
            notificationManager.cancel(NOTIFY_ID);
        }
    }
}
