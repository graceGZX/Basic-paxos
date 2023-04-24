#ifndef __DEMO_H__
#define __DEMO_H__

#include <vector>
#include <memory>

#include "acceptor.h"
#include "proposer.h"

class Demo
{
public:
    Demo(int numOfNodes);
    void run();

private:
    std::vector<std::shared_ptr<Acceptor>> acceptors;
    std::vector<std::shared_ptr<Proposer>> proposers;
};

#endif