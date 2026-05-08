package com.github.xuzhen.dukto;

import androidx.appcompat.app.AppCompatDelegate;
import androidx.core.view.WindowInsetsControllerCompat;
import android.view.Window;
import android.view.WindowManager;
import android.graphics.Color;
import android.os.Build;

public class NightModeHelper {

    public static void setNightMode(Window window, boolean on) {
        if (on) {
            AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_YES);
        } else {
            AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO);
        }

        WindowInsetsControllerCompat controller = new WindowInsetsControllerCompat(window, window.getDecorView());

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
            /*
             * We only apply a light background color to the status bar for API 23 (Android 6.0) and above.
             * Prior to API 23, there is no native support to change the status bar text and icons to a dark color.
             * Therefore, setting a light background on older versions would cause the default white system icons
             * (e.g., clock, battery, network signal) to blend into the background, making them unreadable.
             */
            if (on) {
                controller.setAppearanceLightStatusBars(false);
                window.setStatusBarColor(Color.parseColor("#121212"));
            } else {
                controller.setAppearanceLightStatusBars(true);
                window.setStatusBarColor(Color.WHITE);
            }
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            /*
             * We only modify the navigation bar background color for API 26 (Android 8.0) and above.
             * Prior to API 26, there is no native support to change the navigation bar icons to a dark color.
             * Therefore, setting a light background color on older versions would cause the default white
             * navigation buttons to blend into the background, making them invisible to the user.
             */
            if (on) {
                controller.setAppearanceLightNavigationBars(false);
                window.setNavigationBarColor(Color.parseColor("#121212"));
            } else {
                controller.setAppearanceLightNavigationBars(true);
                window.setNavigationBarColor(Color.WHITE);
            }
        }
    }
}
