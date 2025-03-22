
#include "test_programm_functions.h"

void OutputParseLine() {
    for(int i = 25; i > 0; --i){
        std::cerr << "*";
    }
    std::cerr<< std::endl;
}


void CheckExeptionOutput() {
    OutputParseLine();

    TestExeptionForConstructorWithStopWords();
    TestExeptionForDocumentsWithSameIndex();
    TestExeptionForDocumentsWithNegativeIndex();
    TestExeptionForDocumentsWithSpecialCharacters();
    TestExeptionForQueryWithSpecialCharacters();
    TestExeptionForQueryWithMinusWords();
    TestExeptionForGetDocumentId();

    OutputParseLine();
}

void TestSearchServer() {
    OutputParseLine();

    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestExcludeDocumentsIfMinusWords);
    RUN_TEST(TestSortAndCalculateByRelevance);
    RUN_TEST(TestCalculateRatingFromAddDocuments);
    RUN_TEST(TestOutDocumentsWithPredicate);
    RUN_TEST(TestOutDicumentWithQueryStatus);
    RUN_TEST(TestReturnStatusFromMatchDocument);
    
    OutputParseLine();
}

void TestPaginator() {
    OutputParseLine();

    RUN_TEST(TestCountOfPages);
    RUN_TEST(TestEndPage);

    OutputParseLine();
}

void TestRequestQueue() {
    OutputParseLine();

    RUN_TEST(TestFullQueueQueries);
    RUN_TEST(TestNoFullQueue);

    OutputParseLine();
}