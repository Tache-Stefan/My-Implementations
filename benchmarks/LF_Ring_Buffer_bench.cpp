#include <benchmark/benchmark.h>
#include <LF_Ring_Buffer.h>
#include <atomic>
#include <cstddef>
#include <thread>

static void BM_LFRingBufferSPSCThreads(benchmark::State& state) {
    const std::size_t cap = static_cast<std::size_t>(state.range(0));
    constexpr std::size_t kBatch = 1 << 20; // items per producer/consumer run

    for (auto _ : state) {
        my_std::LF_Ring_Buffer<int> rb(cap);
        std::atomic<bool> start{false};
        int sink = 0;

        std::thread producer([&] {
            while (!start.load(std::memory_order_acquire)) {}
            for (std::size_t i = 0; i < kBatch; ++i) {
                while (!rb.push(static_cast<int>(i))) {}
            }
        });

        std::thread consumer([&] {
            while (!start.load(std::memory_order_acquire)) {}
            int value = 0;
            for (std::size_t i = 0; i < kBatch; ++i) {
                while (!rb.pop(value)) {}
                sink ^= value;
            }
        });

        start.store(true, std::memory_order_release);
        producer.join();
        consumer.join();

        benchmark::DoNotOptimize(sink);
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) *
                            static_cast<int64_t>(kBatch) * 2);
}
BENCHMARK(BM_LFRingBufferSPSCThreads)->Arg(64)->UseRealTime();
BENCHMARK(BM_LFRingBufferSPSCThreads)->Arg(1024)->UseRealTime();
BENCHMARK(BM_LFRingBufferSPSCThreads)->Arg(65536)->UseRealTime();

static void BM_LFRingBufferSPSCThreadsYield(benchmark::State& state) {
    const std::size_t cap = static_cast<std::size_t>(state.range(0));
    constexpr std::size_t kBatch = 1 << 20;

    for (auto _ : state) {
        my_std::LF_Ring_Buffer<int> rb(cap);
        std::atomic<bool> start{false};
        int sink = 0;

        std::thread producer([&] {
            while (!start.load(std::memory_order_acquire)) {}
            for (std::size_t i = 0; i < kBatch; ++i) {
                while (!rb.push(static_cast<int>(i))) {
                    std::this_thread::yield();
                }
            }
        });

        std::thread consumer([&] {
            while (!start.load(std::memory_order_acquire)) {}
            int value = 0;
            for (std::size_t i = 0; i < kBatch; ++i) {
                while (!rb.pop(value)) {
                    std::this_thread::yield();
                }
                sink ^= value;
            }
        });

        start.store(true, std::memory_order_release);
        producer.join();
        consumer.join();

        benchmark::DoNotOptimize(sink);
        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(kBatch) * 2);
}
BENCHMARK(BM_LFRingBufferSPSCThreadsYield)->Arg(64)->UseRealTime();
BENCHMARK(BM_LFRingBufferSPSCThreadsYield)->Arg(1024)->UseRealTime();
BENCHMARK(BM_LFRingBufferSPSCThreadsYield)->Arg(65536)->UseRealTime();

static inline void Backoff(unsigned& spins) {
    if (spins < 32) {
        ++spins; // short spin
    } else {
        std::this_thread::yield();
    }
}

static void BM_LFRingBufferSPSCThreadsBackoff(benchmark::State& state) {
    const std::size_t cap = static_cast<std::size_t>(state.range(0));
    constexpr std::size_t kBatch = 1 << 20;

    for (auto _ : state) {
        my_std::LF_Ring_Buffer<int> rb(cap);
        std::atomic<bool> start{false};
        int sink = 0;

        std::thread producer([&] {
            while (!start.load(std::memory_order_acquire)) {}
            for (std::size_t i = 0; i < kBatch; ++i) {
                unsigned spins = 0;
                while (!rb.push(static_cast<int>(i))) Backoff(spins);
            }
        });

        std::thread consumer([&] {
            while (!start.load(std::memory_order_acquire)) {}
            int value = 0;
            for (std::size_t i = 0; i < kBatch; ++i) {
                unsigned spins = 0;
                while (!rb.pop(value)) Backoff(spins);
                sink ^= value;
            }
        });

        start.store(true, std::memory_order_release);
        producer.join();
        consumer.join();
        benchmark::DoNotOptimize(sink);
    }

    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(kBatch) * 2);
}
BENCHMARK(BM_LFRingBufferSPSCThreadsBackoff)->Arg(64)->Arg(1024)->Arg(65536)->UseRealTime();

static void BM_LFRingBufferSPSCThreadsSteadyHalf(benchmark::State& state) {
    const std::size_t cap = static_cast<std::size_t>(state.range(0));
    constexpr std::size_t kBatch = 1 << 20;

    for (auto _ : state) {
        my_std::LF_Ring_Buffer<int> rb(cap);
        for (std::size_t i = 0; i < cap / 2; ++i) (void)rb.push(static_cast<int>(i));

        std::atomic<bool> start{false};
        int sink = 0;

        std::thread producer([&] {
            while (!start.load(std::memory_order_acquire)) {}
            for (std::size_t i = 0; i < kBatch; ++i) {
                while (!rb.push(static_cast<int>(i))) std::this_thread::yield();
            }
        });

        std::thread consumer([&] {
            while (!start.load(std::memory_order_acquire)) {}
            int value = 0;
            for (std::size_t i = 0; i < kBatch; ++i) {
                while (!rb.pop(value)) std::this_thread::yield();
                sink ^= value;
            }
        });

        start.store(true, std::memory_order_release);
        producer.join();
        consumer.join();
        benchmark::DoNotOptimize(sink);
    }

    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(kBatch) * 2);
}
BENCHMARK(BM_LFRingBufferSPSCThreadsSteadyHalf)->Arg(64)->Arg(1024)->Arg(65536)->UseRealTime();

struct Payload64 {
    std::array<std::uint64_t, 8> data{};
};

static void BM_LFRingBufferSPSCThreadsPayload64(benchmark::State& state) {
    const std::size_t cap = static_cast<std::size_t>(state.range(0));
    constexpr std::size_t kBatch = 1 << 20;

    for (auto _ : state) {
        my_std::LF_Ring_Buffer<Payload64> rb(cap);
        std::atomic<bool> start{false};
        Payload64 in{};
        std::uint64_t sink = 0;

        std::thread producer([&] {
            while (!start.load(std::memory_order_acquire)) {}
            for (std::size_t i = 0; i < kBatch; ++i) {
                in.data[0] = i;
                while (!rb.push(in)) std::this_thread::yield();
            }
        });

        std::thread consumer([&] {
            while (!start.load(std::memory_order_acquire)) {}
            Payload64 out{};
            for (std::size_t i = 0; i < kBatch; ++i) {
                while (!rb.pop(out)) std::this_thread::yield();
                sink ^= out.data[0];
            }
        });

        start.store(true, std::memory_order_release);
        producer.join();
        consumer.join();
        benchmark::DoNotOptimize(sink);
    }

    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(kBatch) * 2);
}
BENCHMARK(BM_LFRingBufferSPSCThreadsPayload64)->Arg(64)->Arg(1024)->Arg(65536)->UseRealTime();

BENCHMARK_MAIN();
