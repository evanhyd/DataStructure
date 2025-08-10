#pragma once
#include <future>
#include <memory>

namespace flow {

  /// @brief A task that can be execute by a thread.
  /// Internally, it uses type erasure to store a std::packaged_task.
  /// The memory is managed by std::unique_ptr.
  class ThreadTask final {
    struct ThreadTaskInterface {
      virtual ~ThreadTaskInterface() = default;
      virtual void executeImp() = 0;
    };

    template <typename Callable>
    struct ThreadTaskImp final : public ThreadTaskInterface {
      Callable task;

      template<typename C>
      ThreadTaskImp(C&& callable) : task(std::forward<C>(callable)) {}

      virtual void executeImp() override {
        task();
      }
    };

    std::unique_ptr<ThreadTaskInterface> task_;

  public:
    template <typename FuncType>
    explicit ThreadTask(std::packaged_task<FuncType> task)
      : task_(std::make_unique<ThreadTaskImp<std::packaged_task<FuncType>>>(std::move(task))) {
    }

    ThreadTask(ThreadTask&&) = default;
    ThreadTask(const ThreadTask&) = delete;
    ThreadTask& operator=(ThreadTask&&) = default;
    ThreadTask& operator=(const ThreadTask&) = delete;
    ~ThreadTask() = default;

    /// @brief Execute this task.
    void execute() {
      task_->executeImp();
    }
  };
}
