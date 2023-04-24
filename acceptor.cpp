#include <iostream>
#include <cassert>
#include <atomic>

#include "acceptor.h"

PrepareResponse Acceptor::prepare(unsigned int proposalNumber)
{
    std::cout << "Acceptor " << id << ": prepare " << proposalNumber << std::endl;
    bool exchanged = false;
    AcceptorState currentState;
    do {
        currentState = this->state.load();
        if (proposalNumber >= currentState.minNumberToAccept) {
            AcceptorState newState = currentState;
            newState.minNumberToAccept = proposalNumber;
            //如果state等于curr,则将state替换为new;
            //如果不一样,将cur修改为state,返回false
            exchanged = this->state.compare_exchange_weak(currentState, newState);
        }
        else {
            // We can ignore this request or respond with a message NACK.
            PrepareResponse nack;
            nack.prepareAck = false;
            return nack;
        }
    } while (!exchanged);

    return currentState.response;
}

bool Acceptor::accept(unsigned int proposalNumber, int value)
{
    std::cout << "Acceptor " << id << ": accept " << proposalNumber << ", value: " << value << std::endl;
    bool exchanged = false;
    AcceptorState currentState = this->state.load();
    assert(currentState.minNumberToAccept >= proposalNumber);

    bool accepted = false;
    if (currentState.minNumberToAccept == proposalNumber) {
        AcceptorState newState = currentState;
        newState.response.alreadyAccepted = true;
        newState.response.prevValue = value;
        newState.response.prevProposalNumber = proposalNumber;
        accepted = this->state.compare_exchange_strong(currentState, newState);
    }

    if (accepted) {
        std::cout << "Acceptor " << this->id << " has accepted (" << proposalNumber << ", " << value << ")\n";
    }
    else {
        std::cout << "Acceptor " << this->id << " has NOT accepted (" << proposalNumber << ", " << value << ")\n";
    }

    // Instead of returning false, we could also just ignore the message and don't respond.
    return accepted;
}

PrepareResponse Acceptor::getAccepted()
{
    return this->state.load().response;
}