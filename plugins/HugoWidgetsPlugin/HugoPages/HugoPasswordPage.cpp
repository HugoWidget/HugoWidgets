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
#include "WinUtils/WinPch.h"

#include "HugoPasswordPage.h"
#include "HugoUtils/HPassword.h"
#include "WinUtils/Console.h"
#include "WinUtils/StrConvert.h"

#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include <Windows.h>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <shlobj.h>

using namespace WinUtils;
using namespace std;

bool ManualInfoAcquirer::GetRegistryMachineId(string &machine_id) {
    char buffer[128] = {0};
    DWORD buffer_size = sizeof(buffer);
    HKEY hKey;
    LONG result =
        RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\SQMClient", 0,
                                KEY_READ | KEY_WOW64_64KEY, &hKey);
    if (result != ERROR_SUCCESS) {
        result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\SQMClient",
                               0, KEY_READ, &hKey);
    }
    if (result != ERROR_SUCCESS)
        return false;

    result = RegQueryValueExA(hKey, "MachineId", nullptr, nullptr,
                              reinterpret_cast<LPBYTE>(buffer), &buffer_size);
    RegCloseKey(hKey);

    if (result == ERROR_SUCCESS) {
        machine_id = buffer;
        return true;
    }
    return false;
}

ManualInfoAcquirer::ManualInfoAcquirer(int ver, const string &ct,
                                       PasswordType t)
    : version(ver), ciphertext(ct), type(t) {}

vector<CrackTask> ManualInfoAcquirer::acquire() {
    vector<CrackTask> tasks;
    string device_id, machine_id;

    char seewocore_ini_path[MAX_PATH] = {0};
    if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_COMMON_APPDATA, nullptr, 0,
                                   seewocore_ini_path))) {
        strcat_s(seewocore_ini_path, MAX_PATH, "\\Seewo\\SeewoCore\\SeewoCore.ini");
        char buf_device[256] = {0};
        GetPrivateProfileStringA("device", "id", "", buf_device, sizeof(buf_device),
                                 seewocore_ini_path);
        device_id = buf_device;
    }

    GetRegistryMachineId(machine_id);

    CrackMode mode;
    if (version == 1)
        mode = CrackMode::MODE_V1;
    else if (version == 2)
        mode = CrackMode::MODE_V2;
    else
        mode = CrackMode::MODE_V3;

    tasks.push_back({mode, type, ciphertext, device_id, machine_id, "", ""});
    return tasks;
}

HugoPasswordPage::HugoPasswordPage(QWidget *parent)
    : FunctionPageBase(parent),
    m_crackButton(new QPushButton("执行密码破解", this)),
    m_resultTextBrowser(new QTextBrowser()), m_mainLayout(new QGridLayout()),
    m_inputMing(new QLineEdit()), m_inputMi(new QLineEdit()),
    m_isCorrLabel(new QLabel("明文密文是否符合将在此处显示", this)),
    m_corrButton(new QPushButton("匹配明文密文", this)),
    m_pwdMode(new QComboBox()) {
    // 初始化UI样式
    m_resultTextBrowser->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_resultTextBrowser->setText("等待执行破解.../");
    m_isCorrLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    m_pwdMode->addItem("v1");
    m_pwdMode->addItem("v2");
    m_pwdMode->addItem("v3");
    m_pwdMode->setCurrentText("");
    m_inputMing->setPlaceholderText("此处输入密码明文");
    m_inputMi->setPlaceholderText("此处输入密码密文(注意不要有额外的空格)");

    m_mainLayout->addWidget(m_crackButton, 0, 0, 1, 3);
    m_mainLayout->addWidget(m_resultTextBrowser, 1, 0, 1, 3);
    m_mainLayout->addWidget(m_corrButton, 2, 2, 1, 1);
    m_mainLayout->addWidget(m_pwdMode, 2, 0, 1, 1);
    m_mainLayout->addWidget(m_inputMing, 3, 0, 1, 1);
    m_mainLayout->addWidget(m_inputMi, 3, 1, 1, 2);
    m_mainLayout->addWidget(m_isCorrLabel, 4, 0, 1, 1);
    m_mainLayout->setSpacing(20);
    m_mainLayout->setColumnMinimumWidth(0, 50);
    m_mainLayout->setColumnMinimumWidth(0, 150);
    m_mainLayout->setColumnMinimumWidth(1, 450);
    m_mainLayout->setColumnMinimumWidth(2, 150);
    m_mainLayout->setColumnStretch(0, 0);
    m_mainLayout->setColumnStretch(1, 2);
    m_mainLayout->setColumnStretch(2, 0);
    m_mainLayout->setContentsMargins(20, 20, 20, 20);
    setLayout(m_mainLayout); // 把布局设置到当前页面
}

