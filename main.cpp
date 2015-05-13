/* 
 * File:   main.cpp
 * Author: Florian
 *
 * Created on 6 mai 2015, 21:51
 */
#include <stdint.h>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator> 
#include <vector> 
#include <exception>
#include <map>
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string.hpp"
#include "book/order_book.h"
#include "processor/processor.h"

ifstream fin("C:/Users/Florian/Downloads/pricer.in/pricer.in");
#define cin fin
using namespace std;

int main(int argc, char** argv) {

    if (argc < 2) {
        cerr << "Error: No target size entered" << std::endl;
        return 1;
    }
    int target_size = atoi(argv[1]);

    Processor * processor = new Processor((uint32_t) target_size);
    string line;
    uint32_t count = 0;
    while (getline(cin, line)) {
        ++count;
        std::vector<std::string> vstrings;
        boost::split(vstrings, line, boost::is_any_of(" "));
        processor->process_message(vstrings, count);
    }
    return 0;
}

