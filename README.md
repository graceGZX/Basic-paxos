# Basic-paxos
基于cpp的basic-paxos编写
+ 实现了多个节点之前对某个提案的共识
+ 通过使用atomic，实现多线程中对变量的原子操作

编译
--------------
g++ -std=c++11 -mcx16 *.cpp -lpthread -latomic -o paxos

运行
--------------------
./paxos
