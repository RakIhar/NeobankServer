#include "pipelinebuilder.h"

void PipeLineBuilder::addMiddleware(std::unique_ptr<IMiddleware> m)
{
    m_middlewares.push_back(std::move(m));
}

RequestDelegate PipeLineBuilder::build(RequestDelegate endpoint)
{
    RequestDelegate next = std::move(endpoint);

    for (int i = m_middlewares.size() - 1; i >= 0; --i)
    {
        IMiddleware* m = m_middlewares[i].get();
        RequestDelegate current = next;

        next = [m, current](MessageContext& ctx){
            m->invoke(ctx, current);
        };
    }

    return next;
}
