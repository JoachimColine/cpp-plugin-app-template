#pragma once

#include <JApp/Core/Models/LogModel.h>

namespace JApp::Core::Models {

// TODO: move to dedicated test directory
class FakeLogModel : public LogModel
{

public:
    explicit FakeLogModel(QObject *parent = nullptr);
    ~FakeLogModel();

private:
    QTimer* m_timer;
};

}
