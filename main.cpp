#include "OrderBook.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <random>

int main() {
    OrderBook book(1000000); 
    
    std::cout << "Starting HFT Simulation (Worst-Case Fragmentation)...\n";

    // Random Number Generator (To break CPU Branch Prediction)
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> side_dist(0, 1);
    std::uniform_int_distribution<int> price_dist(80, 120);
    std::uniform_int_distribution<int> qty_dist(1, 5);

    auto start = std::chrono::high_resolution_clock::now();

    // 500,000 Orders with RANDOM parameters
    // This forces the map to re-balance and insert nodes constantly
    for (int i = 0; i < 500000; ++i) {
        Side side = (side_dist(rng) == 0) ? Side::BUY : Side::SELL;
        double price = (double)price_dist(rng);
        int qty = qty_dist(rng);

        book.addOrder(i, side, price, qty);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    double latency = (diff.count() / 500000) * 1e9;

    std::cout << "--------------------------------\n";
    std::cout << "Orders Processed: 500,000\n";
    std::cout << "Volume Traded:    " << book.getVolume() << "\n";
    std::cout << "Total Time:       " << diff.count() << " s\n";
    std::cout << "Average Latency:  " << latency << " ns\n";
    std::cout << "--------------------------------\n";

    return 0;
}