#include "OrderBook.h"

void OrderBook::addToList(Level& level, Order* order) {
    if (!level.head) {
        level.head = order;
        level.tail = order;
        order->next = nullptr;
        order->prev = nullptr;
    } else {
        level.tail->next = order;
        order->prev = level.tail;
        order->next = nullptr;
        level.tail = order;
    }
}

void OrderBook::removeFromList(Level& level, Order* order) {
    if (order->prev) order->prev->next = order->next;
    else level.head = order->next;

    if (order->next) order->next->prev = order->prev;
    else level.tail = order->prev;
}

void OrderBook::addOrder(int id, Side side, double price, int quantity) {
    if (side == Side::BUY) {
        auto it = asks.begin();
        while (it != asks.end() && it->first <= price && quantity > 0) {
            Level& level = it->second;
            Order* curr = level.head;
            while (curr && quantity > 0) {
                int tradeQty = std::min(quantity, curr->quantity);
                quantity -= tradeQty;
                curr->quantity -= tradeQty;
                
                totalVolume += tradeQty; 

                if (curr->quantity == 0) {
                    Order* next = curr->next;
                    removeFromList(level, curr);
                    orderPool.deallocate(curr);
                    curr = next;
                } else {
                    curr = curr->next;
                }
            }
            if (!level.head) it = asks.erase(it);
            else ++it;
        }
    } else { 
        auto it = bids.begin();
        while (it != bids.end() && it->first >= price && quantity > 0) {
            Level& level = it->second;
            Order* curr = level.head;
            while (curr && quantity > 0) {
                int tradeQty = std::min(quantity, curr->quantity);
                quantity -= tradeQty;
                curr->quantity -= tradeQty;

                totalVolume += tradeQty;

                if (curr->quantity == 0) {
                    Order* next = curr->next;
                    removeFromList(level, curr);
                    orderPool.deallocate(curr);
                    curr = next;
                } else {
                    curr = curr->next;
                }
            }
            if (!level.head) it = bids.erase(it);
            else ++it;
        }
    }

    if (quantity > 0) {
        Order* order = orderPool.allocate();
        if (!order) return; 
        order->id = id; order->side = side; order->price = price; order->quantity = quantity;
        
        if (side == Side::BUY) addToList(bids[price], order);
        else addToList(asks[price], order);
    }
}