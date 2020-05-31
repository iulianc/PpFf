Words* splitInWords(std::string* line) {
    Words* words = new Words();
    size_t start = 0;
    while ( start < line->length() ) {
        size_t next_letter = start;
        while ( next_letter < line->length() && !isalpha(line->at(next_letter)) )
            next_letter += 1;
        if ( next_letter >= line->length() ) break;
        start = next_letter + 1;
        while ( start < line->length() && isalpha(line->at(start)) )
            start += 1;
        std::string word = line->substr(next_letter, start - next_letter); 
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
    Words* words = new Words();
    size_t start = 0;
    while ( start < line->length() ) {
        size_t next_letter = start;
        while ( next_letter < line->length() && !isalpha(line->at(next_letter)) )
            next_letter += 1;
        if ( next_letter >= line->length() ) break;
        start = next_letter + 1;
        while ( start < line->length() && isalpha(line->at(start)) )
            start += 1;
        std::string word = line->substr(next_letter, start - next_letter); 
        words->push_back( *toLowercaseLetters(&word) );
    };

    return words;
}
