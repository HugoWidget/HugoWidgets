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
#ifndef HUGOPROTECTPAGE_H
#define HUGOPROTECTPAGE_H

#include "../HugoMain/functionpagebase.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <optional>
#include <QTimer>
#include <QThread>
#include <QFile>
#include <QDir>

// 前置声明
namespace WinUtils {
    class Console;
}

class HugoProtectPage : public FunctionPageBase
{
    Q_OBJECT
public:
    explicit HugoProtectPage(QWidget *parent = nullptr);
    ~HugoProtectPage() override;

    // 基类纯虚函数实现
    QString id() const override;
    QString name() const override;
    bool handleCommand(const QString& command, const QVariantMap& args = QVariantMap()) override;
    void init() override;

private slots:
    // GUI按钮槽函数
    void onEnableProtect();
    void onDisableProtect();
    void checkProtectStatus();
private:
    // 核心业务逻辑（GUI/非GUI共用）
    bool executeProtectOperation(int operation);
    // 非GUI模式处理
    void handleNoGuiMode(const QVariantMap& args);
    void showInteractiveMenu();
    // 解析命令行参数
    std::optional<int> parseArgs(const QVariantMap& args);
    QTimer *m_checkTimer = nullptr;
    // GUI控件
    QPushButton* m_btnEnable;
    QPushButton* m_btnDisable;
    QLabel* m_resultLabel;
    QGridLayout* m_mainLayout;
    QLabel* m_isEnableLabel;
};

#endif // HUGOPROTECTPAGE_H
