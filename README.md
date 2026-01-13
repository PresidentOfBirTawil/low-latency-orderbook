# High-Frequency Limit Order Book

C++20 implementation of a limit order book optimized for cache locality and instruction pipelining.

### Benchmark Results
**Environment:** Single-threaded, L3 Cache Resident
**Scenario:** Worst-case fragmentation (randomized order flow)

| Metric | Result |
| :--- | :--- |
| **Average Latency** | **54 ns** |
| **Throughput** | ~18.5M orders/sec |
| **Memory Model** | Linear Arena (Zero-allocation hot path) |

### optimization
- **Custom Allocators:** Replaced `std::shared_ptr` / `new` with a contiguous memory pool to eliminate heap fragmentation.
- **Data Structure:** Intrusive linked lists for O(1) order cancellation within price levels.
- **Compiler:** `-O3 -march=native` for aggressive loop unrolling and vectorization.

### Build
```bash
g++ main.cpp OrderBook.cpp -o hft_engine -O3
./hft_engine
