#ifndef HUGO_MAIN_PLUGIN_H
#define HUGO_MAIN_PLUGIN_H

#include "WECore/plugin/wplugininterface.h"
#include <QObject>
#include <QtPlugin>

class HugoMainWidget;

class HugoMainPlugin : public QObject, public WPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID WPluginInterface_iid)
    Q_INTERFACES(WPluginInterface)

signals:
    void sendMsg(we::WMessage &);

public:
    HugoMainPlugin();
    ~HugoMainPlugin();

    bool init(we::WMessage &msg) override;
    void recMsg(we::WMessage &msg) override;
    bool deinit(we::WMessage &msg) override;

private:
    HugoMainWidget *widget = nullptr;
};

#endif // HUGO_MAIN_PLUGIN_H