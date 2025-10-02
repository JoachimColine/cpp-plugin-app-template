#include "BazPlugin.h"
#include <JApp/Log.h>

using namespace Baz;

QString BazPlugin::name() const
{
    return "BazPlugin";
}

bool BazPlugin::initialize()
{
    LOG_DEBUG() << "BazPlugin initialized";
    return true;
}

void BazPlugin::shutdown()
{
    LOG_DEBUG() << "BazPlugin shutdown";
}