HugoPasswordPage::~HugoPasswordPage() = default;

QString HugoPasswordPage::id() const { return "Hugo.Password"; }

QString HugoPasswordPage::name() const { return "密码破解"; }

void HugoPasswordPage::init() {
    if (!noGuiMode()) {
        connect(m_crackButton, &QPushButton::clicked, this,
                &HugoPasswordPage::onCrackButtonClicked);
        connect(m_corrButton, &QPushButton::clicked, this,
                &HugoPasswordPage::onCorrButtonClicked);
    }
}

bool HugoPasswordPage::handleCommand(const QString &command,
                                     const QVariantMap &args) {
    if (noGuiMode()) {
        handleNoGuiMode(args);
        return true;
    }

    if (command == "crack") {
        int mode = args.value("mode", 0).toInt();
        int version = args.value("version", 0).toInt();
        string cipher = args.value("cipher", "").toString().toStdString();

        auto results = doCrack(mode, version, cipher);
        QString resultText;
        for (const auto &res : results) {
            resultText +=
                QString("模式：%1, 类型：%2, 结果：%3\n")
                              .arg(static_cast<int>(res.task.mode))
                              .arg(res.task.type == PasswordType::TYPE_ADMIN ? "管理密码"
                                                                             : "锁屏密码")
                              .arg(res.success
                                       ? QString::fromStdString(res.plaintext)
                                       : QString("失败：%1")
                                             .arg(QString::fromStdString(res.error_message)));
        }
        m_resultTextBrowser->setText(resultText);
        return true;
    }

    return false;
}

void HugoPasswordPage::onCrackButtonClicked() {
    m_crackButton->setEnabled(false);
    m_resultTextBrowser->setText("正在破解中，请稍候...\n");

    auto results = doCrack(1); // 1=自动模式
    QString resultText;
    for (const auto &res : results) {
        resultText +=
            QString("==================== 破解结果 ====================\n");
        resultText += QString("模式：%1\n").arg(static_cast<int>(res.task.mode));
        resultText +=
            QString("类型：%1\n")
                          .arg(res.task.type == PasswordType::TYPE_ADMIN ? "管理密码"
                                                                         : "锁屏密码");
        resultText +=
            QString("密文：%1\n").arg(QString::fromStdString(res.task.ciphertext));
        resultText +=
            QString("设备ID：%1\n").arg(QString::fromStdString(res.task.device_id));
        resultText += QString("机器ID：%1\n")
                          .arg(QString::fromStdString(res.task.machine_id));
        resultText +=
            QString("路径：%1\n").arg(QString::fromStdString(res.task.method));
        resultText +=
            QString("结果：%1\n")
                          .arg(res.success
                                   ? QString::fromStdString(res.plaintext)
                                   : QString("失败：%1")
                                         .arg(QString::fromStdString(res.error_message)));
        resultText +=
            QString("========================================================\n");
    }

    m_resultTextBrowser->setText(resultText);
    m_crackButton->setEnabled(true);
}

void HugoPasswordPage::onCorrButtonClicked() {
    CrackTask task;
    if (m_pwdMode->currentText() == "v1")
        task.mode = CrackMode::MODE_V1;
    else if (m_pwdMode->currentText() == "v2")
        task.mode = CrackMode::MODE_V2;
    else
        task.mode = CrackMode::MODE_V3;
    char seewocore_ini_path[MAX_PATH] = {0};
    if (SUCCEEDED(SHGetFolderPathA(nullptr, CSIDL_COMMON_APPDATA, nullptr, 0,
                                   seewocore_ini_path))) {
        strcat_s(seewocore_ini_path, MAX_PATH, "\\Seewo\\SeewoCore\\SeewoCore.ini");
        char buf_device[256] = {0};
        GetPrivateProfileStringA("device", "id", "", buf_device, sizeof(buf_device),
                                 seewocore_ini_path);
        task.device_id = buf_device;
    }
    ManualInfoAcquirer::GetRegistryMachineId(task.machine_id);
    task.plaintext = m_inputMing->text().toStdString();
    task.ciphertext = m_inputMi->text().toStdString();
    bool isCorrespond = false;
    if (task.mode == CrackMode::MODE_V1) {
        V1Decryptor d1;
        isCorrespond = d1.matched(task);
    }
    if (task.mode == CrackMode::MODE_V2) {
        V2Decryptor d2;
        isCorrespond = d2.matched(task);
    }
    if (task.mode == CrackMode::MODE_V3) {
        V3Decryptor d3;
        isCorrespond = d3.matched(task);
    }
    m_isCorrLabel->setText(isCorrespond ? "✅密文与密码匹配！"
                                        : "❌密文与密码不匹配！");
}

