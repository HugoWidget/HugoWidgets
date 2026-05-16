/**
 * @file lightwidget.cpp
 * @brief Implementation file for the LightWidget class.
 * @author howdy213
 * @date 2026-05-04
 * @version 2.0.0
 *
 * @copyright Copyright 2025-2026 howdy213
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "lightwidget.h"
#include "WECore/service/wserviceregistry.h"
#include "lightsystem.h"
#include "querymaindialog.h"

#include "WECore/metadata/WMetaDocument.h"
#include "WECore/we/we.h"
#include "WECore/file/wpath.h"
#include "WECore/plugin/wplugin.h"
#include "WECore/widget/wwidgetmanager.h"

#include <QMessageBox>

using namespace we::Consts;
using namespace we;

/**
 * @class LightWidgetPrivate
 * @brief Private implementation class for LightWidget.
 */
class LightWidgetPrivate {
public:
    LightSystem *sys = nullptr;
};

/**
 * @brief Initializes all plugins.
 */
void LightWidget::initAllPlugin() {
    auto manager = WE::inst()->getWEClass()->pluginManager();
    auto plugins = manager->allPluginsInst();
    foreach (auto plugin, plugins) {
        manager->initPlugin(plugin);
    }
}

/**
 * @brief Initializes the shared data container for the widget system.
 * @param data Shared pointer to the data container.
 */
void LightWidget::initData(QSharedPointer<WEBaseData> data) {
    QVariant config;
    config.setValue(new WMetaDocument);
    data->addData(Public::Config, config);

    QVariant path;
    path.setValue(new WPath);
    data->addData(Public::Path, path);

    QVariant pluginManager;
    pluginManager.setValue(new WPluginManager);
    data->addData(Public::PManager, pluginManager);

    QVariant widgetManager;
    widgetManager.setValue(new WWidgetManager(this));
    data->addData(Public::WManager, widgetManager);

    QVariant serviceReg;
    serviceReg.setValue(new WServiceRegistry);
    data->addData(Public::ServiceRegistry, serviceReg);
}

/**
 * @brief Constructs a LightWidget object.
 */
LightWidget::LightWidget() : WEBase() {
    d = new LightWidgetPrivate;
    d->sys = new LightSystem;
}

/**
 * @brief Initializes the LightWidget system.
 * @return True if initialization succeeded, false otherwise.
 */
bool LightWidget::init() {
    auto data = WEBase::getWEBaseData();
    initData(data);
    d->sys->loadAllPlugin();
    return true;
}

/**
 * @brief Initializes the main plugin.
 * @param params Command-line parameters.
 * @param defualtWidget Path to the default widget plugin.
 * @return True if the main plugin was successfully initialized.
 */
bool LightWidget::initMainPlugin(QStringList params, QString defualtWidget) {
    auto data = WEBase::getWEBaseData();
    auto manager = data->getData<WPluginManager *>(Public::PManager);
    auto ret = manager->getPluginByAttr(Plugin::MainWidget, true);
    QUuid id;
    if (ret.length() >= 2) {
        auto widget = manager->getPluginByAttr(Plugin::Path, defualtWidget);
        if (!widget.empty()) {
            id = widget[0];
        } else {
            QueryMainDialog dlg;
            foreach (id, ret) {
                auto plugin = manager->getPluginById(id);
                dlg.addMainWidget(plugin->getMetaData(Plugin::Path).toString());
            }
            if (dlg.exec() != QDialog::Accepted)
                return false;
            auto plugin =
                manager->getPluginByAttr(Plugin::Path, dlg.selectedWidgetName());
            id = plugin[0];
        }
    }
    if (ret.length() == 1)
        id = ret[0];
    if (id.isNull()) {
        return false;
    }
    WPlugin *plugin = manager->getPluginById(id);
    if (!manager->loadPlugin(plugin))
        return false;
    return manager->initPlugin(plugin, [&params](WMessage &data) {
        data.map.insert(Data::Params, params);
    });
}