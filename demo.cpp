#include <iostream>
#include <thread>
#include <random>

#include "demo.h"

Demo::Demo(int numOfNodes)
{
    for (int i = 0; i < numOfNodes; i++) {
        acceptors.push_back(std::make_shared<Acceptor>(i));
    }

    for (unsigned i = 0; i < numOfNodes; i++) {
        proposers.push_back(std::make_shared<Proposer>(i, (unsigned)numOfNodes, acceptors));
    }
}

void Demo::run()
{
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(1, 100);

    std::vector<std::thread> threads;
    for (auto proposer : proposers) {
        int x = uniform_dist(e1);
        threads.push_back(std::thread(&Proposer::propose, proposer, x));
    }

    for (std::thread& th : threads) {
        th.join();
    }

    std::cout << "\nFinal state of acceptors after one round of Paxos without a distinguished proposer:\n";
    for (int i = 0; i < this->acceptors.size(); i++) {
        auto r = this->acceptors[i]->getAccepted();
        std::cout << "Acceptor " << i << ": ";
        if (r.alreadyAccepted) {
            std::cout << "value: " << r.prevValue << ", last proposal number: " << r.prevProposalNumber << std::endl;
        }
        else {
            std::cout << "nothing\n";
        }
    }
}