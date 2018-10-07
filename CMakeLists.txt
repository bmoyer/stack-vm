cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_STANDARD 14)
project (stack-vm)
add_executable(stack-vm src/main.cpp src/VirtualMachine.cpp src/VirtualMachine.h src/Bytecode.h)
