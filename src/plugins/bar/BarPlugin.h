#pragma once

#include "JApp/Plugins/Plugin.h"
#include <QObject>

namespace Bar {

class BarPlugin : public QObject, public JApp::Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "japp.plugin.interface")
    Q_INTERFACES(JApp::Plugin)

public:
    ~BarPlugin() override = default;

    QString name() const override;
    bool initialize() override;
    void shutdown() override;
};

}
