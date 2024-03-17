
#include "test_programm_functions.h"


void CheckExeptionOutput() {
    for(int i = 25; i > 0; --i){
        std::cout << "*";
    }
    std::cout << std::endl;
    TestExeptionForConstructorWithStopWords();
    TestExeptionForDocumentsWithSameIndex();
    TestExeptionForDocumentsWithNegativeIndex();
    TestExeptionForDocumentsWithSpecialCharacters();
    TestExeptionForQueryWithSpecialCharacters();
    TestExeptionForQueryWithMinusWords();
    TestExeptionForGetDocumentId();
    for(int i = 25; i > 0; --i){
        std::cout << "*";
    }
    std::cout << std::endl;
}

void TestSearchServer() {
    for(int i = 25; i > 0; --i){
        std::cout << "*";
    }
    std::cout << std::endl;
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestExcludeDocumentsIfMinusWords);
    RUN_TEST(TestSortAndCalculateByRelevance);
    RUN_TEST(TestCalculateRatingFromAddDocuments);
    RUN_TEST(TestOutDocumentsWithPredicate);
    RUN_TEST(TestOutDicumentWithQueryStatus);
    RUN_TEST(TestReturnStatusFromMatchDocument);
    for(int i = 25; i > 0; --i){
        std::cout << "*";
    }
    std::cout << std::endl;
}

void TestPaginator() {
    for(int i = 25; i > 0; --i){
        std::cout << "*";
    }
    std::cout << std::endl;

    RUN_TEST(TestCountOfPages);
    RUN_TEST(TestEndPage);

    for(int i = 25; i > 0; --i){
        std::cout << "*";
    }
    std::cout << std::endl;
}

void TestRequestQueue() {
    for(int i = 25; i > 0; --i){
        std::cout << "*";
    }
    std::cout << std::endl;

    RUN_TEST(TestFullQueueQueries);
    RUN_TEST(TestNoFullQueue);

    for(int i = 25; i > 0; --i){
        std::cout << "*";
    }
    std::cout << std::endl;
    
}