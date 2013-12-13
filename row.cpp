#include "row.h"

Row::Row(int id, string title, string body, string tags) {
    this->id = id;
    tokenize(title, boost::regex("\\W+|\\w+"), words);
    tokenize(body, boost::regex("\\W+|\\w+"), words);
    tokenize(tags, boost::regex("[^ ]+"), this->tags);
}

void Row::tokenize(string text, boost::regex pattern, vector<string> &tokens) {
    // Split the string (e.g. title or body into tokens)
    boost::sregex_token_iterator empty;
    boost::sregex_token_iterator tokens_in(text.begin(), text.end(), pattern);
    for (; tokens_in != empty; ++tokens_in) {
        tokens.push_back(*tokens_in);
    }

}
