# paxos-cpp
Toy implementation of Basic Paxos, on threads.

Compiled like this:
```
g++ -std=c++11 -mcx16 *.cpp -lpthread -latomic -o paxos
```

Fun demo, showing one round of Paxos, is run like this:
```
./paxos
```

The implementation doesn't have a distinguished proposer (leader) - all proposers fight with each other to propose a random value. Majority of acceptors always end up agreeing on a single of these random values.