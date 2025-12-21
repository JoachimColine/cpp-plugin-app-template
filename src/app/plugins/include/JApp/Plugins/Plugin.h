#pragma once
#include <QtPlugin>
#include <QString>
#include <JApp/Core/Context/Context.h>

#define JAPP_PLUGIN_IDD "japp.plugin.interface"

namespace JApp {

class Plugin
{
public:
    virtual ~Plugin() = default;
    virtual QString name() const = 0;
    virtual bool initialize(const JApp::Core::Context& context) = 0;
    virtual void shutdown() = 0;
};

}

Q_DECLARE_INTERFACE(JApp::Plugin, JAPP_PLUGIN_IDD)
