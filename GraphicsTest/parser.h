#include "QPoint"
#include <vector>
#ifndef PARSER_H
#define PARSER_H


class Parser
{
public:
    Parser();
    ~Parser();
    void test();
    void skrivTilFil(std::vector<int> punkter);
    std::vector<int> parseFil();
};

#endif // PARSER_H
