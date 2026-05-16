// hugomainwidget.cpp
#include "hugomainwidget.h"
#include "WECore/plugin/wplugin.h"
#include "WECore/plugin/wplugindata.h"

#include "HugoPages/HugoFreezePage.h"
#include "HugoPages/hugomountpage.h"

#include "Shared/flowlayout.h"
#include "WECore/widget/wwidgetmanager.h"
#include <QAction>
#include <QCoreApplication>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStyle>
#include <QTabBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>
using namespace we;
using namespace we::Consts;

HugoMainWidget::HugoMainWidget(WEBase *base, QWidget *parent)
    : QWidget(parent) {
    QStringList args = QCoreApplication::arguments();
    for (const QString &arg : std::as_const(args)) {
        if (arg == "--no-gui") {
            noGui = true;
            FunctionPageBase::setNoGuiMode(true);
            break;
        }
    }

    createAllFunctionPages();

    if (!noGui) {
        mainWindow = new QMainWindow();
        mainWindow->setWindowTitle("HugoWidgets");
        mainWindow->resize(800, 600);
        createMenuBar();
        createCentralWidget();
        mainWindow->show();
        connect(mainWindow, &QMainWindow::close, []() {
            QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
        });
    }
}

HugoMainWidget::~HugoMainWidget() {
    delete mainWindow;
    qDeleteAll(m_allPagesById);
}

void HugoMainWidget::initWidget() {
    PClass->widgetManager()->setAttr(this, Widget::Name, "HugoMainWidget");
}

void HugoMainWidget::createMenuBar() {
    QMenuBar *menuBar = new QMenuBar(mainWindow);
    mainWindow->setMenuBar(menuBar);

    QMenu *fileMenu = menuBar->addMenu("文件");
    QAction *exitAction = fileMenu->addAction("退出");
    connect(exitAction, &QAction::triggered, mainWindow, &QMainWindow::close);

    QMenu *helpMenu = menuBar->addMenu("帮助");
    QAction *aboutAction = helpMenu->addAction("关于");
    connect(aboutAction, &QAction::triggered,
            []() { QMessageBox::about(nullptr, "关于", "HugoMain Plugin"); });
}

void HugoMainWidget::createCentralWidget() {
    tabWidget = new QTabWidget(mainWindow);
    tabWidget->tabBar()->setMovable(true);
    tabWidget->setTabsClosable(true);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this,
            &HugoMainWidget::onTabCloseRequested);
    mainWindow->setCentralWidget(tabWidget);
    addHomePage();
}

void HugoMainWidget::addHomePage() {
    QWidget *homePage = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(homePage);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    QLabel *titleLabel = new QLabel("HugoWidgets");
    titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    QFont font = titleLabel->font();
    font.setPointSize(14);
    titleLabel->setFont(font);
    mainLayout->addWidget(titleLabel);

    // 按钮容器
    QWidget *buttonContainer = new QWidget();
    FlowLayout *flowLayout = new FlowLayout(buttonContainer, 10, 15, 10);
    flowLayout->setAlignment(Qt::AlignHCenter);
    buttonContainer->setLayout(flowLayout);

    for (auto it = m_allPagesById.begin(); it != m_allPagesById.end(); ++it) {
        FunctionPageBase *page = it.value();
        QPushButton *btn = new QPushButton(page->name());
        btn->setMaximumWidth(200);
        btn->setFixedHeight(50);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        connect(btn, &QPushButton::clicked,
                [this, id = page->id()]() { onButtonClicked(id); });
        flowLayout->addWidget(btn);
    }

    mainLayout->addWidget(buttonContainer);
    mainLayout->addStretch();

    tabWidget->addTab(homePage, "首页");
}

void HugoMainWidget::onButtonClicked(const QString &functionId) {
    if (m_openPages.contains(functionId)) {
        if (FunctionPageBase *page = m_allPagesById.value(functionId))
            tabWidget->setCurrentWidget(page);
        return;
    }
    openFunctionPageById(functionId);
}

void HugoMainWidget::onClosePageRequested(const QString &functionId) {
    if (noGui || !m_openPages.contains(functionId))
        return;

    FunctionPageBase *page = m_allPagesById.value(functionId);
    if (page && tabWidget) {
        int idx = tabWidget->indexOf(page);
        if (idx != -1) {
            tabWidget->removeTab(idx);
            m_openPages.remove(functionId);
            if (m_pageConnections.contains(functionId)) {
                disconnect(m_pageConnections[functionId]);
                m_pageConnections.remove(functionId);
            }
        }
    }
}

void HugoMainWidget::onTabCloseRequested(int tabIndex) {
    if (noGui || tabIndex == 0)
        return;

    QWidget *widget = tabWidget->widget(tabIndex);
    for (auto it = m_allPagesById.begin(); it != m_allPagesById.end(); ++it) {
        if (it.value() == widget) {
            onClosePageRequested(it.key());
            break;
        }
    }
}

void HugoMainWidget::createAllFunctionPages() {
    auto createPage = [this](FunctionPageBase *funcpage) {
        m_allPagesById[funcpage->id()] = funcpage;
        funcpage->init();
    };
    createPage(new HugoFreezePage);
    createPage(new HugoMountPage);
}

FunctionPageBase *
HugoMainWidget::openFunctionPageById(const QString &functionId) {
    if (!m_allPagesById.contains(functionId))
        return nullptr;

    FunctionPageBase *page = m_allPagesById[functionId];
    if (!page)
        return nullptr;

    if (!m_openPages.contains(functionId)) {
        if (!noGui && tabWidget) {
            tabWidget->addTab(page, page->name());
            m_openPages.insert(functionId);
            QMetaObject::Connection conn =
                connect(page, &FunctionPageBase::closeRequested, this,
                                                   [this, functionId]() { onClosePageRequested(functionId); });
            m_pageConnections[functionId] = conn;
            tabWidget->setCurrentWidget(page);
        } else {
            m_openPages.insert(functionId);
        }
    } else if (!noGui && tabWidget) {
        tabWidget->setCurrentWidget(page);
    }
    return page;
}

bool HugoMainWidget::executeCommand(const QString &functionId,
                                    const QString &command,
                                    const QVariantMap &args) {
    FunctionPageBase *page = openFunctionPageById(functionId);
    return page ? page->handleCommand(command, args) : false;
}
