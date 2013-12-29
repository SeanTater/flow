#ifndef ROW_H
#define ROW_H
#include <string>
#include <vector>
#include <string>
#include <boost/regex.hpp>
using namespace std;

class Row {
public:
    int id;
    string tag_string;
    Row(int, string, string, string);
    Row(int, string, string);
    vector<string> words;
    vector<string> tags;
    void tokenize(string text, boost::regex pattern, vector<string> &tokens, string prefix);
};

#endif // ROW_H
