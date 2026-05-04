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
    private static int lastProgress;
    private static Context currentContext;
    private static String currentTitle;
    private static boolean receiving;

    public static void start(Context context, String title, boolean isDownload) {
        if (notificationManager == null) {
            notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                NotificationChannel channel = new NotificationChannel(CHANNEL_ID, "Transferring Progress", NotificationManager.IMPORTANCE_DEFAULT);
                notificationManager.createNotificationChannel(channel);
            }
        }
        notificationManager.cancel(NOTIFY_ID);
        builder = new NotificationCompat.Builder(context, CHANNEL_ID);
        builder.setContentTitle(title)
               .setContentText("")
               .setProgress(100, 0, false)
               .setOngoing(true)
               .setOnlyAlertOnce(true);
        if (isDownload) {
            builder.setSmallIcon(android.R.drawable.stat_sys_download);
        } else {
            builder.setSmallIcon(android.R.drawable.stat_sys_upload);
        }
        lastProgress = 0;
        currentContext = context;
        currentTitle = title;
        receiving = isDownload;
        notificationManager.notify(NOTIFY_ID, builder.build());
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

    public static void setDone(String text) {
        if (builder != null && notificationManager != null) {
            notificationManager.cancel(NOTIFY_ID);
            builder = new NotificationCompat.Builder(currentContext, CHANNEL_ID);
            if (receiving) {
                builder.setSmallIcon(android.R.drawable.stat_sys_download_done);
            } else {
                builder.setSmallIcon(android.R.drawable.stat_sys_upload_done);
            }
            builder.setContentTitle(currentTitle)
                   .setContentText(text);
            notificationManager.notify(NOTIFY_ID, builder.build());
            builder = null;
            currentContext = null;
            currentTitle = null;
        }
    }

    public static void setError(String error) {
        if (builder != null && notificationManager != null) {
            notificationManager.cancel(NOTIFY_ID);
            builder = new NotificationCompat.Builder(currentContext, CHANNEL_ID);
            builder.setSmallIcon(android.R.drawable.stat_notify_error)
                   .setContentTitle(currentTitle)
                   .setContentText(error);
            notificationManager.notify(NOTIFY_ID, builder.build());
            builder = null;
            currentContext = null;
            currentTitle = null;
        }
    }

    public static void cancel() {
        if (notificationManager != null) {
            notificationManager.cancel(NOTIFY_ID);
            builder = null;
            currentContext = null;
            currentTitle = null;
        }
    }
}
