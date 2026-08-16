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
#ifndef HUGOPASSWORDPAGE_H
#define HUGOPASSWORDPAGE_H

#include "../HugoMain/functionpagebase.h"
#include "HugoUtils/HPassword.h"
#include <QComboBox>
#include <QLineEdit>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <vector>
#include <string>
#include<QTextBrowser>
class ManualInfoAcquirer : public InfoAcquirer {
private:
    int version;
    std::string ciphertext;
    PasswordType type;

public:
    ManualInfoAcquirer(int ver, const std::string& ct, PasswordType t = PasswordType::TYPE_ADMIN);
    static bool GetRegistryMachineId(std::string& machine_id) ;
    std::vector<CrackTask> acquire() override;
};
class HugoPasswordPage : public FunctionPageBase
{
    Q_OBJECT
public:
    explicit HugoPasswordPage(QWidget *parent = nullptr);
    ~HugoPasswordPage() override;

    // 实现FunctionPageBase纯虚函数
    QString id() const override;
    QString name() const override;
    bool handleCommand(const QString& command, const QVariantMap& args = QVariantMap()) override;

    // 初始化UI（重写基类）
    void init() override;

private slots:
    // GUI模式：按钮点击执行破解
    void onCrackButtonClicked();
    void onCorrButtonClicked();
private:
    // 核心破解逻辑（GUI/非GUI共用）
    std::vector<CrackResult> doCrack(int mode, int version = 0, const std::string& cipher = "");
    
    // 非GUI模式处理
    void handleNoGuiMode(const QVariantMap& args);
    void showNoGuiMenu();
    
    // GUI控件
    QPushButton* m_crackButton;
    QTextBrowser* m_resultTextBrowser;
    QGridLayout* m_mainLayout;
    QLineEdit* m_inputMing;
    QLineEdit* m_inputMi;
    QLabel* m_isCorrLabel;
    QPushButton* m_corrButton;
    QComboBox* m_pwdMode;
    template<typename T>
    void logInfo(const T& msg);
    template<typename T>
    void logError(const T& msg);
};

#endif // HUGOPASSWORDPAGE_H
