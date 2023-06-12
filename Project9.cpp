//
// Created by JJ Oh on 4/24/23.
//

#include "Project9.h"
std::map<std::string, int> varMap; // map allows to scale with large number of variables
std::vector<std::string> bVector; // allows dynamically allocatable vectors of blips to keep in track as well as use
int blip_reader::op(const std::string& op, int val1, int val2) {
    // does the arthemetric operation passed through the parameter
    if (op == "+") {
        return val1 + val2;
    } else if (op == "-") {
        return val1 - val2;
    } else if (op == "*") {
        return val1 * val2;
    } else if (op == "/") {
        return val1 / val2;
    } else if (op == ">=") {
        return val1 >= val2;
    } else if (op == "<=") {
        return val1 <= val2;
    } else if (op == "%") {
        return val1 % val2;
    } else if (op == "&&") {
        return val1 && val2;
    } else if (op == "||") {
        return val1 || val2;
    } else if (op == ">") {
        return val1 > val2;
    } else if (op == "<") {
        return val1 < val2;
    } else if (op == "==") {
        return val1 == val2;
    } else{ // if (op == "!=")
        return val1 != val2;
    }
}

void blip_reader::bVector_add() { // adds blips to vVector
    const char* token1 = peek_next_token(); // peeks the next token but doesn't read the next token
    std::string peekedToken = std::string(token1); // convert to string
    while (next_token_type != END && peekedToken
    != std::string("var") && peekedToken
    != std::string("set")&& peekedToken
    != std::string("text") && peekedToken
    != std::string("output")) { // while the peekedToken is not another blip or the end of a blip
        read_next_token(); // good to read the next token
        std::string firstOp = std::string(next_token());
        if (firstOp == "//") { // if it is "//" skip to next line
            skip_line();
        }
        bVector.push_back(firstOp); // now found a blip. Add it to the vector
        // peak next token and add until you have added all the blips
        peekedToken = std::string(peek_next_token());
    }
}
void blip_reader::output(){ // when blip is "output"
    bVector_add(); // add the blips
    int polished = polishVecor(bVector); // now polish the Vector
    std::cout << polished; // print the polished vector
    bVector.clear(); // clear the vector
}
void blip_reader::text(){ // when blip is "text"
    // don't need to add the blips to the vector
    read_next_token();
    const char* token1 = next_token();
    std::string text1 = std::string(token1);
    if (text1 == "//") {
        skip_line();
    }
    std::cout << next_token(); // print the text
}
void blip_reader::var(){ // when blip is "var"
    read_next_token();
    const char* token1 = next_token();
    std::string nextToken = std::string(token1);
    bVector_add();
    if (varMap.find(nextToken) != varMap.end()) { // check if variable is already in the map
        std::cout << "variable " << nextToken.c_str() << " incorrectly re-initialized" << "\n";
    }
    varMap[token1] = polishVecor(bVector);
    bVector.clear();
}
void blip_reader::set(){ // when blip is "set" utilize implementation of var
    read_next_token();
    const char* token1 = next_token();
    std::string nextToken = std::string(token1);
    bVector_add();
    if (varMap.find(nextToken) == varMap.end()) { // check if variable is in the map
        std::cout << "variable " << nextToken.c_str() << " not declared" << "\n";
    }
    varMap[token1] = polishVecor(bVector);
    bVector.clear();
}
void blip_reader::run() { // run code
    while (next_token_type != END) {
        read_next_token(); // read the first blip
        std::string nextToken = std::string(next_token()); // set nextToken to the token currently reading
        if (nextToken == "//") { // if its "//" skip to next line since its a comment
            skip_line();
        } else if (nextToken == "text") {
            text();
        } else if (nextToken == "output") {
            output();
        } else if (nextToken == "set") {
            set();
        } else if (nextToken == "var") {
           var();
        }
            else{
                //std::cout << "Error: selectFunction did not call any default function" << std::endl;
            }
        }
    varMap.clear(); // clear Variables map after execution
}
bool blip_reader::isOperation(std::string a) {
    if(a != "+" && a != "<=" && a != "%" && a != "&&" && a != "||"  &&
       a != "!" && a != "~" && a != "-" && a != "*" && a != "/" &&
       a != "==" && a != "!=" && a != ">" && a != "<" &&
       a != ">="){
        return true;
    } else {
        return false;
    }
}
int blip_reader::polishVecor(std::vector<std::string> &vector) {
    std::string current = vector.front();
    if (!(varMap.find(current) == varMap.end())) {
        vector.erase(vector.begin());
        int returnVal = varMap[current];
        return returnVal; // return the number for recursive case
    } else if (isOperation(current)) { // if current is an operation
        vector.erase(vector.begin()); // erase the operation from vector
        return atoi(current.c_str()); // change to int
    } else if (current == "~" ){ // special case: unary operators, only one expression after operator?
        vector.erase(vector.begin());
        int result = -1 * polishVecor(vector); // recursive call to polish following
        return result;
    } else if (current != "!") {
        vector.erase(vector.begin());
        int firstint = polishVecor(vector); // recursive call to polish
        int secondint = polishVecor(vector); // recursive call to polish
        return op(current, firstint, secondint);
    } else if (current == "!") { // if ! than get the opposite
        vector.erase(vector.begin());
        int result_ = polishVecor(vector); // recursive call to polish
        if (result_ != 0) {
            return false;
        }
        else if (result_ == 0) {
            return true;
        }
    } else{
        // this shouldn't happen right?
    }
}
int main(void){
    blip_reader project9;
    set_input("test1.blip");
    project9.run();
    return 0;
}
