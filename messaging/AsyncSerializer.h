#pragma once

#include <future>
#include <vector>
#include <chrono>
#include "ConcurrentQueue.h"
#include "Serializer.h"
#include "IMsg.h"

namespace Serialization {
    class SerializeResult {
    public:
        std::string result;
        std::string error;
        const Messaging::IMsg* const msg;

        SerializeResult()
            : msg(nullptr) { }
    };

    struct SerializeWork {
        const Messaging::IMsg* const msg;
        const bool deletOnFinish;
    };

    class AsyncSerializer {
    private:
        struct SerializeTask {
            std::future<void> task;

            SerializeTask(const std::future<void>& task);
        };

        Serializer serializer;
        ConcurrentQueue<const SerializeWork> serializeWork;
        ConcurrentQueue<SerializeResult> serializeResults;

        SerializeTask* serializeTask;

    public:
        AsyncSerializer();
        ~AsyncSerializer();

        void Serialize(const Messaging::IMsg* const msg, const bool deleteOnFinish = true);
        bool GetSerializeResult(SerializeResult& result);
    };
};
