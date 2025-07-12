#include "../DataStructure/flow_concurrent_queue.h"
#include <gtest/gtest.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

namespace flow {

  class ConcurrentQueueTest : public ::testing::Test {
  };

  TEST_F(ConcurrentQueueTest, DefaultConstructorAndEmpty) {
    ConcurrentQueue<int> q;
    EXPECT_TRUE(q.empty());
    EXPECT_EQ(q.size(), 0);
  }

  TEST_F(ConcurrentQueueTest, PushAndSize) {
    ConcurrentQueue<int> q;
    q.push(1);
    EXPECT_FALSE(q.empty());
    EXPECT_EQ(q.size(), 1);
    q.push(2);
    EXPECT_EQ(q.size(), 2);
  }

  TEST_F(ConcurrentQueueTest, TryFront) {
    ConcurrentQueue<int> q;
    EXPECT_FALSE(q.tryFront().has_value());

    q.push(10);
    std::optional<int> front_val = q.tryFront();
    EXPECT_TRUE(front_val.has_value());
    EXPECT_EQ(*front_val, 10);
    EXPECT_EQ(q.size(), 1);

    q.push(20);
    front_val = q.tryFront();
    EXPECT_TRUE(front_val.has_value());
    EXPECT_EQ(*front_val, 10);
    EXPECT_EQ(q.size(), 2);
  }

  TEST_F(ConcurrentQueueTest, TryPop) {
    ConcurrentQueue<int> q;
    EXPECT_FALSE(q.tryPop().has_value());

    q.push(5);
    q.push(10);

    std::optional<int> val1 = q.tryPop();
    EXPECT_TRUE(val1.has_value());
    EXPECT_EQ(*val1, 5);
    EXPECT_EQ(q.size(), 1);

    std::optional<int> val2 = q.tryPop();
    EXPECT_TRUE(val2.has_value());
    EXPECT_EQ(*val2, 10);
    EXPECT_EQ(q.size(), 0);
    EXPECT_TRUE(q.empty());

    EXPECT_FALSE(q.tryPop().has_value());
  }

  TEST_F(ConcurrentQueueTest, Emplace) {
    ConcurrentQueue<std::pair<int, double>> q;
    q.emplace(1, 1.1);
    EXPECT_EQ(q.size(), 1);
    std::optional<std::pair<int, double>> val = q.tryPop();
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(val->first, 1);
    EXPECT_EQ(val->second, 1.1);

    q.emplace(2, 2.2);
    q.emplace(3, 3.3);
    EXPECT_EQ(q.size(), 2);
    val = q.tryPop();
    EXPECT_TRUE(val.has_value());
    EXPECT_EQ(val->first, 2);
    EXPECT_EQ(val->second, 2.2);
  }

  TEST_F(ConcurrentQueueTest, WaitPopBasic) {
    ConcurrentQueue<int> q;

    std::thread producer([&]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      q.push(42);
    });

    int value = q.waitPop();
    EXPECT_EQ(value, 42);
    EXPECT_TRUE(q.empty());

    producer.join();
  }

  // --- Concurrency Tests ---
  TEST_F(ConcurrentQueueTest, MultipleProducers) {
    constexpr int kProducerNum = 5;
    constexpr int kItemsPerProducer = 100000;

    ConcurrentQueue<int> q;

    std::vector<std::thread> producers;
    for (int i = 0; i < kProducerNum; ++i) {
      producers.emplace_back([&, i]() {
        for (int j = 0; j < kItemsPerProducer; ++j) {
          q.push(i * kItemsPerProducer + j);
        }
      });
    }
    for (auto& p : producers) {
      p.join();
    }

    EXPECT_EQ(q.size(), kProducerNum * kItemsPerProducer);

    std::vector<int> items;
    items.reserve(kProducerNum * kItemsPerProducer);
    while (!q.empty()) {
      items.push_back(q.waitPop());
    }
    EXPECT_EQ(items.size(), kProducerNum* kItemsPerProducer);

    std::sort(items.begin(), items.end());
    for (int i = 0; i < items.size(); ++i) {
      EXPECT_EQ(items[i], i);
    }
  }

  TEST_F(ConcurrentQueueTest, MultipleConsumersWaitPop) {
    constexpr int kTotalItems = 100000;
    constexpr int kNumConsumers = 5;

    ConcurrentQueue<int> q;
    std::thread producer([&]() {
      for (int i = 0; i < kTotalItems; ++i) {
        q.push(i);
      }
    });

    std::vector<std::vector<int>> buckets(kNumConsumers);
    std::atomic<int> count{ 0 };
    std::vector<std::thread> consumers;
    for (int i = 0; i < kNumConsumers; ++i) {
      consumers.emplace_back([&, i]() {
        while (count.fetch_add(1) < kTotalItems) {
          int item = q.waitPop();
          buckets[i].push_back(item);
        }
      });
    }

    producer.join();
    for (auto& t : consumers) {
      t.join();
    }

    std::vector<int> allItems;
    for (auto& bucket : buckets) {
      for (int item : bucket) {
        allItems.push_back(item);
      }
    }

    EXPECT_EQ(allItems.size(), kTotalItems);
    std::sort(allItems.begin(), allItems.end());
    for (int i = 0; i < allItems.size(); ++i) {
      EXPECT_EQ(allItems[i], i);
    }
  }

  TEST_F(ConcurrentQueueTest, MultipleProducerConsumerPushWaitPop) {
    constexpr int kProducerNum = 5;
    constexpr int kNumConsumers = 5;
    constexpr int kItemsPerProducer = 100000;
    constexpr int kTotalItems = kProducerNum * kItemsPerProducer;

    ConcurrentQueue<int> q;
    std::vector<std::thread> producers;
    for (int i = 0; i < kProducerNum; ++i) {
      producers.emplace_back([&, i]() {
        for (int j = 0; j < kItemsPerProducer; ++j) {
          q.push(i * kItemsPerProducer + j);
        }
      });
    }

    std::vector<std::vector<int>> buckets(kNumConsumers);
    std::atomic<int> count{ 0 };
    std::vector<std::thread> consumers;
    for (int i = 0; i < kNumConsumers; ++i) {
      consumers.emplace_back([&, i]() {
        while (count.fetch_add(1) < kTotalItems) {
          int item = q.waitPop();
          buckets[i].push_back(item);
        }
      });
    }

    for (auto& t : producers) {
      t.join();
    }
    for (auto& t : consumers) {
      t.join();
    }

    std::vector<int> allItems;
    for (auto& bucket : buckets) {
      for (int item : bucket) {
        allItems.push_back(item);
      }
    }

    EXPECT_EQ(allItems.size(), kTotalItems);
    std::sort(allItems.begin(), allItems.end());
    for (int i = 0; i < allItems.size(); ++i) {
      EXPECT_EQ(allItems[i], i);
    }
  }
}
