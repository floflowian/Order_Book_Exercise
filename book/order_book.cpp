//#include <iostream>
//#include "book/order_book.h"
//using namespace std;
//
//uint32_t BookSide::insert(double m_price, uint32_t m_quantity, uint32_t m_nbOrders) {
//    uint32_t indexLevelToInsert = 0;
//    if (depth == maxDepth) {
//        cout << "Cannot insert level with price " << m_price << "because maxDepth reached" << endl;
//        exit (EXIT_FAILURE);
//    }
//
//    for (int i = 0; i < depth; ++i) {
//        double priceLevel = levels[i].getPrice();
//        if (m_price == priceLevel) {
//            cout << "Cannot insert level with price " << m_price << "because it already exists" << endl;
//            exit (EXIT_FAILURE);
//        }
//        if (side == ASK && m_price < priceLevel)
//            break;
//        else if (side == BID && m_price > priceLevel)
//            break;
//        ++indexLevelToInsert;
//    }
//
//    ++depth;
//    for (int i = indexLevelToInsert + 1; i < depth; ++i) {
//        levels[i] = levels[i - 1];
//    }
//    BookLevel levelToInsert(m_price, m_quantity, m_nbOrders);
//    levels[indexLevelToInsert] = levelToInsert;
//    
//    return indexLevelToInsert+1;
//}
//
//void BookSide::change(uint32_t levelIndex, uint32_t m_quantity, uint32_t m_nbOrders) {
//    if(levelIndex>=depth){
//        cout<< "Cannot change level "<<levelIndex<<" because level>depth"<<endl;
//        exit (EXIT_FAILURE);
//    }
//    levels[levelIndex].setQuantity(m_quantity);
//    levels[levelIndex].setNbOrders(m_nbOrders);
//}
//
//void BookSide::overlay(uint32_t levelIndex, double m_price, uint32_t m_quantity, uint32_t m_nbOrders) {
//if(levelIndex>=depth){
//        cout<< "Cannot overlay level "<<levelIndex<<" because level>depth"<<endl;
//        exit (EXIT_FAILURE);
//    }
//    levels[levelIndex].setPrice(m_price);
//    levels[levelIndex].setQuantity(m_quantity);
//    levels[levelIndex].setNbOrders(m_nbOrders);
//}
//
//void BookSide::remove(uint32_t levelIndex) {
//
//}
//void BookSide::removeAll() {
//    for(int i=0;i<depth;++i){
//        levels[i].setNbOrders(0);
//        levels[i].setQuantity(0);
//        levels[i].setPrice(0.0);
//    }
//    depth=0;
//}
//void BookSide::display() {
//    cout << "---------------------------------------------------" << endl;
//    cout << "|                       " << this->getSideString() << "                      |" << endl;
//    cout << "|       Price        Quantity           NbOrders  |" << endl;
//    cout << "---------------------------------------------------" << endl;
//    for (int i = 0; i < maxDepth; ++i) {
//        cout << "|       " << levels[i].getPrice() << "        " << levels[i].getQuantity() << "                    " << levels[i].getNbOrders() << "   |" << endl;
//    }
//    cout << "---------------------------------------------------" << endl;
//
//}