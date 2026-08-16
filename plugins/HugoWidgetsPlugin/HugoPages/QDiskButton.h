/*
 * Copyright 2025-2026 howdy213, JYardX
 *
 * This file is part of HugoWidgets.
 *
 * HugoWidgets is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HugoWidgets is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HugoWidgets. If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef QDISKBUTTON_H
#define QDISKBUTTON_H

#include <QCheckBox>
#include <QColor>

#include "HugoUtils/HFreezeInterface.h"

class QDiskButton : public QCheckBox {
    Q_OBJECT

public:
    explicit QDiskButton(QWidget *parent = nullptr);
    void applyChanges();
    void setDriveInfo(const QString &drive, long long total, long long available);
    void setStatus(DriveFreezeState status);
    wchar_t getDriveLetter();

protected:
    void paintEvent(QPaintEvent *event) override;
    bool hitButton(const QPoint &pos) const override;

private:
    void init();
    int m_padding = 2;
    int m_borderWidth = 4;
    QColor m_colorUnchecked = Qt::gray;
    QColor m_colorChecked = QColor(41, 128, 185);
    QColor m_bgColor = Qt::white;
    QColor m_textColor = Qt::black;
    int m_fixedWidth = 200;
    int m_fixedHeight = 100;
    QString m_driveLetter = "C";
    long long m_totalSpace = 0;
    long long m_availableSpace = 0;
    DriveFreezeState m_status = DriveFreezeState::Unknown;
};

#endif // QDISKBUTTON_H
