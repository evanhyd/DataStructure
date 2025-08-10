#pragma once
#include "flow_concurrent_queue.h"
#include "flow_thread_task.h"
#include "flow_vector.h"

#include <future>
#include <thread>

namespace flow {

  /// @brief A simple thread pool with fixed number of worker threads.
  /// All the threads poll tasks from a single queue.
  /// If all the threads are waiting, then the user must manually call runPendingTask() to resolve deadlock.
  template <typename ThreadSafeQueue = ConcurrentQueue<ThreadTask>>
  class SimpleThreadPool {
    flow::Vector<std::jthread> threads_;
    ThreadSafeQueue queue_;

  public:
    explicit SimpleThreadPool(std::size_t threadCount = std::thread::hardware_concurrency() - 1) {
      threads_.reserve(threadCount);
      for (std::size_t i = 0; i < threadCount; ++i) {
        threads_.emplaceBack(std::bind(&SimpleThreadPool::runTasks, this, std::placeholders::_1));
      }
    }

    /// @return The number of worker threads. 
    std::size_t poolSize() const {
      return threads_.size();
    }

    /// @brief Submit a task to the thread pool queue.
    /// @param callable The callable.
    /// @param ...args  The callable arguments.
    template <typename Callable, typename ...Args>
    std::future<std::invoke_result_t<Callable, Args&&...>> submit(Callable callable, Args&&... args) {
      using ReturnType = std::invoke_result_t<Callable, Args&&...>;
      std::packaged_task<ReturnType()> task(std::bind(std::move(callable), std::forward<Args>(args)...));
      auto result = task.get_future();
      queue_.push(ThreadTask(std::move(task)));
      return result;
    }

    /// @brief Try run a task from the pool queue.
    /// If the queue is empty, then it yields the thread.
    /// This helps resolves thread deadlock due to dependency waiting.
    void runPendingTask() {
      if (auto task = queue_.tryPop(); task) {
        task->execute();
      } else {
        std::this_thread::yield();
      }
    }

  private:
    void runTasks(std::stop_token token) {
      while (!token.stop_requested()) {
        runPendingTask();
      }
    }
  };
}
