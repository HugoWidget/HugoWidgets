#ifndef HUGO_MAIN_WIDGET_H
#define HUGO_MAIN_WIDGET_H

#include "WECore/plugin/wplugindata.h"
#include "functionpagebase.h"
#include <QMainWindow>
#include <QMap>
#include <QSet>
#include <QVariantMap>

class QTabWidget;
class QMenuBar;

class HugoMainWidget : public QWidget {
    Q_OBJECT

public:
    explicit HugoMainWidget(we::WEBase *base = nullptr,
                            QWidget *parent = nullptr);
    ~HugoMainWidget();

    virtual void initWidget();

    bool executeCommand(const QString &functionId, const QString &command,
                        const QVariantMap &args = QVariantMap());

private slots:
    void onButtonClicked(const QString &functionId);
    void onClosePageRequested(const QString &functionId);
    void onTabCloseRequested(int tabIndex);

private:
    void createMenuBar();
    void createCentralWidget();
    void addHomePage();
    void createAllFunctionPages();
    FunctionPageBase *openFunctionPageById(const QString &functionId);

private:
    QMainWindow *mainWindow = nullptr;
    QTabWidget *tabWidget = nullptr;
    QMap<QString, FunctionPageBase *> m_allPagesById;
    QSet<QString> m_openPages;
    QMap<QString, QMetaObject::Connection> m_pageConnections;
    bool noGui = false;
};

#endif