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
#ifndef HUGOMOUNTPAGE_H
#define HUGOMOUNTPAGE_H

#include "../HugoMain/functionpagebase.h"
#include <QString>
#include <QWidget>
namespace Ui {
class HugoMountPage;
}

class HugoMountPage : public FunctionPageBase {
    Q_OBJECT

public:
    explicit HugoMountPage(QWidget *parent = nullptr);
    ~HugoMountPage();
    virtual void init() {}
    virtual void cleanup() {}

    virtual QString id() const;
    virtual QString name() const;

    virtual bool handleCommand(const QString &command,
                               const QVariantMap &args = QVariantMap());

private slots:
    void on_m_btnQuery_clicked();
    void on_m_btnMount_clicked();
    void on_pushButton_clicked();
    void on_m_btnUnmount_clicked();

private:
    Ui::HugoMountPage *ui;
    void queryAndRefresh();
};

#endif // HUGOMOUNTPAGE_H
