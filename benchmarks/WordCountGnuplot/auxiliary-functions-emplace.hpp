Words* splitInWords_(std::string* line) {
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

Words* splitInLowerCaseWords_(std::string* line) {
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
