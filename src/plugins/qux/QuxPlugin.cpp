#include "QuxPlugin.h"
#include <JApp/Log.h>

using namespace Qux;

QString QuxPlugin::name() const
{
    return "QuxPlugin";
}

bool QuxPlugin::initialize()
{
    LOG_DEBUG() << "QuxPlugin initialized";
    return true;
}

void QuxPlugin::shutdown()
{
    LOG_DEBUG() << "QuxPlugin shutdown";
}
