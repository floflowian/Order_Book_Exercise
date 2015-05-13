/* 
 * File:   processor.h
 * Author: Florian
 *
 * Created on 6 mai 2015, 23:15
 */

#ifndef PROCESSOR_H
#define	PROCESSOR_H

#include <iomanip> 
using namespace std;

class Processor {
    order_book book;
    uint32_t target_size;
    int32_t expense;
    int32_t income;

public:

    Processor(uint32_t target) : target_size(target), expense(-1), income(-1) {
    }

    void updateIncome(uint32_t timestamp) {
        int32_t currentExpense = expense;
        if (book.bid_quantitySum() >= target_size) {
            uint32_t buyLeft = target_size;
            uint32_t level = 1;
            expense = 0;
            while (buyLeft > 0) {
                if (buyLeft > book.bid_getLevel(level).size) {
                    buyLeft -= book.bid_getLevel(level).size;
                    expense += book.bid_getLevel(level).size * book.bid_getLevel(level).price;
                    ++level;
                } else {
                    expense += buyLeft * book.bid_getLevel(level).price;
                    buyLeft = 0;
                }
            }
        } else {
            expense = -1;
        }
        if (currentExpense != expense) {
            if (expense != -1) {
                double expenseDisplay = expense / 10e2;
                cout << timestamp << " S " << fixed << setprecision(2) << expenseDisplay << endl;
            } else {
                cout << timestamp << " S " << "NA" << endl;
            }
        }
    }

    void updateExpense(uint32_t timestamp) {
        int32_t currentIncome = income;
        if (book.ask_quantitySum() >= target_size) {
            uint32_t buyLeft = target_size;
            uint32_t level = 1;
            income = 0;
            while (buyLeft > 0) {
                if (buyLeft > book.ask_getLevel(level).size) {
                    buyLeft -= book.ask_getLevel(level).size;
                    income += book.ask_getLevel(level).size * book.ask_getLevel(level).price;
                    ++level;
                } else {
                    income += buyLeft * book.ask_getLevel(level).price;
                    buyLeft = 0;
                }
            }
        } else {
            income = -1;
        }
        if (currentIncome != income) {
            if (income != -1) {
                double incomeDisplay = income / 10e2;
                cout << timestamp << " B " << fixed << setprecision(2) << incomeDisplay << endl;
            } else {
                cout << timestamp << " B " << "NA" << endl;
            }
        }
    }

    void reduce_order(vector<string> tokens, uint32_t lineNumber) {
        uint32_t timestamp;
        char action;
        string orderId;
        uint32_t size;

        try {
            timestamp = stoi(tokens[0]);
            action = tokens[1][0];
            orderId = tokens[2];
            size = stoi(tokens[3]);
        } catch (exception& e) {
            cerr << "Invalid line " << lineNumber << endl;
            return;
        }
        std::map<std::string, idLevel>::iterator it = book.orderIdLevel.find(orderId);
        if (it != book.orderIdLevel.end()) {
            if (it->second.side == Side::Ask) {
                uint32_t currentQty = book.ask_getLevel(it->second.level).size;
                book.totalAskQuantity -= size;
                if (currentQty - size == 0) {
                    book.ask_remove(it->second.level);
                    book.orderIdLevel.erase(orderId);
                } else if (currentQty - size > 0) {
                    book.ask_getLevel(it->second.level).size = currentQty - size;
                } else
                    throw std::logic_error("Cannot reduce ask quantity of order below 0");
            } else if (it->second.side == Side::Bid) {
                uint32_t currentQty = book.bid_getLevel(it->second.level).size;
                book.totalBidQuantity -= size;
                if (currentQty - size == 0) {
                    book.bid_remove(it->second.level);
                    book.orderIdLevel.erase(orderId);
                } else if (currentQty - size > 0) {
                    book.bid_getLevel(it->second.level).size = currentQty - size;
                } else
                    throw std::logic_error("Cannot reduce bid quantity of order below 0");

            }
        } else {
            cerr << "Order id " << orderId << " not found" << endl;
        }
        updateExpense(timestamp);
        updateIncome(timestamp);
    }

    void add_order(vector<string> tokens, uint32_t lineNumber) {
        uint32_t timestamp;
        char action;
        string orderId;
        char side;
        double price;
        uint32_t size;
        try {
            timestamp = stoi(tokens[0]);
            action = tokens[1][0];
            orderId = tokens[2];
            side = tokens[3][0];
            price = stod(tokens[4]);
            size = stoi(tokens[5]);
        } catch (exception& e) {
            cerr << "Invalid line " << lineNumber << endl;
            return;
        }
        uint32_t priceInt = price * 10e2;
        if (side == 'S') {
            uint32_t insertedLevel = book.ask_insert(priceInt, size, orderId);
            idLevel o(insertedLevel, Side::Ask);
            book.orderIdLevel.insert(std::pair<std::string, idLevel>(orderId, o));
        } else if (side == 'B') {
            uint32_t insertedLevel = book.bid_insert(priceInt, size, orderId);
            idLevel o(insertedLevel, Side::Bid);
            book.orderIdLevel.insert(std::pair<std::string, idLevel>(orderId, o));
        } else {
            cerr << "Unrecognized side on line " << lineNumber << endl;
            return;
        }

        updateExpense(timestamp);
        updateIncome(timestamp);
    }

    void process_message(vector<string> tokens, uint32_t lineNumber) {

        if (tokens[1] == "R") {
            reduce_order(tokens, lineNumber);
        } else if (tokens[1] == "A") {
            add_order(tokens, lineNumber);
        } else {
            cerr << "Undefined action on line " << lineNumber << endl;
            return;
        }
    }
};

#endif	/* PROCESSOR_H */

