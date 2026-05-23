/*
 * Copyright 2025-2026 howdy213, JYardX
 *
 * This file is part of HugoProgs.
 *
 * HugoProgs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HugoProgs is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HugoProgs. If not, see <https://www.gnu.org/licenses/>.
 */
#include "QDiskButton.h"
#include <QFontMetrics>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>

QDiskButton::QDiskButton(QWidget *parent) : QCheckBox(parent) { init(); }

void QDiskButton::init() {
    setFixedSize(m_fixedWidth, m_fixedHeight);
    update();
    updateGeometry();
}

void QDiskButton::applyChanges() {
    setFixedSize(m_fixedWidth, m_fixedHeight);
    update();
    updateGeometry();
}

void QDiskButton::setDriveInfo(const QString &drive, long long total,
                               long long available) {
    m_driveLetter = drive;
    m_totalSpace = total;
    m_availableSpace = available;
    update();
}

void QDiskButton::setStatus(DriveFreezeState status) {
    m_status = status;
    setChecked(status == DriveFreezeState::PendingFreeze ||
               status == DriveFreezeState::Frozen);
    update();
}

wchar_t QDiskButton::getDriveLetter() {
    return m_driveLetter[0].toUpper().toLatin1();
}

void QDiskButton::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int totalInset = m_padding + m_borderWidth / 2;
    QRect boxRect =
        rect().adjusted(totalInset, totalInset, -totalInset, -totalInset);

    bool checked = isChecked();
    bool enabled = isEnabled();
    QPen pen;
    pen.setWidth(m_borderWidth);
    pen.setCapStyle(Qt::RoundCap);
    pen.setColor(enabled ? (checked ? m_colorChecked : m_colorUnchecked)
                         : Qt::lightGray);

    QBrush brush(m_bgColor, Qt::SolidPattern);

    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(boxRect);

    if (checked) {
        const int checkMargin = 6;
        const int checkBgSize = 18;
        QRect checkBgRect(checkMargin, checkMargin, checkBgSize, checkBgSize);

        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(41, 128, 185));
        painter.drawRect(checkBgRect);

        QPen checkPen(Qt::white, 2.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(checkPen);

        QPointF p1(checkBgRect.left() + 4, checkBgRect.top() + 10);
        QPointF p2(checkBgRect.left() + 7, checkBgRect.top() + 13);
        QPointF p3(checkBgRect.left() + 14, checkBgRect.top() + 6);

        QPainterPath checkPath;
        checkPath.moveTo(p1);
        checkPath.lineTo(p2);
        checkPath.lineTo(p3);
        painter.drawPath(checkPath);
    }

    painter.setPen(m_textColor);
    QString driveText =
        QString("%1盘\n总空间：%2\n可用空间：%3")
                            .arg(m_driveLetter)
                            .arg(m_totalSpace ? QString::number(m_totalSpace) : "--")
                            .arg(m_availableSpace ? QString::number(m_availableSpace) : "--");
    painter.drawText(boxRect, Qt::AlignCenter, driveText);

    QString statusText;
    QColor statusBg;
    switch (m_status) {
    case DriveFreezeState::Unfrozen:
        statusText = "未冻结";
        statusBg = QColor(255, 200, 150);
        break;
    case DriveFreezeState::Frozen:
        statusText = "已冻结";
        statusBg = QColor(144, 238, 144);
        break;
    case DriveFreezeState::PendingFreeze:
        statusText = "将冻结";
        statusBg = QColor(216, 191, 216);
        break; // 浅紫
    case DriveFreezeState::PendingUnfreeze:
        statusText = "将解冻";
        statusBg = QColor(173, 216, 230);
        break; // 浅蓝
    case DriveFreezeState::Unknown:
        statusText = "未知";
        statusBg = QColor(230, 230, 230);
        break;
    default:
        statusText = "";
        statusBg = Qt::transparent;
        break;
    }

    if (!statusText.isEmpty()) {
        QFontMetrics fm(painter.font());
        int textWidth = fm.horizontalAdvance(statusText);
        int textHeight = fm.height();
        const int labelPaddingH = 4; // 水平内边距
        const int labelPaddingV = 2; // 垂直内边距
        int labelWidth = textWidth + 2 * labelPaddingH;
        int labelHeight = textHeight + 2 * labelPaddingV;

        const int margin = 4; // 与边框的间距
        int labelX = boxRect.right() - labelWidth - margin;
        int labelY = boxRect.top() + margin;

        // 防止标签太大超出 boxRect
        if (labelX < boxRect.left())
            labelX = boxRect.left();
        if (labelY + labelHeight > boxRect.bottom())
            labelY = boxRect.bottom() - labelHeight;

        QRect labelRect(labelX, labelY, labelWidth, labelHeight);

        // 绘制标签背景
        painter.setPen(Qt::NoPen);
        painter.setBrush(statusBg);
        painter.drawRoundedRect(labelRect, 4, 4);

        // 绘制标签文字
        painter.setPen(Qt::black);
        painter.drawText(labelRect, Qt::AlignCenter, statusText);
    }
}

bool QDiskButton::hitButton(const QPoint &pos) const {
    Q_UNUSED(pos);
    return true; // 整个控件均可点击
}
