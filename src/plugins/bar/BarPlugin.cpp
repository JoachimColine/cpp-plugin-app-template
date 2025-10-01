#include "BarPlugin.h"
#include <JApp/Log.h>

using namespace Bar;

QString BarPlugin::name() const
{
    return "BarPlugin";
}

bool BarPlugin::initialize()
{
    LOG_DEBUG() << "BarPlugin initialized";
    return true;
}

void BarPlugin::shutdown()
{
    LOG_DEBUG() << "BarPlugin shutdown";
}
