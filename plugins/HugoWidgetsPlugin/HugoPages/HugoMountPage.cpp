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
#include "HugoUtils/HMount.h"
#include "WECore/plugin/wplugin.h"
#include "WECore/plugin/wplugindata.h"
#include "WinUtils/WinUtils.h"

#include "hugomountpage.h"
#include <QDir>
#include "ui_hugomountpage.h"
using namespace we;
using namespace WinUtils;
using namespace Consts;
using namespace std;
static auto g_hMount = new HMount;

HugoMountPage::HugoMountPage(QWidget *parent)
    : FunctionPageBase(parent), ui(new Ui::HugoMountPage) {
    ui->setupUi(this);
    queryAndRefresh();
}

HugoMountPage::~HugoMountPage() {
    delete ui;
    ui = nullptr;
}

QString HugoMountPage::id() const { return "hugo_mountpage"; }

QString HugoMountPage::name() const { return "SWAce希沃虚拟磁盘管理"; }

bool HugoMountPage::handleCommand(const QString &command,
                                  const QVariantMap &args) {}

void HugoMountPage::queryAndRefresh() {
    auto pThis = g_hMount;

    HugoMountInfo info = pThis->GetAllInfo();
    QString output;
    // Driver information
    if (!info.driver.binaryPath.empty()) {
        output += "Driver  : " + info.driver.binaryPath + "\n";
        output += "Version : " + info.driver.version += "\n";
        output += QString("Status  : ") +
                  QString(info.driver.isRunning ? "RUNNING" : "STOPPED") += "\n";
    } else {
        output += QString("Driver ") + " not found or access denied.\n";
    }

    // Disk count and per-disk details
    output += "Slots   : " + std::to_string(info.diskCount) + "\n\n";
    for (const auto &disk : info.disks) {
        output += "Disk " + std::to_string(disk.index) + "\n";
        output += "Capacity        : " + std::to_string(disk.capacitySectors) +
                  " sectors\n";
        output +=
            "Backing Files   : " + std::to_string(disk.backingFiles.size()) + "\n";
        for (size_t k = 0; k < disk.backingFiles.size(); ++k) {
            const auto &bf = disk.backingFiles[k];
            output += QString("  [") + QString(std::to_string(bf.size).c_str()) +
                      QString("] ") + QString(bf.path.c_str()) + QString("\n");
        }
        // Partition information
        {
            output += "      0              0    ";
            output += "  <disk>\n";
            // Print each partition
            for (const auto &p : disk.partitions) {
                output += "      " + QString(std::to_string(p.index).c_str()) +
                          QString("   ") + QString("            ") +
                          QString(std::to_string(p.startLBA).c_str()) + "    ";
                output += "  " + p.typeDescription + "\n";
            }
            output += "\n";
        }
    }
    ui->m_labelStatus->setText(output);
    ui->m_comboMount->clear();
    ui->m_comboUnmount->clear();
    for (const auto &disk : info.disks) {
        QString newdisk;
        newdisk += "Disk " + std::to_string(disk.index);
        for (const auto &p : disk.partitions) {
            QString newpartion;
            newpartion += " " + std::to_string(p.index) + " " + p.typeDescription;
            QString newthing = newdisk + newpartion;
            ui->m_comboMount->addItem(newthing);
            for (auto letter : g_hMount->FindMountedDrive(disk.index, p.index)) {
                ui->m_comboUnmount->addItem(QString(letter));
            }
        }
    }
}

void HugoMountPage::on_m_btnQuery_clicked() { queryAndRefresh(); }

void HugoMountPage::on_m_btnMount_clicked() {
    QString cuurentDisk = ui->m_comboMount->currentText();
    QString cuurentLetter = ui->m_editMountDrive->text().toUpper();
    if (cuurentLetter.size() != 1 || cuurentLetter[0] > 'Z' ||
        cuurentLetter[0] < 'A') {
        ui->m_labelMsg->setText("请输入一个英文字母的盘符");
        return;
    }
    if (cuurentDisk.contains("Disk")) {
        int diskid = cuurentDisk[5].digitValue();
        int partid = cuurentDisk[7].digitValue();
        if (!g_hMount->Mount(diskid, partid, cuurentLetter[0].toLatin1())) {
            ui->m_labelMsg->setText("挂载成功");
            queryAndRefresh();
        } else {
            ui->m_labelMsg->setText("挂载失败，可能是选择了系统已有的磁盘");
        }
    } else {
        ui->m_labelMsg->setText("未选择正确的载虚拟磁盘");
    }
}

void HugoMountPage::on_pushButton_clicked() {
    QDir path = PPlugin->getMetaData(Plugin::Path).toString();
    path.cdUp();
    QString filePath = path.absoluteFilePath("tools/PyExplorer.exe");
    RunExternalProgram(filePath.toStdWString());
}

void HugoMountPage::on_m_btnUnmount_clicked() {
    if (!ui->m_comboUnmount->currentText().isEmpty()) {
        g_hMount->Unmount(ui->m_comboUnmount->currentText().at(0).toLatin1());
        ui->m_labelMsg->setText("卸载成功");
    }
    queryAndRefresh();
}
