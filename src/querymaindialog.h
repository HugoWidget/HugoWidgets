/**
 * @file querymaindialog.h
 * @brief Header file for the dialog that queries and selects the main widget.
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
#ifndef QUERYMAINDIALOG_H
#define QUERYMAINDIALOG_H
#include <QButtonGroup>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

/**
 * @class QueryMainDialog
 * @brief Dialog for selecting a main widget from a list of available widgets.
 */
class QueryMainDialog : public QDialog {
    Q_OBJECT
public:
    explicit QueryMainDialog(QWidget *parent = nullptr);
    void addMainWidget(const QString &name);
    bool hasSelectedWidget() const;
    QString selectedWidgetName() const;

private:
    QButtonGroup *m_btnGroup;      ///< Group of radio buttons for widget selection.
    QVBoxLayout *m_contentLayout;  ///< Layout that holds the radio buttons.
    int m_btnId;                   ///< Next ID to assign to a radio button.
};

#endif // QUERYMAINDIALOG_H