/**
 * @file lightsystem.cpp
 * @brief Implementation file for the lightweight plugin loading system.
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
#include "lightsystem.h"
#include "WECore/metadata/WMetaDocument.h"
#include "WECore/we/we.h"
#include "WECore/we/webase.h"
#include "WECore/file/wpath.h"
#include "WECore/plugin/wplugin.h"
#include "WECore/plugin/wpluginmanager.h"

#include <QCoreApplication>
#include <QDir>

using namespace we::Consts;
using namespace we;

/**
 * @class LightSystemPrivate
 * @brief Private implementation class for LightSystem.
 */
class LightSystemPrivate {
public:
    // Currently empty, reserved for future extensions
};

/**
 * @brief Constructs a LightSystem object.
 */
LightSystem::LightSystem() {
    d = new LightSystemPrivate;
}

/**
 * @brief Destroys the LightSystem object.
 */
LightSystem::~LightSystem() {
    delete d;
}

/**
 * @brief Loads all plugins defined in the configuration file.
 */
void LightSystem::loadAllPlugin() {
    QString plugins = WPath().getModuleFolder() + Plugins::ConfigPath;
    WMetaDocument doc;
    doc.load(plugins, true);
    auto map = doc.toMap();
    for (auto it = map.begin(); it != map.end(); ++it) {
        QJsonDocument doc2(it.value().toJsonObject());
        loadPlugin(plugins, doc2.toJson());
    }
}

/**
 * @brief Loads a plugin using its JSON configuration.
 * @param curPath Path to the directory containing the configuration.
 * @param config JSON configuration string.
 */
void LightSystem::loadPlugin(QString curPath, QString config) {
    WMetaDocument doc;
    doc.load(config, false);
    QString jsonPath = WPath().resolvePath(
        curPath, qvariant_cast<QString>(doc.get(Plugins::PluginConfigPath)));

    WMetaDocument objectDoc;
    objectDoc.load(jsonPath, true);

    for (int i = 1;; ++i) {
        QString num = QString::number(i);

        auto obj = objectDoc.get(num).toJsonObject();
        if (obj.isEmpty())
            break;
        QJsonDocument doc2(obj);
        WMetaDocument doc3;
        doc3.load(doc2.toJson(), false);

        loadSinglePlugin(jsonPath, &doc3);
    }
}

/**
 * @brief Loads a single plugin instance.
 * @param jsonPath Path to the plugin's JSON file.
 * @param doc Metadata document containing the plugin configuration.
 */
void LightSystem::loadSinglePlugin(QString jsonPath, WMetaDocument *doc) {
    if (!doc->hasArg(Plugin::Path))
        return;
    WPlugin *plugin = new WPlugin(WE::inst()->getWEClass()->pluginManager());
    plugin->readConfig(jsonPath, doc->toJsonString());
    WE::inst()->getWEClass()->pluginManager()->addPlugin(plugin);
}