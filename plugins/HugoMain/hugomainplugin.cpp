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
#include "hugomainplugin.h"
#include "hugomainwidget.h"

#include "WinUtils/Console.h"

#include "WECore/plugin/wplugin.h"
#include "WECore/plugin/wplugindata.h"
#include "WECore/widget/wwidgetmanager.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QUuid>
using namespace we;
using namespace we::Consts;

HugoMainPlugin::HugoMainPlugin() {}

HugoMainPlugin::~HugoMainPlugin() {}

bool HugoMainPlugin::init(we::WMessage &msg) {
    WinUtils::Console console;
    console.attach();
    PluginData::setData(qvariant_cast<WEBase *>(msg.map[Data::WEBase]));
    PluginData::setPlugin(qvariant_cast<WPlugin *>(msg.map[Data::Plugin]));

    auto widgetManager = PClass->widgetManager();
    widget = new HugoMainWidget(PData);
    widgetManager->addWidget(QUuid::createUuid(), widget, this);

    WPlugin *plugin = qvariant_cast<WPlugin *>(msg.map[Data::Plugin]);
    plugin->setMetaData(Plugin::Name, "HugoMain");

    // 解析命令行参数
    static bool cmdProcessed = false;
    if (!cmdProcessed && widget) {
        cmdProcessed = true;
        QStringList args = QCoreApplication::arguments();
        QString functionId;
        QString command;
        QVariantMap argsMap;
        bool noGui = false;

        for (int i = 1; i < args.size(); ++i) {
            if (args[i] == "--function" && i + 1 < args.size()) {
                functionId = args[++i];
            } else if (args[i] == "--command" && i + 1 < args.size()) {
                command = args[++i];
            } else if (args[i] == "--arg" && i + 1 < args.size()) {
                QString argStr = args[++i];
                int eqPos = argStr.indexOf('=');
                if (eqPos != -1) {
                    QString key = argStr.left(eqPos);
                    QString value = argStr.mid(eqPos + 1);
                    argsMap[key] = value;
                }
            } else if (args[i] == "--no-gui") {
                noGui = true;
            }
        }

        if (!functionId.isEmpty() && !command.isEmpty()) {
            widget->executeCommand(functionId, command, argsMap);
        }

        if (noGui) {
            QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
        }
    }

    return true;
}

void HugoMainPlugin::recMsg(we::WMessage &msg) {}

bool HugoMainPlugin::deinit(we::WMessage &msg) {
    Q_UNUSED(msg);
    return true;
}