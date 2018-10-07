#include <iostream>
#include "VirtualMachine.h"
#include "Bytecode.h"

using namespace std;

int main() {
    //vector<int> code{ICONST, 7, ICONST, 35, IADD, PRINT, HALT};
    vector<int> code{ICONST, 7, STORE, 0};
    vector<FunctionMetaData> fmd{FunctionMetaData("main", 0, 1, 0)};
    
    // factorial
    vector<int> code2{
        //.def factorial: ARGS=1, LOCALS=0	ADDRESS
//	IF N < 2 RETURN 1
			LOAD, 0,				// 0
			ICONST, 2,				// 2
			ILT,					// 4
			BRF, 10,				// 5
			ICONST, 1,				// 7
			RET,					// 9
//CONT:
//	RETURN N * FACT(N-1)
			LOAD, 0,				// 10
			LOAD, 0,				// 12
			ICONST, 1,				// 14
			ISUB,					// 16
			CALL, 1,	// 17
			IMUL,					// 19
			RET,					// 20
//.DEF MAIN: ARGS=0, LOCALS=0
// PRINT FACT(1)
			ICONST, 12,				// 21    <-- MAIN METHOD!
			CALL, 1,	// 23
			PRINT,					// 25
			HALT					// 26
    };
    vector<FunctionMetaData> fmd2{FunctionMetaData("main", 0, 0, 21), FunctionMetaData("factorial", 1, 0, 0)};

    VirtualMachine vm(code2, 0, fmd2);
    vm.exec(21);

    return 0;
}
