/**
 * @file querymaindialog.cpp
 * @brief Implementation file for the main widget selection dialog.
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
#include "querymaindialog.h"

/**
 * @brief Constructs the selection dialog.
 * @param parent The parent widget.
 */
QueryMainDialog::QueryMainDialog(QWidget *parent)
    : QDialog(parent), m_btnGroup(new QButtonGroup(this)), m_btnId(0) {
    setWindowTitle("选择MainWidget");
    setMinimumSize(300, 200);
    setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *tipLabel = new QLabel("请选择优先启动的MainWidget：", this);
    mainLayout->addWidget(tipLabel);

    m_contentLayout = new QVBoxLayout();
    mainLayout->addLayout(m_contentLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *okBtn = new QPushButton("确定", this);
    QPushButton *cancelBtn = new QPushButton("取消", this);
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    btnLayout->addSpacing(10);
    btnLayout->addWidget(cancelBtn);
    mainLayout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    m_btnGroup->setExclusive(true);
}

/**
 * @brief Adds a main widget option to the dialog.
 * @param name The name of the widget to be displayed.
 */
void QueryMainDialog::addMainWidget(const QString &name) {
    if (name.isEmpty())
        return;

    QRadioButton *radioBtn = new QRadioButton(name, this);
    m_contentLayout->addWidget(radioBtn);
    m_btnGroup->addButton(radioBtn, m_btnId);
    radioBtn->setProperty("widgetName", name);
    m_btnId++;

    adjustSize();
}

/**
 * @brief Returns whether a widget has been selected.
 * @return True if a radio button is checked, false otherwise.
 */
bool QueryMainDialog::hasSelectedWidget() const {
    return m_btnGroup->checkedId() != -1;
}

/**
 * @brief Returns the name of the selected widget.
 * @return The widget name, or an empty string if none selected.
 */
QString QueryMainDialog::selectedWidgetName() const {
    QAbstractButton *checkedBtn = m_btnGroup->checkedButton();
    if (!checkedBtn) {
        return "";
    }
    return checkedBtn->property("widgetName").toString();
}