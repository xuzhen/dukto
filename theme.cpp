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

#include "theme.h"

#include <QColor>

const QString Theme::DEFAULT_THEME_COLOR = "#248b00";

Theme::Theme(QObject *parent) : QObject(parent)
{
}

void Theme::setThemeColor(const QString &color)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
    QColor c = QColor::fromString(color);
#else
    QColor c;
    c.setNamedColor(color);
#endif
    if (mThemeColor == c.name()) {
        return;
    }
    int lightness = c.lightness();
    QColor lc, dc;
    if (lightness < 60) {
        lc = c.lighter(180);
    } else if (lightness <= 210) {
        lc = c.lighter(120);
    } else {
        lc = c;
    }
    if (lightness >= 20) {
        dc = c.darker(120);
    } else {
        dc = c;
    }
    mThemeColor = c.name();
    mThemeLighterColor = lc.name();
    mThemeDarkerColor = dc.name();
    if (lightness >= 200) {
        mThemeTextColor = "#555555";
        mThemeBgColor = "#aaaaaa";
    } else {
        mThemeTextColor = "#fcfcfc";
        mThemeBgColor = "#fcfcfc";
    }
    emit themeColorChanged();
    emit themeLighterColorChanged();
    emit themeDarkerColorChanged();
    emit themeTextColorChanged();
    emit themeBgColorChanged();
}

void Theme::setDarkMode(bool darkMode) {
    if (darkMode) {
        mTextColor = "#cccccc";
        mDimmedTextColor = "#999999";
        mBgColor = "#333333";
        mLighterBgColor = "#444444";
        mDarkerBgColor = "#222222";
        mBorderColor = "#444444";
        mDisabledColor = "#666666";
        mInactiveTextColor = "#555555";
        mOverlayColor = "#333333";
    } else {
        mTextColor = "#555555";
        mDimmedTextColor = "#888888";
        mBgColor = "#fcfcfc";
        mLighterBgColor = "#eeeeee";
        mDarkerBgColor = "#cccccc";
        mBorderColor = "#d0d0d0";
        mDisabledColor = "#cccccc";
        mInactiveTextColor = "#bbbbbb";
        mOverlayColor = "#ffffff";
    }
    mDarkMode = darkMode;

    emit textColorChanged();
    emit dimmedTextColorChanged();
    emit bgColorChanged();
    emit lighterBgColorChanged();
    emit darkerBgColorChanged();
    emit borderColorChanged();
    emit inactiveTextColorChanged();
    emit disabledColorChanged();
    emit overlayColorChanged();
}

float Theme::getHue(const QString &color) {

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
    QColor c = QColor::fromString(color);
#else
    QColor c;
    c.setNamedColor(color);
#endif
    QColor converted = c.toHsv();
    return converted.hsvHueF();
}

float Theme::getSaturation(const QString &color) {

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
    QColor c = QColor::fromString(color);
#else
    QColor c;
    c.setNamedColor(color);
#endif
    QColor converted = c.toHsv();
    return converted.hsvSaturationF();
}

float Theme::getLightness(const QString &color) {

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
    QColor c = QColor::fromString(color);
#else
    QColor c;
    c.setNamedColor(color);
#endif
    QColor converted = c.toHsv();
    return converted.valueF();
}
