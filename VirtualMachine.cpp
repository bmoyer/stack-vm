#include <vector>

using namespace std;

class VirtualMachine {
    public:
        VirtualMachine::VirtualMachine();

        void VirtualMachine::run();

    private:
        unique_ptr<vector<int> stack>; // operand stack, grows upwards
        unique_ptr<vector<int>> code; // code memory
        unique_ptr<vector<int>> globals; // global variable space
};

