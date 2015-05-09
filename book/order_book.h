/* 
 * File:   order_book.h
 * Author: Florian
 *
 * Created on 6 mai 2015, 21:56
 */

#ifndef ORDER_BOOK_H
#define	ORDER_BOOK_H
#include <stdio.h>
#include <string.h>
using namespace std;

typedef struct {
    uint32_t side;
    uint32_t level;
} idLevel;

struct order_book {

    typedef struct {
        uint32_t price = 0;
        uint32_t size = 0;
        std::string id;
    } entry;

    typedef struct {
        entry bid;
        entry ask;
    } level;

    entry& bid_getLevel(uint32_t index) {
        if (index > bidDepth){
            std::cout<<index<<" > "<<bidDepth<<std::endl;
            throw std::logic_error("Cannot get level > bid depth");
        }
        return entries[index - 1].bid;
    }

    entry& ask_getLevel(uint32_t index) {
        if (index > askDepth){
            std::cout<<index<<" > "<<askDepth<<std::endl;
            throw std::logic_error("Cannot get level > ask depth");
        }
        return entries[index - 1].ask;
    }

    uint32_t bid_quantitySum() {
        uint32_t sum = 0;
        for (int i = 0; i < bidDepth; ++i) {
            sum += entries[i].bid.size;
        }
        return sum;
    }

    uint32_t ask_quantitySum() {
        uint32_t sum = 0;
        for (int i = 0; i < askDepth; ++i) {
            sum += entries[i].ask.size;
        }
        return sum;
    }

    uint32_t bid_insert(uint32_t price, uint32_t quantity, string id) {
        uint32_t level = 1;
        struct level lvl;
        entries.push_back(lvl);
        for (int i = 0; i < bidDepth; ++i) {
            uint32_t priceLevel = entries[i].bid.price;
            if (price == priceLevel) {
                ++level;
                break;
            }
            if (price > priceLevel)
                break;
            ++level;
        }
      if (level <= bidDepth) {
            int levelint = (int) level;
            for (int i = bidDepth - 1; i >= levelint - 1; --i) {
                if(entries[i].bid.id=="f"){
                    cout<<endl;
                }
                std::map<std::string, idLevel>::iterator it = orderIdLevel.find(entries[i].bid.id);
                it->second.level=it->second.level+1;
                entries[i + 1].bid.price = entries[i].bid.price;
                entries[i + 1].bid.size = entries[i].bid.size;
                entries[i + 1].bid.id = entries[i].bid.id;
            }
            entries[level - 1].bid.price = price;
            entries[level - 1].bid.size = quantity;
            entries[level - 1].bid.id = id;
        } else if (level > bidDepth) {
            entries[bidDepth].bid.price = price;
            entries[bidDepth].bid.size = quantity;
            entries[bidDepth].bid.id = id;
        }
        ++bidDepth;
        return level;
    }

    uint32_t ask_insert(uint32_t price, uint32_t quantity, std::string id) {
        uint32_t level = 1;
        level lvl;
        entries.push_back(lvl);
        for (int i = 0; i < askDepth; ++i) {
            uint32_t priceLevel = entries[i].ask.price;
            if (price == priceLevel) {
                ++level;
                break;
            }
            if (price < priceLevel)
                break;
            ++level;
        }
       if (level <= askDepth) {
            int levelint = (int) level;
            for (int i = askDepth - 1; i >= levelint - 1; --i) {
                std::map<std::string, idLevel>::iterator it = orderIdLevel.find(entries[i].ask.id);
                it->second.level=it->second.level+1;
                entries[i + 1].ask.price = entries[i].ask.price;
                entries[i + 1].ask.size = entries[i].ask.size;
                entries[i + 1].ask.id = entries[i].ask.id;
            }
            entries[level - 1].ask.price = price;
            entries[level - 1].ask.size = quantity;
            entries[level - 1].ask.id = id;
        } else if (level > askDepth) {
            entries[askDepth].ask.price = price;
            entries[askDepth].ask.size = quantity;
            entries[askDepth].ask.id = id;
        }
        ++askDepth;
        return level;
    }

    void bid_remove(uint32_t level) {
        if (level < 1 || level > entries.size() || level > bidDepth)
            throw std::logic_error("Cannot remove bid level");
        if (level == bidDepth) {
            entries[level - 1].bid.price = 0;
            entries[level - 1].bid.size = 0;
            entries[level - 1].bid.id = "";
        } else {
            int levelint = (int) level;
            for (int i = levelint; i <= bidDepth - 1; ++i) {
                std::map<std::string, idLevel>::iterator it = orderIdLevel.find(entries[i].bid.id);
                it->second.level=it->second.level-1;
                entries[i - 1].bid.price = entries[i].bid.price;
                entries[i - 1].bid.size = entries[i].bid.size;
                entries[i - 1].bid.id = entries[i].bid.id;
            }

        entries.pop_back();
//            entries[bidDepth - 1].bid.price = 0;
//            entries[bidDepth - 1].bid.size = 0;
//            entries[bidDepth - 1].bid.id = "";
        }

        --bidDepth;
    }

    void ask_remove(uint32_t level) {
        if (level < 1 || level > entries.size() || level > askDepth)
            throw std::logic_error("Cannot remove ask level");
        if (level == askDepth) {
            entries[level - 1].ask.price = 0;
            entries[level - 1].ask.size = 0;
            entries[level - 1].ask.id = "";
        } else {
            int levelint = (int) level;
            for (int i = levelint; i <= askDepth - 1; ++i) {
                std::map<std::string, idLevel>::iterator it = orderIdLevel.find(entries[i].ask.id);
                it->second.level=it->second.level-1;
                entries[i - 1].ask.price = entries[i].ask.price;
                entries[i - 1].ask.size = entries[i].ask.size;
                entries[i - 1].ask.id = entries[i].ask.id;
            }
            entries.pop_back();
//            entries[askDepth - 1].ask.price = 0;
//            entries[askDepth - 1].ask.size = 0;
//            entries[askDepth - 1].ask.id = "";
        }

        --askDepth;
    }

    void bid_display() {
        cout << "---------------------------------------------------" << endl;
        cout << "|                       " << "BID" << "                      |" << endl;
        cout << "|       Price        Quantity           OrderId  |" << endl;
        cout << "---------------------------------------------------" << endl;
        for (int i = 0; i < bidDepth; ++i) {
            cout << "|       " << entries[i].bid.price << "        " << entries[i].bid.size << "                    " << entries[i].bid.id << "   |" << endl;
        }
        cout << "---------------------------------------------------" << endl;

    }

    void ask_display() {
        cout << "---------------------------------------------------" << endl;
        cout << "|                       " << "ASK" << "                      |" << endl;
        cout << "|       Price        Quantity           OrderId  |" << endl;
        cout << "---------------------------------------------------" << endl;
        for (int i = 0; i < askDepth; ++i) {
            cout << "|       " << entries[i].ask.price << "        " << entries[i].ask.size << "                    " << entries[i].ask.id << "   |" << endl;
        }
        cout << "---------------------------------------------------" << endl;

    }

    uint32_t bidDepth = 0;
    uint32_t askDepth = 0;
    vector<level> entries;
    std::map<std::string, idLevel> orderIdLevel;
};

#endif	/* ORDER_BOOK_H */

