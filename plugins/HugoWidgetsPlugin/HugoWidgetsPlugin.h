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
#ifndef HUGOWIDGETSPLUGIN_H
#define HUGOWIDGETSPLUGIN_H

#include "WECore/plugin/wplugininterface.h"

#include <QObject>
#include <QtPlugin>

/**
 * @class HugoWidgetsPlugin
 * @brief An hugowidgets plugin class that demonstrates how to extend the application.
 */
class HugoWidgetsPlugin : public QObject, public WPluginInterface {
public:
    Q_OBJECT
    Q_PLUGIN_METADATA(IID WPluginInterface_iid)
    Q_INTERFACES(WPluginInterface)
public:
    HugoWidgetsPlugin();
    ~HugoWidgetsPlugin();
    bool init(we::WMessage &msg) override;
    void recMsg(we::WMessage &msg) override;
    virtual bool deinit(we::WMessage &msg) override;
};

#endif // HUGOWIDGETSPLUGIN_H