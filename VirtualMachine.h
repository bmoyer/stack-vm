#include <vector>
#include <memory>

using namespace std;

typedef struct FunctionMetaData
{
    string name;
    int numArgs;
    int numLocals;
    int address;
    FunctionMetaData(string n, int na, int nl, int addr) :
        name(n), numArgs(na), numLocals(nl), address(addr) {};
} FunctionMetaData;

typedef struct Context {
    shared_ptr<Context> invokingContext; // parent in the stack, i.e. the caller
    FunctionMetaData metadata; // info about the function we're executing
    int returnIp; // ip location to return to once function returns
    vector<int> locals; // args + locals, indexed from 0

    Context(shared_ptr<Context> ic, int rip, FunctionMetaData fmd) :
        invokingContext(ic), returnIp(rip), metadata(fmd) {
            vector<int>(metadata.numArgs + metadata.numLocals).swap(locals);
        };
} Context;

class VirtualMachine
{
  public:
    VirtualMachine(vector<int> code, int numGlobals, vector<FunctionMetaData> functionMeta);
    void exec(int startIp);

  private:
    void simulateCpu();

    string disInstr();
    string stringifyStack();
    string stringifyCallStack();
    string stringifyDataMemory();

    // Memory
    vector<int> stack;   // operand stack, grows upwards
    vector<int> code;    // code memory
    vector<int> globals; // global variable space
    shared_ptr<Context> ctx; // active context

    vector<FunctionMetaData> metadata;

    // Registers
    int ip;      // instruction pointer register
    int sp = -1; // stack pointer register

    const int DEFAULT_STACK_SIZE = 1000;
    const int DEFAULT_CALL_STACK_SIZE = 1000;
    const bool TRACE = false;
};
