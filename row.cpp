#include "row.h"

Row::Row(int id, string title, string body, string tags) {
    this->id = id;
    tag_string = tags;
    tokenize(title, boost::regex("[^\\w\\s]+|\\w+"), words, string(""));
    tokenize(body, boost::regex("[^\\w\\s]+|\\w+"), words, string(""));
    words_with_tags = words;
    tokenize(tags, boost::regex("[^ ]+"), words_with_tags, string("tag: "));
    tokenize(tags, boost::regex("[^ ]+"), this->tags, string(""));
    //words.push_back(string("tag: 0"));
}

Row::Row(int id, string title, string body) {
    this->id = id;
    tokenize(title, boost::regex("[^\\w\\s]+|\\w+"), words, string(""));
    tokenize(body, boost::regex("[^\\w\\s]+|\\w+"), words, string(""));
}

void Row::tokenize(string text, boost::regex pattern, vector<string> &tokens, string prefix) {
    // Split the string (e.g. title or body into tokens)
    boost::sregex_token_iterator empty;
    boost::sregex_token_iterator tokens_in(text.begin(), text.end(), pattern);
    for (; tokens_in != empty; ++tokens_in) {
        tokens.push_back(prefix + (*tokens_in) );
    }

}
