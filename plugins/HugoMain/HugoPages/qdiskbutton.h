#ifndef QDISKBUTTON_H
#define QDISKBUTTON_H

#include <QCheckBox>
#include <QColor>

#include "HugoUtils/HFreezeInterface.h"

class QDiskButton : public QCheckBox {
    Q_OBJECT

public:
    explicit QDiskButton(QWidget *parent = nullptr);
    void applyChanges();
    void setDriveInfo(const QString &drive, long long total, long long available);
    void setStatus(DriveFreezeState status);
    wchar_t getDriveLetter();

protected:
    void paintEvent(QPaintEvent *event) override;
    bool hitButton(const QPoint &pos) const override;

private:
    void init();
    int m_padding = 2;
    int m_borderWidth = 4;
    QColor m_colorUnchecked = Qt::gray;
    QColor m_colorChecked = QColor(41, 128, 185);
    QColor m_bgColor = Qt::white;
    QColor m_textColor = Qt::black;
    int m_fixedWidth = 200;
    int m_fixedHeight = 100;
    QString m_driveLetter = "C";
    long long m_totalSpace = 0;
    long long m_availableSpace = 0;
    DriveFreezeState m_status = DriveFreezeState::Unknown;
};

#endif // QDISKBUTTON_H
