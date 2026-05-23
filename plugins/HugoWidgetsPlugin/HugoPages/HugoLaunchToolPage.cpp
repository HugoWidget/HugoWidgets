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
#include "HugoLaunchToolPage.h"
#include "ui_HugoLaunchToolPage.h"

HugoLaunchToolPage::HugoLaunchToolPage(QWidget *parent)
    : FunctionPageBase(parent), ui(new Ui::HugoLaunchToolPage) {
    ui->setupUi(this);
    startProcessStatusMonitor();
}

QString HugoLaunchToolPage::id() const { return "hugo_launchtoolpage"; }

QString HugoLaunchToolPage::name() const { return "希沃启动工具"; }

HugoLaunchToolPage::~HugoLaunchToolPage() {
    stopMonitorThread();
    stopProcessStatusMonitor();
    delete ui;
}

bool HugoLaunchToolPage::handleCommand(const QString &command,
                                       const QVariantMap &args) {}

void HugoLaunchToolPage::on_m_btnStop_clicked() {
    if (m_kill.hStopEvent && m_kill.hThread)
        return;
    startMonitorThread();
}

void HugoLaunchToolPage::on_m_btnStart_clicked() {
    stopMonitorThread();
    WinUtils::TerminateProcessesByName(L"HugoLaunchTool.exe");
    WinUtils::WinSvcMgr(L"SeewoCoreService").Start();
}

void HugoLaunchToolPage::startMonitorThread() {
    std::vector<WinUtils::string_t> SeewoProc{};
    SeewoProc.push_back(L"SeewoCore.exe");
    SeewoProc.push_back(L"SeewoServiceAssistant.exe");
    SeewoProc.push_back(L"SeewoAbility.exe");
    SeewoProc.push_back(L"SeewoLauncherGuard.exe");
    m_kill = WinUtils::StartProcessMonitor(GetModuleHandle(NULL), SeewoProc, 200);
}

void HugoLaunchToolPage::stopMonitorThread() {
    if (m_kill.hStopEvent && m_kill.hThread) {
        WinUtils::StopProcessMonitor(m_kill);
        m_kill = {};
    }
}

void ProcessStatusChecker::start() {
    m_stop = false;
    while (!m_stop) {
        bool coreRunning = isProcessRunning(L"SeewoCore.exe");
        bool assistRunning = isProcessRunning(L"SeewoServiceAssistant.exe");
        emit coreStatusChanged(coreRunning);
        emit assistStatusChanged(assistRunning);
        Sleep(500);
    }
    emit finished();
}

void ProcessStatusChecker::stop() { m_stop = true; }

bool ProcessStatusChecker::isProcessRunning(const std::wstring &processName) {
    return !WinUtils::GetProcessIdsByName(processName).empty();
}

void HugoLaunchToolPage::startProcessStatusMonitor() {
    if (m_statusThread && m_statusThread->isRunning())
        return;
    m_statusChecker = new ProcessStatusChecker;
    m_statusThread = new QThread(this);
    m_statusChecker->moveToThread(m_statusThread);
    connect(m_statusThread, &QThread::started, m_statusChecker,
            &ProcessStatusChecker::start);
    connect(m_statusThread, &QThread::finished, m_statusChecker,
            &QObject::deleteLater);
    connect(m_statusThread, &QThread::finished, m_statusThread,
            &QObject::deleteLater);
    connect(m_statusChecker, &ProcessStatusChecker::coreStatusChanged, this,
            &HugoLaunchToolPage::onCoreStatusChanged);
    connect(m_statusChecker, &ProcessStatusChecker::assistStatusChanged, this,
            &HugoLaunchToolPage::onAssistantStatusChanged);
    m_statusThread->start();
}

void HugoLaunchToolPage::stopProcessStatusMonitor() {
    if (!m_statusThread || !m_statusThread->isRunning())
        return;
    QMetaObject::invokeMethod(m_statusChecker, "stop", Qt::QueuedConnection);
    m_statusThread->quit();
    m_statusThread->wait(2000);
    if (m_statusThread->isRunning()) {
        m_statusThread->terminate();
        m_statusThread->wait();
    }
    m_statusThread = nullptr;
    m_statusChecker = nullptr;
}

void HugoLaunchToolPage::onCoreStatusChanged(bool running) {
    if (ui->m_isCoreRun)
        ui->m_isCoreRun->setText(QString("%1SeewoCore%2运行")
                                     .arg(running ? "✔️" : "❌")
                                     .arg(running ? "已" : "未"));
}

void HugoLaunchToolPage::onAssistantStatusChanged(bool running) {
    if (ui->m_isAssistantRun)
        ui->m_isAssistantRun->setText(QString("%1SeewoServiceAssistant%2运行")
                                          .arg(running ? "✔️" : "❌")
                                          .arg(running ? "已" : "未"));
}
