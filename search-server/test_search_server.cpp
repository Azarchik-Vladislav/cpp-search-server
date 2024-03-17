
#include "test_search_server.h"

using namespace std::string_literals;

void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const std::string content = "cat in the city"s;
    const std::vector<int> ratings = {1, 2, 3};
    {
        SearchServer server(""s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        
        const auto found_docs = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }

    {
        SearchServer server("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        
        ASSERT_HINT(server.FindTopDocuments("in"s).empty(),
                    "Stop words must be excluded from documents"s);
    }
};

// Тест проверяет, что поисковая система исключает стоп-слова при вводе поискового запроса


void TestExcludeDocumentsIfMinusWords() {
    const int doc_id = 42;
    const std::string content = "cat in the city"s;
    const std::vector<int> ratings = {1, 2, 3};
    
    {
        SearchServer server(""s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        
        ASSERT_HINT(server.FindTopDocuments("dog n the city -cat"s).empty(), 
                    "Documest's which contains minus words must exluded from result"s);
    }
    
    {
        SearchServer server(""s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        
        std::tuple<std::vector<std::string>, DocumentStatus> tmp = server.MatchDocument("dog in the city", doc_id);
        ASSERT_HINT(!std::get<std::vector<std::string>>(tmp).empty(), "You parse query wrong");
    }
    
    {
        SearchServer server(""s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        
        std::tuple<std::vector<std::string>, DocumentStatus> tmp = server.MatchDocument("dog in the city -cat", doc_id);
        ASSERT_HINT(std::get<std::vector<std::string>>(tmp).empty(), "You parse query with minus words");
        
        tmp = server.MatchDocument("dog -cat in the city ", doc_id);
        ASSERT_HINT(std::get<std::vector<std::string>>(tmp).empty(), "You parse query with minus words");
        
        tmp = server.MatchDocument("-cat dog in the city ", doc_id);
        ASSERT_HINT(std::get<std::vector<std::string>>(tmp).empty(), "You parse query with minus words");
    }
}

void TestSortAndCalculateByRelevance() {
    
    SearchServer server("и в на"s);
    server.AddDocument(0, "белый кот и модный ошейник"s, DocumentStatus::ACTUAL, {8, -3});
    server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
    server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL, {5, -12, 2, 1});
    server.AddDocument(3, "покрашен новый столб"s, DocumentStatus::ACTUAL, {5, -12, 2, 1});
    
    std::vector<Document> documents = server.FindTopDocuments("белый столб нем кот"s, DocumentStatus::ACTUAL);
        ASSERT_EQUAL(documents.size(), 3u);
    
    {
        size_t i = 1;
        while(i < documents.size()){
            ASSERT(documents[i-1].relevance >= documents[i].relevance);
            ++i;
        }
    }
    
    {
        double relevance_from_doc_id_0 = ((1. / 4) * log(4. / 2)) + ((1.0 / 4) * log(4.));
        ASSERT_EQUAL_HINT(relevance_from_doc_id_0, documents[0].relevance, "Incorrect calculate relevance"s );                                          
    } 
}

void TestCalculateRatingFromAddDocuments() {
    {
        SearchServer server(""s);
        server.AddDocument(0, "белый кот и модный ошейник"s, DocumentStatus::ACTUAL, {8, -20});
        const auto& iter = server.FindTopDocuments("кот"s, DocumentStatus::ACTUAL);
        ASSERT_EQUAL(iter[0].rating, -6); 
    } 
    
    {
       SearchServer server(""s);
        server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 4, 7});
        const auto& iter = server.FindTopDocuments("кот"s, DocumentStatus::ACTUAL);
        ASSERT_EQUAL(iter[0].rating, 6); 
    }
}

void TestOutDocumentsWithPredicate() {
    SearchServer server("и в на"s);
    server.AddDocument(0, "белый кот и модный ошейник"s, DocumentStatus::BANNED, {8, -2});
    server.AddDocument(1, "кот пушистый хвост"s, DocumentStatus::BANNED, {7, 2, 7});
    server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL, {5, -12, 2, 1});
    server.AddDocument(3, "покрашен новый столб"s, DocumentStatus::IRRELEVANT, {5, -1, 2, 1});

    ASSERT_EQUAL(server.FindTopDocuments("кот"s, [](int, DocumentStatus, int rating) { 
                                                                         return rating == 3;}).size(), 1u);
}

void TestOutDicumentWithQueryStatus() {
    SearchServer server("и в на"s);
    server.AddDocument(0, "белый кот и модный ошейник"s, DocumentStatus::BANNED, {8, -2});
    server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::BANNED, {7, 2, 7});
    server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL, {5, -12, 2, 1});
    server.AddDocument(3, "покрашен новый столб"s, DocumentStatus::IRRELEVANT, {5, -12, 2, 1});
    
    ASSERT_HINT(!server.FindTopDocuments("пёс"s).empty(), "Incorrect handling of a case with no status"s);
    ASSERT_HINT(!server.FindTopDocuments("кот"s, DocumentStatus::BANNED).empty(),"Incorrect case processing indicating document status"s);
    ASSERT_HINT(server.FindTopDocuments("кот"s, DocumentStatus::REMOVED).empty(), "Incorrect handling of the case when there is no document with the requested status"s);
}

void TestReturnStatusFromMatchDocument() {
    SearchServer server("и в на"s);
    server.AddDocument(0, "белый пёс и модный ошейник"s, DocumentStatus::BANNED, {8, -2});
    server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
    
    std::tuple<std::vector<std::string>, DocumentStatus> tmp = server.MatchDocument("пёс в городе", 0);
    
    ASSERT_HINT((std::get<DocumentStatus>(tmp) == DocumentStatus::BANNED), "Don't return status id"s);

}