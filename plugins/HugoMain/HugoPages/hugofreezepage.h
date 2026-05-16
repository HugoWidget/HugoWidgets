#ifndef HUGOFREEZEPAGE_H
#define HUGOFREEZEPAGE_H

#include <QWidget>
#include <QStorageInfo>
#include<QList>
#include "HugoPages/HFreezeNone.h"
#include"QDiskButton.h"
#include "functionpagebase.h"
#include "HugoPages/hfreezeapiex.h"
#include "HugoUtils/hfreezedriver.h"
namespace Ui {
class HugoFreezePage;
}

class HugoFreezePage : public FunctionPageBase
{
    Q_OBJECT

public:
    explicit HugoFreezePage(QWidget *parent = nullptr);
    ~HugoFreezePage();
    virtual void init() {}
    virtual void cleanup() {}

    virtual QString id() const ;
    virtual QString name() const ;

    virtual bool handleCommand(const QString& command, const QVariantMap& args = QVariantMap()) ;
private slots:
    void onComboFreezeModeCurrentTextChanged(const QString &arg1);

    void on_m_btnRefresh_clicked();

    void on_m_btnSet_clicked();

private:
    void enableAllButtons(bool enable);
    IHugoFreeze* m_freeze=nullptr;
    static HFreezeApiEx* m_freezeApi;
    static HFreezeDriver* m_freezeDriver;
    static HFreezeNone* m_freezeNone;
    Ui::HugoFreezePage *ui;
    void refresh();
    static QList<QStorageInfo> diskList;
    QVector<QDiskButton*> m_diskBtns;
};

#endif // HUGOFREEZEPAGE_H
