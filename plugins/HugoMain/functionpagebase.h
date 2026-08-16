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
#ifndef FUNCTIONPAGEBASE_H
#define FUNCTIONPAGEBASE_H

#include <QWidget>
#include <QString>
#include <QVariantMap>

class FunctionPageBase : public QWidget
{
    Q_OBJECT
public:
    explicit FunctionPageBase(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~FunctionPageBase() {}

    virtual void init() {}
    virtual void cleanup() {}

    virtual QString id() const = 0;
    virtual QString name() const = 0;

    virtual bool handleCommand(const QString& command, const QVariantMap& args = QVariantMap()) = 0;

    static void setNoGuiMode(bool mode) { s_noGuiMode = mode; }
    static bool noGuiMode() { return s_noGuiMode; }

signals:
    void closeRequested();

private:
    static inline bool s_noGuiMode = false;
};
Q_DECLARE_METATYPE(FunctionPageBase*)
#endif // FUNCTIONPAGEBASE_H