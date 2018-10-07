#include "VirtualMachine.h"
#include "Bytecode.h"

#include <iostream>
#include <stdexcept>
#include <iomanip>

VirtualMachine::VirtualMachine(vector<int> code, int numGlobals, vector<FunctionMetaData> functionMeta) {
    this->code = code;
    vector<int>(numGlobals).swap(globals);
    vector<int>(DEFAULT_STACK_SIZE).swap(stack);
    this->metadata = functionMeta;
}

void VirtualMachine::exec(int startIp) {
    this->ip = startIp;
    ctx = make_shared<Context>(nullptr, 0, metadata[0]);
    simulateCpu();
}

// Simulate the fetch-decode-execute cycle
void VirtualMachine::simulateCpu() {
    int opcode = code[ip];
    int a, b, addr, regNum;
    while (opcode != HALT && ip < code.size()) {
        if(TRACE) cerr << disInstr();
        ip++;
        switch(opcode) {
            case IADD:
            {
                b = stack[sp--];
                a = stack[sp--];
                stack[++sp] = a + b;
                break;
            }
            case ISUB:
            {
                b = stack[sp--];
                a = stack[sp--];
                stack[++sp] = a - b;
                break;
            }
            case IMUL:
            {
                b = stack[sp--];
                a = stack[sp--];
                stack[++sp] = a * b;
                break;
            }
            case ILT:
            {
                b = stack[sp--];
                a = stack[sp--];
                stack[++sp] = a < b;
                break;
            }
            case IEQ:
            {
                b = stack[sp--];
                a = stack[sp--];
                stack[++sp] = a == b;
                break;
            }
            case BR:
            {
                ip = code[ip++];
                break;
            }
            case BRT:
            {
                addr = code[ip++];
                if (stack[sp--]) ip = addr;
                break;
            }
            case BRF:
            {
                addr = code[ip++];
                if (!stack[sp--]) ip = addr;
                break;
            }
            case ICONST:
            {
                stack[++sp] = code[ip++];
                break;
            }
            case LOAD:
            {
                regNum = code[ip++];
                stack[++sp] = ctx->locals[regNum];
                break;
            }
            case GLOAD:
            {
                addr = code[ip++];
                stack[++sp] = globals[addr];
                break;
            }
            case STORE:
            {
                regNum = code[ip++];
                ctx->locals[regNum] = stack[sp--];
                break;
            }
            case GSTORE:
            {
                addr = code[ip++];
                globals[addr] = stack[sp--];
                break;
            }
            case PRINT:
            {
                cout << stack[sp--] << endl;
                break;
            }
            case POP:
            {
                --sp;
                break;
            }
            case CALL:
            {
                // expects all args are already on the stack
                int funcIndex = code[ip++]; // index of the target function in fn metadata array
                int numArgs = metadata[funcIndex].numArgs;

                // set context to new context which points to: parent context, return address, function metadata
                ctx = make_shared<Context>(ctx, ip, metadata[funcIndex]); 

                // copy args from the stack into the new context
                int firstArg = sp - numArgs + 1;
                for(int i = 0; i < numArgs; i++) {
                    ctx->locals[i] = stack[firstArg + i];
                }
                sp -= numArgs; // args removed from the stack
                ip = metadata[funcIndex].address; // jump to the function location
                break;
            }
            case RET:
            {
                ip = ctx->returnIp;
                ctx = ctx->invokingContext;
                break;
            }
            default:
            {
                throw runtime_error("invalid opcode " + opcode +
                    string(" at ip=") + to_string(ip-1));
            }
        }
        if(TRACE) cerr << stringifyStack() << " " << stringifyCallStack() << endl;
        opcode = code[ip];
    }
    if(TRACE) cerr << disInstr();
    if(TRACE) cerr << stringifyStack() << endl;
    if(TRACE) cerr << stringifyDataMemory() << endl;
}

string VirtualMachine::disInstr() {
    int opcode = code[ip];
    string opName = BYTECODES[opcode].name;
    ostringstream ss;
    ss << setw(4) << ip << ":\t" << opName;
    int numArgs = BYTECODES[opcode].args;
    if (opcode == CALL) {
        ss << " " << metadata[code[ip+1]].name;
    }
    else if (numArgs > 0) {
        vector<string> operands;
        for(int i = ip+1; i <= ip+numArgs; i++) {
            operands.push_back(to_string(code[i]));
        }

        for(int i = 0; i < operands.size(); i++) {
            string s = operands[i];
            ss << " ";
            if (i > 0) ss << ", ";
            ss << s;
        }
    }
    return ss.str();
}

string VirtualMachine::stringifyStack() {
    return "";
}

string VirtualMachine::stringifyCallStack() {
    return "";
}

string VirtualMachine::stringifyDataMemory() {
    ostringstream ss;
    ss << "Data memory: ";
    int addr = 0;
    for (int o : globals) {
        ss << setw (4) << addr << ": " << o << endl;
        addr++;
    }
    return ss.str();
}
