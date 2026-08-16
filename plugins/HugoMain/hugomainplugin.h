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
#ifndef HUGO_MAIN_PLUGIN_H
#define HUGO_MAIN_PLUGIN_H

#include "WECore/plugin/wplugininterface.h"
#include <QObject>
#include <QtPlugin>

class HugoMainWidget;

class HugoMainPlugin : public QObject, public WPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID WPluginInterface_iid)
    Q_INTERFACES(WPluginInterface)

signals:
    void sendMsg(we::WMessage &);

public:
    HugoMainPlugin();
    ~HugoMainPlugin();

    bool init(we::WMessage &msg) override;
    void recMsg(we::WMessage &msg) override;
    bool deinit(we::WMessage &msg) override;

private:
    HugoMainWidget *widget = nullptr;
};

#endif // HUGO_MAIN_PLUGIN_H