#include "FooPlugin.h"
#include <JApp/Log.h>

using namespace Foo;

QString FooPlugin::name() const
{
    return "FooPlugin";
}

bool FooPlugin::initialize()
{
    LOG_DEBUG() << "FooPlugin initialized";
    return true;
}

void FooPlugin::shutdown()
{
    LOG_DEBUG() << "FooPlugin shutdown";
}
