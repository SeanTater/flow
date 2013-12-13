#ifndef ROW_H
#define ROW_H
#include <string>
#include <vector>
#include <boost/regex.hpp>
using namespace std;

class Row {
public:
    int id;
    Row(int, string, string, string);
    vector<string> words;
    vector<string> tags;
    void tokenize(string, boost::regex, vector<string>&);
};

#endif // ROW_H
