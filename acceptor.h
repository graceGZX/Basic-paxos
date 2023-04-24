#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include <atomic>

struct PrepareResponse
{
    bool prepareAck;
    bool alreadyAccepted;
    int prevValue;
    unsigned int prevProposalNumber;
};

struct AcceptorState
{
    PrepareResponse response;
    unsigned int minNumberToAccept;
};

class Acceptor
{
public:
    Acceptor(int id) : id(id) { this->state = AcceptorState { { true, false}, 0 }; }
    PrepareResponse prepare(unsigned int proposalNumber);
    bool accept(unsigned int proposalNumber, int value);
    PrepareResponse getAccepted();

private:
    int id;

    // NOTE: currently the struct contains 14 bytes, which requires special hardware to support
    // atomic instructions for 16-byte data. Consider keeping a shared_ptr instead.
    std::atomic<AcceptorState> state;
};

#endif