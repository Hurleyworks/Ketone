// from Anthony Williams Concurrency Book -- Boost License
#pragma once

namespace qms
{
    class receiver
    {
        queue q;

    public:
        operator sender()
        {
            return sender(&q);
        }

        dispatcher wait()
        {
            return dispatcher(&q);
        }
    };
}
