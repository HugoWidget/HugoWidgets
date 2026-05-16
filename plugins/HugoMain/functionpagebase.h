#ifndef FUNCTIONPAGEBASE_H
#define FUNCTIONPAGEBASE_H

#include <QWidget>
#include <QString>
#include <QVariantMap>

class FunctionPageBase : public QWidget
{
    Q_OBJECT
public:
    explicit FunctionPageBase(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~FunctionPageBase() {}

    virtual void init() {}
    virtual void cleanup() {}

    virtual QString id() const = 0;
    virtual QString name() const = 0;

    virtual bool handleCommand(const QString& command, const QVariantMap& args = QVariantMap()) = 0;

    static void setNoGuiMode(bool mode) { s_noGuiMode = mode; }
    static bool noGuiMode() { return s_noGuiMode; }

signals:
    void closeRequested();

private:
    static inline bool s_noGuiMode = false;
};

#endif // FUNCTIONPAGEBASE_H