#ifndef PIPELINE_H
#define PIPELINE_H

#include "../middleware/imiddleware.h"

class PipeLineBuilder
{
public:
    void addMiddleware(std::unique_ptr<IMiddleware> m);
    RequestDelegate build(RequestDelegate endpoint);
private:
    std::vector<std::unique_ptr<IMiddleware>> m_middlewares;
};

#endif // PIPELINE_H
