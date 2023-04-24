#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <random>

#include "proposer.h"

Proposer::Proposer(
    unsigned int minProposalNumber,
    unsigned int proposalNumbersIncrement,
    std::vector<std::shared_ptr<Acceptor>> acceptors)
        : currentProposalNumber(minProposalNumber),
          proposalNumbersIncrement(proposalNumbersIncrement),
          acceptors(acceptors)
{
    std::random_shuffle(this->acceptors.begin(), this->acceptors.end());
}

void Proposer::propose(int val)
{
    unsigned int n;
    bool exchanged;
    do {
        n = this->currentProposalNumber.load();
        exchanged = this->currentProposalNumber.compare_exchange_weak(n, n + this->proposalNumbersIncrement);
    } while (!exchanged);

    std::default_random_engine randomEngine(val);
    std::uniform_int_distribution<int> uniform_dist(1, 1000);

    // TODO: persist n.

    // Phase 1.
    int majority = this->acceptors.size() / 2 + 1;
    PrepareResponse highestNumberedResponse;
    highestNumberedResponse.alreadyAccepted = false;

    // TODO: parallelize.
    for (int i = 0; i < majority; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(uniform_dist(randomEngine)));
        PrepareResponse response = this->acceptors[i]->prepare(n);
        //如果prepare被拒绝
        if (!response.prepareAck) {
            std::cout << "Prepare failed!\n";
            return;
        }
        //先让high提案被接受，然后如果响应编号大于high提案编号，则替换
        if (response.alreadyAccepted
            && (!highestNumberedResponse.alreadyAccepted
                || response.prevProposalNumber > highestNumberedResponse.prevProposalNumber))
        {
            highestNumberedResponse = response;
        }
    }

    // Phase 2
    //之前没有接收过提案，则自由选择提案值
    int actualVal = highestNumberedResponse.alreadyAccepted ? highestNumberedResponse.prevValue : val;

    // TODO: parallelize;
    for (int i = 0; i < majority; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(uniform_dist(randomEngine)));
        this->acceptors[i]->accept(n, actualVal);
    }
}