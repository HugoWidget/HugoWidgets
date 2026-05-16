#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QButtonGroup>
#include <QGridLayout>
#include <QStackedWidget>
#include <QToolButton>
#include <QVector>
#include <QWidget>

class SideBar : public QWidget {
    Q_OBJECT

public:
    explicit SideBar(QWidget *parent = nullptr);
    void setBtnCount(unsigned int count);
    void setButtonContent(unsigned int index, QString content);
    void connectStack(QStackedWidget *stackedWidget);
    ~SideBar();
private slots:
    void onButtonClicked();

private:
    QStackedWidget *stack = nullptr;
    QButtonGroup btnGroup;
};

#endif // SIDEBAR_H
