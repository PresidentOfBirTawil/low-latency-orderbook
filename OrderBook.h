#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <chrono>
#include <algorithm>

enum class Side { BUY, SELL };

struct Order {
    int id;
    Side side;
    double price;
    int quantity;
    Order* next; 
    Order* prev;
};

class OrderPool {
private:
    std::vector<Order> pool;
    Order* freeListHead;

public:
    OrderPool(size_t size) {
        pool.resize(size);
        for (size_t i = 0; i < size - 1; ++i) {
            pool[i].next = &pool[i + 1];
        }
        pool[size - 1].next = nullptr;
        freeListHead = &pool[0];
    }

    Order* allocate() {
        if (!freeListHead) return nullptr;
        Order* order = freeListHead;
        freeListHead = freeListHead->next;
        return order;
    }

    void deallocate(Order* order) {
        order->next = freeListHead;
        freeListHead = order;
    }
};

class OrderBook {
private:
    struct Level {
        Order* head = nullptr;
        Order* tail = nullptr;
    };

    std::map<double, Level, std::greater<double>> bids;
    std::map<double, Level, std::less<double>> asks;
    OrderPool orderPool;
    
    unsigned long long totalVolume = 0; 

    void addToList(Level& level, Order* order);
    void removeFromList(Level& level, Order* order);

public:
    OrderBook(size_t size) : orderPool(size) {}
    void addOrder(int id, Side side, double price, int quantity);
    
    unsigned long long getVolume() const { return totalVolume; }
};