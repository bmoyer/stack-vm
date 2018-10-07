#include <iostream>
#include "VirtualMachine.h"
#include "Bytecode.h"

using namespace std;

void addFunction()
{
    vector<int> code{
        LOAD, 0, // move argument from register 0 to stack
        LOAD, 1, // move argument from register 1 to stack
        LOAD, 2,
        IADD,
        IADD,
        RET,

        ICONST, 7,
        ICONST, 7,
        ICONST, 28,
        CALL, 1, // this will move arguments from stack to registers
        PRINT,
        HALT};

    vector<FunctionMetaData> fmd{
        FunctionMetaData("main", 0, 0, 9),
        FunctionMetaData("addFunction", 3, 3, 0),
    };

    VirtualMachine vm(code, 0, fmd);
    vm.exec(9);
}

void factorial()
{
    //vector<int> code{ICONST, 7, ICONST, 35, IADD, PRINT, HALT};
    vector<int> code{ICONST, 7, STORE, 0};
    vector<FunctionMetaData> fmd{FunctionMetaData("main", 0, 1, 0)};

    // factorial
    vector<int> code2{
        //.def factorial: ARGS=1, LOCALS=0	ADDRESS
        //	IF N < 2 RETURN 1
        LOAD, 0,   // 0
        ICONST, 2, // 2
        ILT,       // 4
        BRF, 10,   // 5
        ICONST, 1, // 7
        RET,       // 9
                   //CONT:
                   //	RETURN N * FACT(N-1)
        LOAD, 0,   // 10
        LOAD, 0,   // 12
        ICONST, 1, // 14
        ISUB,      // 16
        CALL, 1,   // 17
        IMUL,      // 19
        RET,       // 20
                   //.DEF MAIN: ARGS=0, LOCALS=0
                   // PRINT FACT(1)
        ICONST, 5, // 21    <-- MAIN METHOD!
        CALL, 1,   // 23
        PRINT,     // 25
        HALT       // 26
    };
    vector<FunctionMetaData> fmd2{
        FunctionMetaData("main", 0, 0, 21),
        FunctionMetaData("factorial", 1, 0, 0)};

    VirtualMachine vm(code2, 0, fmd2);
    vm.exec(21);
}

int main()
{
    factorial();
    addFunction();

    return 0;
}
