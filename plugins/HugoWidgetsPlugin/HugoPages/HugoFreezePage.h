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
#ifndef HUGOFREEZEPAGE_H
#define HUGOFREEZEPAGE_H

#include <QList>
#include <QStorageInfo>
#include <QWidget>

#include "../HugoMain/FunctionPageBase.h"
#include "HugoPages/HFreezeApiEx.h"
#include "HugoPages/HFreezeNone.h"
#include "HugoUtils/HFreezeDriver.h"
#include "QDiskButton.h"
namespace Ui {
class HugoFreezePage;
}

class HugoFreezePage : public FunctionPageBase {
    Q_OBJECT

public:
    explicit HugoFreezePage(QWidget *parent = nullptr);
    ~HugoFreezePage();
    virtual void init() {}
    virtual void cleanup() {}

    virtual QString id() const;
    virtual QString name() const;

    virtual bool handleCommand(const QString &command,
                               const QVariantMap &args = QVariantMap());
private slots:
    void onComboFreezeModeCurrentTextChanged(const QString &arg1);
    void on_m_btnRefresh_clicked();
    void on_m_btnSet_clicked();

private:
    void enableAllButtons(bool enable);
    void refresh();

    Ui::HugoFreezePage *ui = nullptr;
    QVector<QDiskButton *> m_diskBtns;
    QList<QStorageInfo> diskList;

private:
    IHugoFreeze *m_freeze = nullptr;
    static HFreezeApiEx *m_freezeApi;
    static HFreezeDriver *m_freezeDriver;
    static HFreezeNone *m_freezeNone;
};

#endif // HUGOFREEZEPAGE_H
