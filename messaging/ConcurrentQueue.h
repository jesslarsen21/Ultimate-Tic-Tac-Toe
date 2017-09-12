#pragma once

#include <iostream>
#include <atomic>


//<summary>
// This class acts as a lock-less concurrent stack -- To be used for multithreading puposes.
//</summary>
//<comments>
// Do not use with GCC prior to 4.8.3 (bug 60272), clang prior to 2014-05-05 (bug 18899)
// MSVC prior to 2014-03-17 (bug 819819). 
template<typename A>
class ConcurrentQueue {
    struct Node {
        A data;
        std::atomic<Node*> next;
    };

    std::atomic<Node*> _head;
    std::atomic<Node*> _tail;

public:
    ConcurrentQueue()
        : _head(new Node()),
          _tail(_head.load(std::memory_order_relaxed)) {
        Node* front = _head.load(std::memory_order_relaxed);
        front->next.store(NULL, std::memory_order_relaxed);
    }

    void Push(const A& data) {
        Node* node = new Node();

        node->data = data;
        node->next.store(NULL, std::memory_order_relaxed);

        Node* prevHead = _head.exchange(node, std::memory_order_acq_rel);
        prevHead->next.store(node, std::memory_order_relaxed);

        /*
        // Only atomicy, reordering is fine.
        newNode->next = head.load(std::memory_order_relaxed); 
        // next will be the current head of the stack.

        // Threads will contest until compareAndSwap is successful. 
        // `&head` - The destination of the exchange.
        // `&newNode->next` - What is expected as a result of the compare.
        // `newNode` - What to copy into `head`.
        while(! std::atomic_compare_exchange_weak_explicit(
                &head, 
                &newNode->next,
                newNode,
                std::memory_order_release,
                std::memory_order_relaxed 
                )) ;
        */
    }

    bool Pop(A& data) {
        Node* tail = _tail.load(std::memory_order_relaxed);
        Node* next = tail->next.load(std::memory_order_acquire);

        if (!next) return false;

        data = next->data;
        _tail.store(next, std::memory_order_release);
        delete tail;
        return true;
    }

    /*
    bool Pop(A& data) {
        Node* cur = head.load(std::memory_order_relaxed);
        if (!cur) return false;

        while(! std::atomic_compare_exchange_weak_explicit(
                &head,
                &cur,
                cur->next,
                std::memory_order_release,
                std::memory_order_relaxed
                )) ;

        data = A(cur->data);
        return true;
    } */
};
