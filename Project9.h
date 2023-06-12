//
// Created by JJ Oh on 4/24/23.
//

#ifndef PA8_PROJECT9_H
#define PA8_PROJECT9_H
#include "Parse.h"
#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include <string>

class blip_reader{
public:
    void bVector_add();
    void output();
    void text();
    void var();
    void set();
    void run();
    bool isOperation(std::string a);
    int polishVecor(std::vector<std::string>& expression);
    int op(const std::string& op, int val1, int val2);
};
#endif //PA8_PROJECT9_H
