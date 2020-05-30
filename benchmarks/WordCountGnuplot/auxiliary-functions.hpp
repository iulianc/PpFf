Words* splitInWords(std::string* line) {
    std::string lettres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    Words* words = new Words();
    size_t next_start = 0;
    do {
        size_t next_letter = line->find_first_of(lettres, next_start);
        if ( next_letter == std::string::npos ) break;
        next_start = line->find_first_not_of(lettres, next_letter+1);
        words->push_back( line->substr(next_letter, next_start - next_letter) );
    } while ( next_start != std::string::npos );

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
            printf( "Cas impossible: '%c'\n", c );
        }
    }
    return result;
}

Words* splitInLowerCaseWords(std::string* line) {
    std::string lettres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    Words* words = new Words();
    size_t next_start = 0;
    do {
        size_t next_letter = line->find_first_of(lettres, next_start);
        if ( next_letter == std::string::npos ) break;
        next_start = line->find_first_not_of(lettres, next_letter+1);
        std::string word = line->substr(next_letter, next_start - next_letter); 
        std::string* lc_word = toLowercaseLetters(&word);
        words->push_back( *lc_word );
    } while ( next_start != std::string::npos );

    return words;
}
