/**
 * @file main.cpp
 * @brief 程序入口文件，实现了程序的初始化
 * @author howdy213
 * @date 2026-2-1
 * @version 1.2.0
 *
 * Copyright 2025-2026 howdy213
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

#include "WECore/file/wpath.h"
#include "WECore/metadata/WMetaDocument.h"
#include "WECore/we/we.h"

#include <QApplication>
#include <QLockFile>
#include <QMenu>
#include <QMessageBox>
#include <QStyleFactory>
#include <QSystemTrayIcon>
#include "Qlementine/lib/include/oclero/qlementine.hpp"
using namespace we::Consts;
using namespace we;

int main(int argc, char *argv[]) {
    QLockFile lockfile(WPath().getModuleFolder() + "we.lock");
    if (!lockfile.tryLock(0))
        return 1;

    auto lwptr = new LightWidget;
    if (!WE::init(lwptr))
        return 0;
    lwptr->init();
    lwptr->getWEClass()->configManager()->load(
        WPath().getModuleFolder() + Config::ConfigPath, true);
    auto config = lwptr->getWEClass()->configManager();

    if (qEnvironmentVariableIsEmpty("QT_FONT_DPI")) {
        if (config->hasArg(Config::Font))
            qputenv("QT_FONT_DPI",
                    qvariant_cast<QByteArray>(
                        lwptr->getWEClass()->configManager()->get(Config::Font)));
        if (config->hasArg(Config::Scale))
            qputenv("QT_SCALE_FACTOR",
                    qvariant_cast<QByteArray>(
                        lwptr->getWEClass()->configManager()->get(Config::Scale)));
    }

    QApplication a(argc, argv);
    // auto* style=new oclero::qlementine::QlementineStyle(&a);
    // a.setStyle(style);
    a.setWindowIcon(QIcon(":/icons/icon/we.png"));
    // a.setQuitOnLastWindowClosed(false);

    QStringList params;
    for (int i = 1; i <= argc - 1; i++)
        params.push_back(argv[i]);
    QString defaultMain = "";
    if (config->hasArg(Config::DefaultMain))
        defaultMain = lwptr->getWEClass()
                          ->configManager()
                          ->get(Config::DefaultMain)
                          .toString();
    if (!lwptr->initMainPlugin(params, defaultMain)) {
        QMessageBox::information(0, "提示", "无可用主界面，将退出");
        return -1;
    }

    return a.exec();
}
