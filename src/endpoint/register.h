#ifndef ENDPOINT_REGISTER_H
#define ENDPOINT_REGISTER_H

#include "iendpoint.h"
#include <QDebug>

namespace Endpoints {

class Register : public IEndpoint
{
public:
    Register(){};

    void invoke(MessageContext& ctx) override
    {
        qDebug() << "Register endpoint";
    };
};

}

#endif // REGISTER_H
