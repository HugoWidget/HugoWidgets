#include "sidebar.h"
SideBar::SideBar(QWidget *parent) :
    QWidget(parent)
{

}
void SideBar::setBtnCount(unsigned int count){
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    gridLayout = new QGridLayout(this);
    gridLayout->setVerticalSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, -1);
    for(unsigned int i=0;i<=count-1;i++){
        QToolButton* btn=new QToolButton(this);
        btn->setStyleSheet(QString::fromUtf8("/* \351\273\230\350\256\244 */\n"
                                             "QToolButton{   \n"
                                             "	font: 25 11pt \"Microsoft YaHei\";\n"
                                             "	color:rgb(120, 120, 120);\n"
                                             "	border-top: 3px outset transparent;\n"
                                             "	border-bottom: 7px outset transparent;\n"
                                             "	border-right: 3px outset transparent;\n"
                                             "	border-left: 3px outset transparent;\n"
                                             "    min-width: 70px;\n"
                                             "    min-height: 50px;\n"
                                             "	background-color:rgb(240,240,240);\n"
                                             "}\n"
                                             "\n"
                                             "/* \351\274\240\346\240\207\346\202\254\345\201\234 */\n"
                                             "QToolButton:hover{\n"
                                             "	background-color:rgb(225, 225, 225) ;\n"
                                             "}\n"
                                             "\n"
                                             "/* \347\202\271\345\207\273\345\222\214\346\214\211\344\270\213 */\n"
                                             "QToolButton:pressed,QToolButton:checked{\n"
                                             "	border-left: 2px outset rgb(100, 100, 100);\n"
                                             "	background-color: rgb(210, 210, 210);\n"
                                             "	color:rgb(0, 120, 240);	\n"
                                             "}"));
        btn->setCheckable(true);
        btn->setAutoExclusive(true);
        gridLayout->addWidget(btn, i, 0, 1, 1);
        btnGroup.addButton(btn,i);
    }
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

    gridLayout->addItem(verticalSpacer, count, 0, 1, 1);
    btnGroup.button(0)->setChecked(true);
    QPalette pal(this->palette());
    pal.setColor(QPalette::Window, QColor(240, 240, 240));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->setFixedWidth(70);
    connect(&btnGroup, &QButtonGroup::buttonClicked, this, &SideBar::onButtonClicked);

}
SideBar::~SideBar()
{
}

void SideBar::connectStack(QStackedWidget* stackedWidget){
    stack=stackedWidget;
}
void SideBar::onButtonClicked(){
    stack->setCurrentIndex(btnGroup.checkedId());
}
void SideBar::setButtonContent(unsigned int index,QString content){
    if(index<=btnGroup.buttons().length()-1){
        btnGroup.button(index)->setText(content);
    }
}
