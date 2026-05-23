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
#include "HugoPages/HugoPasswordPage.h"
#include "hugowidgetsplugin.h"

#include "HugoPages/HugoLaunchToolPage.h"
#include "HugoPages/HugoPasswordPage.h"
#include "HugoPages/HugoProtectPage.h"
#include "HugoPages/HugoFreezePage.h"
#include "HugoPages/HugoMountPage.h"

#include "WECore/plugin/wplugin.h"
#include "WECore/plugin/wplugindata.h"
#include "WECore/widget/wwidgetmanager.h"


#include <QWidget>
#include <Windows.h>

//using namespace LightWidget::Consts;
using namespace we::Consts;
using namespace we;

/**
 * @brief Constructs the HugoWidgetsPlugin object.
 */
HugoWidgetsPlugin::HugoWidgetsPlugin() {}

/**
 * @brief Destroys the HugoWidgetsPlugin object.
 */
HugoWidgetsPlugin::~HugoWidgetsPlugin() {}

/**
 * @brief Initializes the plugin.
 * @param msg Initialization message.
 * @return True if initialization succeeded, false otherwise.
 */
bool HugoWidgetsPlugin::init(WMessage &msg) {

    PluginData::setData(qvariant_cast<WEBase *>(msg.map[Data::WEBase]));
    PluginData::setPlugin(qvariant_cast<WPlugin *>(msg.map[Data::Plugin]));

    auto widgetManager = PClass->widgetManager();

    WPlugin *plugin = qvariant_cast<WPlugin *>(msg.map[Data::Plugin]);
    plugin->setMetaData(Plugin::Name, "HugoWidgets");
    plugin->setMetaData(Plugin::Author, "HugoWidget");

    auto createPage=[&](FunctionPageBase* base){
    WMessage msg2;
    msg2.object = base;
    WEvent extEvent(QString("hugo.hugowidget.hugowidgets.addpage"), msg2);
    widgetManager->publish(extEvent);
    };
    createPage(new HugoPasswordPage);
    createPage(new HugoFreezePage);
    createPage(new HugoLaunchToolPage);
    createPage(new HugoMountPage);
    createPage(new HugoProtectPage);

    return true;
}

/**
 * @brief Receives messages sent to the plugin.
 * @param msg The incoming message.
 */
void HugoWidgetsPlugin::recMsg(WMessage &msg) {
    Q_UNUSED(msg);
}

/**
 * @brief Deinitializes the plugin.
 * @param msg Deinitialization message.
 * @return True if deinitialization succeeded.
 */
bool HugoWidgetsPlugin::deinit(WMessage &msg) {
    Q_UNUSED(msg);
    return true;
}