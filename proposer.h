#ifndef __PROPOSER_H__
#define __PROPOSER_H__

#include <vector>
#include <atomic>
#include <memory>

#include "acceptor.h"

class Proposer
{
public:
    Proposer(
        unsigned int minProposalNumber,
        unsigned int proposalNumbersIncrement,
        std::vector<std::shared_ptr<Acceptor>> acceptors);

    void propose(int val);

private:
    std::atomic<unsigned int> currentProposalNumber;
    unsigned int proposalNumbersIncrement;
    std::vector<std::shared_ptr<Acceptor>> acceptors;
};

#endif