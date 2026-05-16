/**
 * @file lightwidget.h
 * @brief Header file for the LightWidget class.
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
#ifndef LIGHTWIDGET_H
#define LIGHTWIDGET_H
#include "WECore/we/webase.h"
#include "WECore/plugin/wpluginmessage.h"
#include "WECore/plugin/wpluginmanager.h"

#include <QMap>
#include <QString>

class LightWidgetPrivate;
namespace we {
/**
 * @class LightWidget
 * @brief Main class for the lightweight widget system.
 */
class LightWidget : public WEBase {
public:
    LightWidget();
    bool init();
    bool initMainPlugin(QStringList params, QString defualtWidget);
    void initAllPlugin();
    bool initPlugin(
        WPlugin *plugin, InitDataProc proc = [](WMessage &) {});
    void initData(QSharedPointer<WEBaseData> data);

private:
    LightWidgetPrivate *d = nullptr;
};
} // namespace we

#endif // LIGHTWIDGET_H