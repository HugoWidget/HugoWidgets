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
#ifndef HUGOMAINWIDGET_H
#define HUGOMAINWIDGET_H

#include "WECore/we/webase.h"
#include <QMainWindow>
#include <QMap>
#include <QSet>
#include <QVariantMap>

#include "functionpagebase.h"

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
    FunctionPageBase *openFunctionPageById(const QString &functionId);

private:
    QMainWindow *mainWindow = nullptr;
    QTabWidget *tabWidget = nullptr;
    QMap<QString, FunctionPageBase *> m_allPagesById;
    QSet<QString> m_openPages;
    QMap<QString, QMetaObject::Connection> m_pageConnections;
    bool noGui = false;
};

#endif // HUGOMAINWIDGET_H