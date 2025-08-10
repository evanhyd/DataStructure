#pragma once
#include "flow_concurrent_queue.h"
#include "flow_thread_task.h"
#include "flow_vector.h"
#include "flow_work_stealing_queue.h"

#include <future>
#include <thread>

namespace flow {

  /// @brief A work-stealing multiqueue threadpool.
  /// Each worker thread has a thread_local task queue to reduce thread contention.
  template <typename ThreadSafeQueue = ConcurrentQueue<ThreadTask>, typename LocalQueue = WorkStealingQueue<ThreadTask>>
  class MultiQueueThreadPool {
    flow::Vector<std::jthread> threads_;
    flow::Vector<LocalQueue> localQueues_;
    ThreadSafeQueue poolQueue_;

    inline static thread_local std::size_t threadId = 0;
    inline static thread_local LocalQueue* localQueue = nullptr;

  public:
    explicit MultiQueueThreadPool(std::size_t threadCount = std::thread::hardware_concurrency() - 1)
      : localQueues_(threadCount) {

      threads_.reserve(threadCount);
      for (std::size_t i = 0; i < threadCount; ++i) {
        threads_.emplaceBack(std::bind(&MultiQueueThreadPool::runTasks, this, std::placeholders::_1, i));
      }
    }

    /// @return The number of worker threads. 
    std::size_t poolSize() const {
      return threads_.size();
    }

    /// @brief Submit a task to the thread-local queue.
    /// If the task is submit by an external thread outside of the thread pool,
    /// then it submits to the pool queue shared by all threads instead.
    /// @param callable The callable.
    /// @param ...args  The callable arguments.
    template <typename Callable, typename ...Args>
    std::future<std::invoke_result_t<Callable, Args&&...>> submit(Callable callable, Args&&... args) {
      using ReturnType = std::invoke_result_t<Callable, Args&&...>;
      std::packaged_task<ReturnType()> task(std::bind(std::move(callable), std::forward<Args>(args)...));
      auto result = task.get_future();

      if (localQueue) {
        localQueue->push(ThreadTask(std::move(task)));
      } else {
        poolQueue_.push(ThreadTask(std::move(task)));
      }
      
      return result;
    }

    /// @brief Try run a task from the local queue.
    /// If the local queue is empty, it tries to run a task from the pool queue.
    /// If the pool queue is empty, it tries to steal the work from other thread's local queue.
    /// If all queues are empty, then it yields the thread.
    /// This helps resolves thread deadlock due to dependency waiting.
    void runPendingTask() {

      // Pop from local queue.
      if (localQueue) {
        if (auto task = localQueue->tryPop(); task) {
          task->execute();
          return;
        }
      }

      // Pop from the pool queue.
      if (auto task = poolQueue_.tryPop(); task) {
        task->execute();
        return;
      }

      // Steal from other queue.
      std::size_t threadCount = poolSize();
      for (std::size_t i = 0; i < threadCount; ++i) {
        std::size_t id = (threadId + 1 + i) % threadCount;
        if (auto task = localQueues_[id].trySteal(); task) {
          task->execute();
          return;
        }
      }

      std::this_thread::yield();
    }

  private:
    void runTasks(std::stop_token token, std::size_t id) {
      threadId = id;
      localQueue = &localQueues_[id];

      while (!token.stop_requested()) {
        runPendingTask();
      }
    }
  };

}
