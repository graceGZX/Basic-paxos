# paxos-cpp
+ 实现了多节点之间对某个提案的共识
+ 通过使用atmoic，实现了在多线程中对变量的原子操作

编译
---------------
g++ -std=c++11 -mcx16 *.cpp -lpthread -latomic -o paxos

运行
---------------
./paxos
