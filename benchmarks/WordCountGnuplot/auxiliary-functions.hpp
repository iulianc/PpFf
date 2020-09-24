Words* splitInWords(std::string* line) {
    Words* words = new Words();
    size_t start = 0, next_letter;
    while ( start < line->length() ) {
        for ( next_letter = start;
              next_letter < line->length() && !isalpha(line->at(next_letter));
              next_letter++ )
            {};
        if ( next_letter >= line->length() ) break;
        for ( start = next_letter + 1;
              start < line->length() && isalpha(line->at(start)); start++ )
            {};
        words->emplace_back( *line, next_letter, start - next_letter );
    };

    return words;
}

std::string* toLowercaseLetters(std::string* data) {
    std::for_each( data->begin(),
                   data->end(),
                   [](char& c) { c = ::tolower(c); } );

    return data;
}

const int GRAIN = 1000;

char tolower_(char c) {
    if ( ('A' <= c) && (c <= 'Z')) {
        for ( int i = 0; i < 32; i++ ) {
            c += 1;
        }
    } else {
        int r = c;
        for ( int i = 0; i < GRAIN; i++ ) {
            r += 1;
        }
        c = r - GRAIN;
    }
    return c;
}


std::string* toLowercaseLetters_(std::string* data) {
    std::for_each( data->begin(),
                   data->end(),
                   [](char& c) { c = tolower_(c); } );

    return data;
}

Words* splitInLowerCaseWords(std::string* line) {
    Words* words = new Words();
    size_t start = 0, next_letter;
    while ( start < line->length() ) {
        for ( next_letter = start;
              next_letter < line->length() && !isalpha(line->at(next_letter));
              next_letter++ )
            {};
        if ( next_letter >= line->length() ) break;
        for ( start = next_letter + 1;
              start < line->length() && isalpha(line->at(start)); start++ )
            {};
        words->emplace_back( *line, next_letter, start - next_letter );
        toLowercaseLetters(&words->back());
    };

    return words;
}
