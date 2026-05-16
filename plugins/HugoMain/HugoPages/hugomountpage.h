#ifndef HUGOMOUNTPAGE_H
#define HUGOMOUNTPAGE_H

#include <QWidget>
#include <iomanip>
#include "functionpagebase.h"
#include"HugoUtils/HMount.h"
#include<QString>
namespace Ui {
class HugoMountPage;
}

class HugoMountPage : public FunctionPageBase
{
    Q_OBJECT

public:
    explicit HugoMountPage(QWidget *parent = nullptr);
    ~HugoMountPage();
    virtual void init() {}
    virtual void cleanup() {}

    virtual QString id() const ;
    virtual QString name() const ;

    virtual bool handleCommand(const QString& command, const QVariantMap& args = QVariantMap()) ;


private slots:
    void on_m_btnQuery_clicked();

    void on_m_btnMount_clicked();

    void on_pushButton_clicked();

    void on_m_btnUnmount_clicked();

private:
    Ui::HugoMountPage *ui;
    void queryAndRefresh();
};

#endif // HUGOMOUNTPAGE_H
