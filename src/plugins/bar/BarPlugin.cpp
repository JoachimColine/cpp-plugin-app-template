#include "BarPlugin.h"
#include <JApp/Log.h>
#include <QThread>

using namespace Bar;

QString BarPlugin::name() const
{
    return "BarPlugin";
}

bool BarPlugin::initialize(const JApp::Core::Context& context)
{
    LOG_DEBUG() << "BarPlugin initializing...";
    QThread::sleep(1);
    LOG_DEBUG() << "BarPlugin initialized";
    return true;
}

void BarPlugin::shutdown()
{
    LOG_DEBUG() << "BarPlugin shutdown";
}
