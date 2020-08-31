// from Anthony Williams Concurrency Book -- Boost License
#pragma once

namespace qms
{
    struct message_base
    {
        virtual ~message_base()
        {
        }
    };

    template <typename Msg>
    struct wrapped_message : message_base
    {
        Msg contents;
        explicit wrapped_message (Msg const&& contents_) :
            contents (std::move (contents_))
        {
        }
    };

    class queue
    {
        std::mutex m;
        std::condition_variable c;
        std::deque<std::shared_ptr<message_base>> q;
        QmsID lastID = QmsID::Invalid;

     public:
        template <typename T>
        void push (T const& msg)
        {
            std::lock_guard<std::mutex> lk (m);

            // if the incoming message is the same as the last message then replace the
            // last message with the new one. We only want to do this with certain message types
            // like RenderNextFrame that don't have any important state
			if (q.size() && msg.id == QmsID::RenderNextFrame && lastID == QmsID::RenderNextFrame)
            {
                //LOG(DBUG) << "Popping RenderNextFrame";
                q.pop_back();
            }
           
            // priority messages go to the front
            if (msg.id == QmsID::TopPriority)
            {
                q.push_front (std::make_shared<wrapped_message<T>> (std::move (msg)));
            }
            else
            {
                q.push_back (std::make_shared<wrapped_message<T>> (std::move (msg)));
            }

            lastID = msg.id;

            c.notify_all();
        }

        std::shared_ptr<message_base> wait_and_pop()
        {
            std::unique_lock<std::mutex> lk (m);
            c.wait (lk, [&] { return !q.empty(); });
            auto res = q.front();
            q.pop_front();
            return res;
        }

        std::shared_ptr<message_base> wait_and_pop_back()
        {
            std::unique_lock<std::mutex> lk (m);
            c.wait (lk, [&] { return !q.empty(); });
            auto res = q.back();
            q.pop_back();
            return res;
        }

        void clear()
        {
            std::unique_lock<std::mutex> lk (m);
            if (q.size())
            {
                //LOG(DBUG) << "Clearing QUEUE";
                q.clear();
                c.notify_all();
            }
        }
        size_t size()
        {
            return q.size();
        }
    };
} // namespace qms
