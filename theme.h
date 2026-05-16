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

#ifndef THEME_H
#define THEME_H

#include <QObject>

class Theme : public QObject
{
    Q_OBJECT
    // determined by theme color
    Q_PROPERTY(QString themeColor READ themeColor NOTIFY themeColorChanged)
    Q_PROPERTY(QString themeLighterColor READ themeLighterColor NOTIFY themeLighterColorChanged)
    Q_PROPERTY(QString themeDarkerColor READ themeDarkerColor NOTIFY themeDarkerColorChanged)
    Q_PROPERTY(QString themeTextColor READ themeTextColor NOTIFY themeTextColorChanged)
    Q_PROPERTY(QString themeBgColor READ themeBgColor NOTIFY themeBgColorChanged)

    // determined by dark mode
    Q_PROPERTY(QString textColor READ textColor NOTIFY textColorChanged)
    Q_PROPERTY(QString dimmedTextColor READ dimmedTextColor NOTIFY dimmedTextColorChanged)
    Q_PROPERTY(QString inactiveTextColor READ inactiveTextColor NOTIFY inactiveTextColorChanged)

    Q_PROPERTY(QString bgColor READ bgColor NOTIFY bgColorChanged)
    Q_PROPERTY(QString lighterBgColor READ lighterBgColor NOTIFY lighterBgColorChanged)
    Q_PROPERTY(QString darkerBgColor READ darkerBgColor NOTIFY darkerBgColorChanged)

    Q_PROPERTY(QString borderColor READ borderColor NOTIFY borderColorChanged)
    Q_PROPERTY(QString disabledColor READ disabledColor NOTIFY disabledColorChanged)

    Q_PROPERTY(QString overlayColor READ overlayColor NOTIFY overlayColorChanged)

public:
    explicit Theme(QObject *parent = nullptr);
    inline QString themeColor() { return mThemeColor; }
    inline QString themeLighterColor() { return mThemeLighterColor; }
    inline QString themeDarkerColor() { return mThemeDarkerColor; }
    inline QString themeTextColor() { return mThemeTextColor; }
    inline QString themeBgColor() { return mThemeBgColor; }

    inline QString textColor() { return mTextColor; }
    inline QString dimmedTextColor() { return mDimmedTextColor; }
    inline QString inactiveTextColor() { return mInactiveTextColor; }
    inline QString bgColor() { return mBgColor; }
    inline QString lighterBgColor() { return mLighterBgColor; }
    inline QString darkerBgColor() { return mDarkerBgColor; }
    inline QString borderColor() { return mBorderColor; }
    inline QString disabledColor() { return mDisabledColor; }
    inline QString overlayColor() { return mOverlayColor; }

    inline bool darkMode() { return mDarkMode; }
    void setDarkMode(bool darkMode);

    void setThemeColor(const QString &color);

    static const QString DEFAULT_THEME_COLOR;

Q_SIGNALS:
    void themeColorChanged();
    void themeLighterColorChanged();
    void themeDarkerColorChanged();
    void themeTextColorChanged();
    void themeBgColorChanged();

    void textColorChanged();
    void dimmedTextColorChanged();
    void inactiveTextColorChanged();
    void bgColorChanged();
    void lighterBgColorChanged();
    void darkerBgColorChanged();
    void borderColorChanged();
    void disabledColorChanged();
    void overlayColorChanged();

public Q_SLOTS:
    float getHue(const QString &color);
    float getSaturation(const QString &color);
    float getLightness(const QString &color);

private:
    QString mThemeColor;
    QString mThemeLighterColor;
    QString mThemeDarkerColor;
    QString mThemeTextColor;
    QString mThemeBgColor;

    QString mTextColor;
    QString mDimmedTextColor;
    QString mInactiveTextColor;
    QString mBgColor;
    QString mLighterBgColor;
    QString mDarkerBgColor;
    QString mBorderColor;
    QString mDisabledColor;
    QString mOverlayColor;
    bool mDarkMode = false;
};

#endif // THEME_H
