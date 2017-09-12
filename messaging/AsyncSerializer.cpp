#include <iostream>
#include "AsyncSerializer.h"

namespace Serialization { 
    AsyncSerializer::SerializeTask::SerializeTask(const std::future<void>& task)
        : task(task) { }

    AsyncSerializer::AsyncSerializer() {
        this->serializeTask = new SerializeTask(
            [serializer, serializeWork, serializeResults]() {
                SerializeWork work;
                SerializeResult result;

                while (true) {
                    std::cout << "Waiting. . ." << std::endl;
                    // Attempt to grab some work.
                    if (serializeWork.Pop(work)) {

                        // Serialize the work.
                        result.error = serializer.Serialize(work.msg, result.result);
                        result.msg = work.msg;

                        // Check if the original message needs to be freed.
                        if (work.deleteOnFinish)
                            delete work.msg; 

                        // Push the result to the results queue.
                        serializeResults.Push(result);

                    } else {
                        // No work to be done, sleep this thread handle for a some time.
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    } 
                }
            }
        );
    }

    AsyncSerializer::~AsyncSerializer() { 
        SerializeWork work;
        while (serializeWork.Pop(work)) {
            if (work.deleteOnFinish)
                delete work.msg;
        }
    }

    void AsyncSerializer::Serialize(const Messaging::IMsg* const msg, const bool deleteOnFinish) {
        SerializeWork work = { msg, deleteOnFinish };
        serializeWork.Push(work); 
    }

    bool GetSerializeResult(SerializeResult& result) {
        return serializeResults.Pop(result);
    }
}
