#include <QtPlugin>
#include <QString>

namespace JApp {

class Plugin
{
public:
    virtual ~Plugin() = default;
    virtual QString name() const = 0;
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
};

}

Q_DECLARE_INTERFACE(JApp::Plugin, "japp.plugin.interface")
