#pragma once

#include "JApp/Plugins/Plugin.h"
#include <QObject>

namespace Qux {

class QuxPlugin : public QObject, public JApp::Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID JAPP_PLUGIN_IDD)
    Q_INTERFACES(JApp::Plugin)

public:
    ~QuxPlugin() override = default;

    QString name() const override;
    bool initialize() override;
    void shutdown() override;
};

}
