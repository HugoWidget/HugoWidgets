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
#ifndef HUGOLAUNCHTOOLPAGE_H
#define HUGOLAUNCHTOOLPAGE_H

#include <QWidget>
#include"../HugoMain/functionpagebase.h"
#include <QThread>
#include"HugoUtils/include/WinUtils/WinUtils.h"
#include"HugoUtils/include/WinUtils/WinSvcMgr.h"

class ProcessStatusChecker;
namespace Ui {
class HugoLaunchToolPage;
}

class HugoLaunchToolPage : public FunctionPageBase
{
    Q_OBJECT

public:
    explicit HugoLaunchToolPage(QWidget *parent = nullptr);
    ~HugoLaunchToolPage();

    virtual void init() {}
    virtual void cleanup() {}

    virtual QString id() const;
    virtual QString name() const;

    virtual bool handleCommand(const QString& command, const QVariantMap& args = QVariantMap());

private slots:

    void on_m_btnStop_clicked();

    void on_m_btnStart_clicked();

    void onCoreStatusChanged(bool running);
    void onAssistantStatusChanged(bool running);

private:
    Ui::HugoLaunchToolPage *ui;
    void startMonitorThread();
    void stopMonitorThread();
    void startProcessStatusMonitor();
    void stopProcessStatusMonitor();
    WinUtils::MonitorHandle m_kill={};
    WinUtils::MonitorHandle m_empty={};
    QThread* m_statusThread = nullptr;
    ProcessStatusChecker* m_statusChecker = nullptr;
};


class ProcessStatusChecker : public QObject
{
    Q_OBJECT
public:
    explicit ProcessStatusChecker(QObject *parent = nullptr)
        : QObject(parent), m_stop(true) {}
public slots:
    void start();
    void stop();
signals:
    void coreStatusChanged(bool running);
    void assistStatusChanged(bool running);
    void finished();
private:
    bool isProcessRunning(const std::wstring &processName);
    std::atomic<bool> m_stop;
};
#endif // HUGOLAUNCHTOOLPAGE_H
