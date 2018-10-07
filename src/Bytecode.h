#include <string>
#include <vector>

using namespace std;

enum OPCODE : unsigned char
{
    IADD = 1,
    ISUB = 2,
    IMUL = 3,
    ILT  = 4,
    IEQ = 5,
    BR = 6,
    BRT = 7,
    BRF = 8,
    ICONST = 9,
    LOAD = 10,
    GLOAD = 11,
    STORE = 12,
    GSTORE = 13,
    PRINT = 14,
    POP = 15,
    CALL = 16,
    RET = 17,
    HALT = 18,
};

typedef struct Bytecode
{
    string name;
    int args;
    Bytecode(string n, int a) : name(n), args(a) {};
} Bytecode;

const vector<Bytecode> BYTECODES = { 
    Bytecode("nop", 0),
    Bytecode("iadd", 0),
    Bytecode("isub", 0),
    Bytecode("imul", 0),
    Bytecode("ilt", 0),
    Bytecode("ieq", 0),
    Bytecode("br", 1),
    Bytecode("brt", 1),
    Bytecode("brf", 1),
    Bytecode("iconst", 1),
    Bytecode("load", 1),
    Bytecode("gload", 1),
    Bytecode("store", 1),
    Bytecode("gstore", 1),
    Bytecode("print", 0),
    Bytecode("pop", 0),
    Bytecode("call", 1), // call index of function in meta-info table
    Bytecode("ret", 0),
    Bytecode("halt", 0),
    };