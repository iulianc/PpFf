Words* splitInWords(std::string* line) {
    std::string delimiter = " ";

    Words* words = new Words();
    size_t start = 0, end = 0;
    do {
        end = line->find(delimiter, start);
        size_t len = end - start;
        words->push_back( line->substr(start, len) );
        start += len + delimiter.length();
    } while (end != std::string::npos);

    return words;
}

std::string* toLowercaseLetters(std::string* data) {
    std::string* result = new std::string;
    for (char c: *data) {
        if ('a' <= c && c <= 'z') {
            result->push_back(c);
        } else if ('A' <= c && c <= 'Z') {
            result->push_back(c-('Z'-'z')); 
        } else {
            // NOOP: Le caractere est ignore/supprime!
        }
    }
    return result;
}

bool notEmpty(std::string* s) {
    return s->size() > 0;
}

Words* splitInNonEmptyLowerCaseWords(std::string* line) {
    std::string delimiter = " ";

    Words* words = new Words();
    size_t start = 0, end = 0;
    do {
        end = line->find(delimiter, start);
        size_t len = end - start;
        std::string word = line->substr(start, len);
        std::string* lc_word = toLowercaseLetters(&word);
        if (notEmpty(lc_word)) {
            words->push_back( *lc_word );
        }
        start += len + delimiter.length();
    } while (end != std::string::npos);

    return words;
}
