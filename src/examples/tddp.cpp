

// top down operator precedence parser source file example

#include "mee.hpp"

#include <iostream>

int main(int argc, char **argv)
{
    if (argc == 1) return 1;

    for (int i = 1; i < argc; i++)
    {
        std::string parg = argv[i];
        mee::common::lexer l1(parg);
        l1.tokenize();
        std::vector<mee::common::node> v1(l1.get_nodes());
        mee::tddp::expr e1(v1);
        
        double result = e1.parse();
        std::cout << "result: " << result << '\n';
    }
}

