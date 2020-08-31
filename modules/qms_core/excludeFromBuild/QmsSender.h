// from Anthony Williams Concurrency Book -- Boost License
#pragma once

namespace qms
{
    class sender
    {
        queue*q;
    public:
        sender():
            q(nullptr)
        {}
        explicit sender(queue*q_):
            q(q_)
        {}
        template<typename Message>
        void send(Message const& msg)
        {
			if (msg.id == QmsID::ClearQueue)
			{
				q->clear();
			}

            if(q)
            {
                q->push(msg);
            }
        }
    };
}
