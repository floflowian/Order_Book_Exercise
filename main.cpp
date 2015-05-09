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

#include "book/order_book.h"
#include "processor/processor.h"
using namespace std;

int main(int argc, char** argv) {
    
    if (argc < 2) {
        cerr << "Error: No target size entered" << std::endl;
        return 1;
    }
    int target_size = atoi(argv[1]);
    
    Processor * processor = new Processor((uint32_t)target_size);
    string line;
//    ifstream myfile("log.txt");
    ifstream myfile("C:/Users/Florian/Downloads/pricer.in/pricer.in");
    uint32_t count = 0;
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            ++count;
            std::filebuf fb;
            std::ostream os(&fb);
            std::stringstream ss(line);
            std::istream_iterator<std::string> begin(ss);
            std::istream_iterator<std::string> end;
            std::vector<std::string> vstrings(begin, end);
            std::copy(vstrings.begin(), vstrings.end(), std::ostream_iterator<std::string>(os, "\n"));
            processor->process_message(vstrings, count);
        }
        myfile.close();
    }
    return 0;
}

