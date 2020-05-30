Words* splitInWords(std::string* line) {
    std::string lettres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    Words* words = new Words();
    size_t next_start = 0;
    while ( next_start < line->length() ) {
        size_t next_letter = line->find_first_of(lettres, next_start);
        if ( next_letter >= line->length() ) break;
        next_start = line->find_first_not_of(lettres, next_letter+1);
        std::string word = line->substr(next_letter, next_start - next_letter); 
        words->push_back( word );
    };

    return words;
}

std::string* toLowercaseLetters(std::string* data) {
    std::for_each( data->begin(),
                   data->end(),
                   [](char& c) { c = ::tolower(c); } );

    return data;
}

Words* splitInLowerCaseWords(std::string* line) {
    std::string lettres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    Words* words = new Words();
    size_t next_start = 0;
    while ( next_start < line->length() ) {
        size_t next_letter = line->find_first_of(lettres, next_start);
        if ( next_letter >= line->length() ) break;
        next_start = line->find_first_not_of(lettres, next_letter+1);
        std::string word = line->substr(next_letter, next_start - next_letter); 
        words->push_back( *toLowercaseLetters(&word) );
    };

    return words;
}
