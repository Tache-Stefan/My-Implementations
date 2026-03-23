# C++ Implementations

<b>WIP</b>

Custom implementations of C++ containers and data structures.

## Building

```bash
cmake -B build -G Ninja
cmake --build build
```

## Running Tests

```bash
ctest --test-dir build --output-on-failure
```

## Running Benchmarks

Build only the benchmark target:

```bash
cmake -B build -G Ninja
cmake --build build --target benchmarks
```

Run all benchmarks:

```bash
build\benchmarks.exe
```

Run only SPSC benchmarks:

```bash
build\benchmarks.exe --benchmark_filter=BM_LFRingBufferSPSCThreads.*
```

Recommended stable run (longer + repeated):

```bash
build\benchmarks.exe --benchmark_min_time=1s --benchmark_repetitions=8 --benchmark_report_aggregates_only=true
```