vector<CrackResult> HugoPasswordPage::doCrack(int mode, int version,
                                              const string &cipher) {
    InfoAcquirer *acquirer = nullptr;
    if (mode == 1) {
        acquirer = new AutoInfoAcquirer();
        logInfo("使用自动模式获取破解任务");
    } else {
        acquirer = new ManualInfoAcquirer(version, cipher);
        logInfo(QString("使用手动模式：版本%1，密文%2")
                    .arg(version)
                    .arg(QString::fromStdString(cipher))
                    .toStdString());
    }

    vector<CrackTask> tasks = acquirer->acquire();
    delete acquirer;

    if (tasks.empty()) {
        logError("无破解任务");
        return {};
    }

    // 执行破解
    V1Decryptor d1;
    V2Decryptor d2;
    V3Decryptor d3;
    vector<Decryptor *> decryptors = {&d1, &d2, &d3};

    CrackExecutor executor;
    vector<CrackResult> results = executor.execute(tasks, decryptors);

    return results;
}

void HugoPasswordPage::handleNoGuiMode(const QVariantMap &args) {
    Console console;
    console.setLocale();

    // 有参数时执行命令
    if (!args.isEmpty()) {
        int mode = args.value("mode", 0).toInt();
        int version = args.value("version", 0).toInt();
        string cipher = args.value("cipher", "").toString().toStdString();

        auto results = doCrack(mode, version, cipher);

        // 非GUI输出结果
        wcout << L"\n==================== 破解结果 ====================\n";
        for (const auto &res : results) {
            wstring modeStr = (res.task.mode == CrackMode::MODE_V1)   ? L"V1"
                              : (res.task.mode == CrackMode::MODE_V2) ? L"V2"
                                                                      : L"V3";
            wstring typeStr = (res.task.type == PasswordType::TYPE_ADMIN)
                                  ? L"管理密码"
                                  : L"锁屏密码";
            wcout << L"[" << modeStr << L"] " << typeStr << L" : ";
            if (res.success) {
                wcout << L"成功 -> " << ConvertString<wstring>(res.plaintext) << endl;
            } else {
                wcout << L"失败 (" << ConvertString<wstring>(res.error_message) << L")"
                      << endl;
            }
        }
        wcout << L"========================================================\n";
        return;
    }

    // 无参数时显示原始菜单
    showNoGuiMenu();
}

void HugoPasswordPage::showNoGuiMenu() {
    Console console;
    console.setLocale();

    int choice = -1;
    wcout << L"选择模式:\n" << L"0 - 手动输入\n" << L"1 - 自动读取\n";
    wcin >> choice;

    vector<CrackResult> results;
    if (choice == 1) {
        results = doCrack(1);
    } else {
        int ver;
        wstring wcipher;
        wcout << L"密文版本 (1/2/3): ";
        wcin >> ver;
        wcout << L"密文: ";
        wcin >> wcipher;
        string cipher = ConvertString<string>(wcipher);
        results = doCrack(0, ver, cipher);
    }

    // 输出结果
    wcout << L"\n==================== 破解结果 ====================\n";
    for (const auto &res : results) {
        wstring modeStr = (res.task.mode == CrackMode::MODE_V1)   ? L"V1"
                          : (res.task.mode == CrackMode::MODE_V2) ? L"V2"
                                                                  : L"V3";
        wstring typeStr =
            (res.task.type == PasswordType::TYPE_ADMIN) ? L"管理密码" : L"锁屏密码";
        wcout << L"[" << modeStr << L"] " << typeStr << L" : ";
        if (res.success) {
            wcout << L"成功 -> " << ConvertString<wstring>(res.plaintext) << endl;
        } else {
            wcout << L"失败 (" << ConvertString<wstring>(res.error_message) << L")"
                  << endl;
        }
    }
    wcout << L"========================================================\n";

    wcout << L"\n按任意键继续" << endl;
    (void)_getwch();
}

template <typename T> void HugoPasswordPage::logInfo(const T &msg) {
    if (noGuiMode()) {
        wcout << L"[INFO] " << ConvertString<wstring>(msg) << endl;
    }
}

template <typename T> void HugoPasswordPage::logError(const T &msg) {
    if (noGuiMode()) {
        wcerr << L"[ERROR] " << ConvertString<wstring>(msg) << endl;
    }
}
