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

#include "HugoProtectPage.h"
#include "WinUtils/WinUtils.h"
#include "WinUtils/Console.h"
#include "WinUtils/Logger.h"
#include "WinUtils/WinSvcMgr.h"
#include "HugoUtils/HInfo.h"

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <limits>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;
using namespace WinUtils;

HugoProtectPage::HugoProtectPage(QWidget *parent)
    : FunctionPageBase(parent)
    , m_btnEnable(new QPushButton("开启文件保护"))
    , m_btnDisable(new QPushButton("关闭文件保护"))
    , m_resultLabel(new QLabel("操作结果将显示在这里"))
    , m_mainLayout(new QGridLayout(this))
    , m_isEnableLabel(new QLabel("是否开启了文件保护"))
{
    m_resultLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_isEnableLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

HugoProtectPage::~HugoProtectPage() = default;

QString HugoProtectPage::id() const
{
    return "Hugo.Protect";
}

QString HugoProtectPage::name() const
{
    return "文件保护";
}

void HugoProtectPage::init()
{
    if (!noGuiMode()) {
        m_mainLayout->addWidget(m_btnEnable,0,0);
        m_mainLayout->addWidget(m_btnDisable,0,1);
        m_mainLayout->addWidget(m_resultLabel,1,0);
        m_mainLayout->addWidget(m_isEnableLabel,1,1);
        m_mainLayout->setSpacing(15);
        m_mainLayout->setContentsMargins(20, 20, 20, 20);
        connect(m_btnEnable, &QPushButton::clicked, this, &HugoProtectPage::onEnableProtect);
        connect(m_btnDisable, &QPushButton::clicked, this, &HugoProtectPage::onDisableProtect);
        m_checkTimer = new QTimer(this);
        connect(m_checkTimer, &QTimer::timeout, this, &HugoProtectPage::checkProtectStatus);
        m_checkTimer->start(500);
    }
}

bool HugoProtectPage::handleCommand(const QString& command, const QVariantMap& args)
{
    if (noGuiMode()) {
        handleNoGuiMode(args);
        return true;
    }

    if (command == "protect") {
        int op = args.value("op", -1).toInt();
        bool ret = executeProtectOperation(op);
        m_resultLabel->setText(ret ? "操作成功" : "操作失败");
        return true;
    }
    return false;
}

void HugoProtectPage::onEnableProtect()
{
    bool ret = executeProtectOperation(1);
    WinUtils::TerminateProcessesByName(L"HugoLaunchTool.exe");
    WinUtils::WinSvcMgr(L"SeewoCoreService").Start();
    m_resultLabel->setText(ret ? "✔️ 开启文件保护成功" : "❌ 开启文件保护失败");
}

void HugoProtectPage::onDisableProtect()
{
    bool ret = executeProtectOperation(0);
    WinUtils::TerminateProcessesByName(L"HugoLaunchTool.exe");
    WinUtils::WinSvcMgr(L"SeewoCoreService").Start();
    m_resultLabel->setText(ret ? "✔️ 关闭文件保护成功" : "❌ 关闭文件保护失败");
}

bool HugoProtectPage::executeProtectOperation(int operation)
{
    if (operation != 0 && operation != 1) {
        WuLog::Log(LogLevel::Error, L"无效操作参数");
        return false;
    }
    HInfo info;
    auto driverPath = info.getHugoProtectDriverPath();
    if (!driverPath.has_value()) {
        WuLog::Log(LogLevel::Error, L"未找到 DriverService.exe 路径");
        return false;
    }
    const wstring opDesc = operation ? L"开启" : L"关闭";
    const wstring opCmd = operation ? L"install" : L"uninstall";
    WuLog::Log(LogLevel::Info, format(L"[执行{}] DriverService 路径：{}", opDesc, driverPath.value().wstring()));
    bool success = RunExternalProgram(*driverPath, L"runas", opCmd, *driverPath,0);
    WuLog::Log(success ? LogLevel::Info : LogLevel::Error,
         format(L"{}操作{}！", opDesc, success ? L"成功" : L"失败"));
    return success;
}

std::optional<int> HugoProtectPage::parseArgs(const QVariantMap& args)
{
    if (args.isEmpty()) return nullopt;

    QString cmd = args.value("cmd").toString();
    if (cmd == "-enable") return 1;
    if (cmd == "-disable") return 0;

    WuLog::Log(LogLevel::Error, L"参数无效！");
    return nullopt;
}

void HugoProtectPage::showInteractiveMenu()
{
    Console console;
    console.setLocale();

    int choice = -1;
    while (true) {
        wcout << L"0 - 关闭 文件保护" << endl;
        wcout << L"1 - 开启 文件保护" << endl;
        wcout << L"请输入数字：";

        wcin >> choice;
        if (wcin.fail() || (choice != 0 && choice != 1)) {
            wcin.clear();
            wcin.ignore((numeric_limits<streamsize>::max)(), L'\n');
            WuLog::Log(LogLevel::Error, L"输入无效");
        } else break;
    }

    executeProtectOperation(choice);
    wcout << L"\n操作完成，按任意键退出..." << endl;
    (void)_getwch();
}

void HugoProtectPage::handleNoGuiMode(const QVariantMap& args)
{
    Console console;
    console.setLocale();
    LoggerCore::Inst().AddStrategy<ConsoleLogStrategy>();
    LoggerCore::Inst().EnableApartment(DftLogger);
    auto op = parseArgs(args);
    if (op.has_value()) {
        executeProtectOperation(*op);
    } else {
        showInteractiveMenu();
    }
}

void HugoProtectPage::checkProtectStatus()
{
    m_checkTimer->stop();

    QThread *thread = QThread::create([this]() {
        QString statusText;
        const QString dirPath  = "C:/Program Files (x86)/Seewo/SeewoService";
        const QString filePath = dirPath + "/.testfile";
        QDir dir(dirPath);

        if (!dir.exists()) {
            statusText = QStringLiteral("❌希沃管家不存在，无法检测");
        } else {
            QFile file(filePath);
            bool created = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
            if (!created) {
                statusText = QStringLiteral("❌无法创建测试文件（可能权限不足）");
            } else {
                file.close();
                QFile::remove(filePath);
                bool isProtected = QFile::exists(filePath);
                statusText = isProtected ? QStringLiteral("✔️文件保护已开启")
                                         : QStringLiteral("✔️文件保护已关闭");
            }
        }
        QMetaObject::invokeMethod(this, [this, statusText]() {
            if (m_isEnableLabel && m_checkTimer) {
                m_isEnableLabel->setText(statusText);
                m_checkTimer->start(500);
            }
        }, Qt::QueuedConnection);
    });

    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();
}
