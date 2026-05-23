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
