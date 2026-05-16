#include "hugofreezepage.h"
#include "HugoPages/HFreezeNone.h"
#include "HugoPages/hfreezeapiex.h"
#include "ui_hugofreezepage.h"
#include <QMessageBox>
#include <QPushButton>

HFreezeApiEx *HugoFreezePage::m_freezeApi = nullptr;
HFreezeDriver *HugoFreezePage::m_freezeDriver = nullptr;
HFreezeNone *HugoFreezePage::m_freezeNone = nullptr;

HugoFreezePage::HugoFreezePage(QWidget *parent)
    : FunctionPageBase(parent), ui(new Ui::HugoFreezePage) {
    ui->setupUi(this);
    ui->m_comboFreezeMode->addItems({"None", "Api", "Driver"});
    connect(ui->m_comboFreezeMode, &QComboBox::currentTextChanged, this,
            &HugoFreezePage::onComboFreezeModeCurrentTextChanged);

    const auto diskList = QStorageInfo::mountedVolumes();
    int validDiskCount = 0;
    for (const QStorageInfo &si : diskList) {
        const QString rootPath = si.rootPath();
        if (rootPath.isEmpty())
            continue;
        if (::GetDriveTypeA(rootPath.toStdString().c_str()) != DRIVE_FIXED)
            continue;

        const QChar driveLetterChar = rootPath.at(0).toUpper();
        const wchar_t driveLetter = driveLetterChar.toLatin1();

        auto *btn = new QDiskButton;
        m_diskBtns.append(btn);

        const qint64 totalMB = si.bytesTotal() / 1024 / 1024;
        const qint64 freeMB = si.bytesFree() / 1024 / 1024;
        btn->setDriveInfo(QString(driveLetterChar), totalMB, freeMB);
        btn->setStatus(DriveFreezeState::Unknown);

        connect(btn, &QDiskButton::checkStateChanged, this, [this]() {
            auto *clickedBtn = qobject_cast<QDiskButton *>(sender());
            if (!clickedBtn)
                return;

            const bool isChecked = clickedBtn->isChecked();
            const wchar_t letter = clickedBtn->getDriveLetter();

            if (letter == L'C') {
                if (!isChecked) {
                    clickedBtn->setChecked(true);
                }
                return;
            }
            if (isChecked) {
                for (auto *otherBtn : m_diskBtns) {
                    if (otherBtn->getDriveLetter() == L'C') {
                        if (!otherBtn->isChecked())
                            otherBtn->setChecked(true);
                        break;
                    }
                }
            }
        });
        ui->m_layoutDisks->addWidget(btn, validDiskCount / 3, validDiskCount % 3);
        ++validDiskCount;
    }
    const int rows = (validDiskCount + 2) / 3;
    ui->widget->setMinimumHeight(rows * 130);
}

HugoFreezePage::~HugoFreezePage() { delete ui; }

QString HugoFreezePage::id() const { return "hugo_freezepage"; }

QString HugoFreezePage::name() const { return "希沃冰点管理"; }

bool HugoFreezePage::handleCommand(const QString &command,
                                   const QVariantMap &args) {
    return 0;
}

void HugoFreezePage::onComboFreezeModeCurrentTextChanged(const QString &arg1) {
    if (m_freeze) {
        m_freeze->Cleanup();
        m_freeze = nullptr;
    }
    if (arg1 == "Api") {
        if (!m_freezeApi) {
            m_freezeApi = new HFreezeApiEx;
        }
        m_freeze = m_freezeApi;
    } else if (arg1 == "Driver") {
        if (!m_freezeDriver) {
            m_freezeDriver = &HFreezeDriver::Instance();
        }
        m_freeze = m_freezeDriver;
    } else if (arg1 == "None") {
        if (!m_freezeNone) {
            m_freezeNone = new HFreezeNone;
        }
        m_freeze = m_freezeNone;
    }
    if (m_freeze) {
        m_freeze->Init();
    }
}

void HugoFreezePage::refresh() {
    auto res = m_freeze->GetFreezeState();

    int i = 0;
    for (auto *btnDisk : std::as_const(m_diskBtns)) {
        wchar_t letter = btnDisk->getDriveLetter();
        if (res.diskInfos.find(letter) == res.diskInfos.end()) {
            btnDisk->hide();
            continue;
        }
        btnDisk->show();
        auto x = res.diskInfos[letter];
        btnDisk->setDriveInfo(QString(1, char(letter)), x.bytesTotal, x.bytesFree);
        btnDisk->setStatus(x.state);
        ui->m_layoutDisks->addWidget(btnDisk, i / 3, i % 3);
        i++;
    }
    ui->m_labelMessage->setText(QString::fromStdWString(res.msg));
}

void HugoFreezePage::on_m_btnRefresh_clicked() {
    ui->m_btnRefresh->setDisabled(true);
    refresh();
    ui->m_btnRefresh->setDisabled(false);
}

void HugoFreezePage::on_m_btnSet_clicked() {
    QStringList driveLetters;
    for (auto *btn : std::as_const(m_diskBtns)) {
        if (btn->isChecked())
            driveLetters.push_back(QString(1, (char)btn->getDriveLetter()));
    }
    enableAllButtons(false);
    ui->m_comboFreezeMode->setDisabled(true);
    auto res = m_freeze->SetFreezeState(driveLetters.join("").toStdWString());
    refresh();
    ui->m_labelMessage->setText(QString::fromStdWString(res.msg));
    enableAllButtons(true);
    ui->m_comboFreezeMode->setDisabled(false);
}

void HugoFreezePage::enableAllButtons(bool enable) {
    ui->m_btnRefresh->setEnabled(enable);
    ui->m_btnTry->setEnabled(enable);
    ui->m_btnSet->setEnabled(enable);
}
