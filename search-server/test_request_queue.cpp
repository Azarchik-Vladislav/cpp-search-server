#include "test_request_queue.h"

using namespace std::string_literals;

void TestFullQueueQueries() {
    {
        SearchServer search_server("and in at"s);
        RequestQueue request_queue(search_server);
        search_server.AddDocument(1, "curly cat curly tail"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(2, "curly dog and fancy collar"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
        search_server.AddDocument(3, "big cat fancy collar "s, DocumentStatus::ACTUAL, { 1, 2, 8 });
        search_server.AddDocument(4, "big dog sparrow Eugene"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
        search_server.AddDocument(5, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
        
        // 1439 запросов с нулевым результатом
        for (int i = 0; i < 1441; ++i) {
            request_queue.AddFindRequest("empty request"s);
        }

        ASSERT_EQUAL_HINT(request_queue.GetNoResultRequests(), 1440, "Incorrect handling of the all cases no result"s);
    }

    {
        SearchServer search_server("and in at"s);
        RequestQueue request_queue(search_server);
        search_server.AddDocument(1, "curly cat curly tail"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(2, "curly dog and fancy collar"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
        search_server.AddDocument(3, "big cat fancy collar "s, DocumentStatus::ACTUAL, { 1, 2, 8 });
        search_server.AddDocument(4, "big dog sparrow Eugene"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
        search_server.AddDocument(5, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
        // 1439 запросов с нулевым результатом
        for (int i = 0; i < 1441; ++i) {
            request_queue.AddFindRequest("big dog"s);
        }

        ASSERT_EQUAL_HINT(request_queue.GetNoResultRequests(), 0, "Incorrect handling of the all cases with non empty result"s);
    }

    {
        SearchServer search_server("and in at"s);
        RequestQueue request_queue(search_server);
        search_server.AddDocument(1, "curly cat curly tail"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(2, "curly dog and fancy collar"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
        search_server.AddDocument(3, "big cat fancy collar "s, DocumentStatus::ACTUAL, { 1, 2, 8 });
        search_server.AddDocument(4, "big dog sparrow Eugene"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
        search_server.AddDocument(5, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
 
        for (int i = 0; i < 1439; ++i) {
             request_queue.AddFindRequest("big collar"s);
        }
        request_queue.AddFindRequest("curly dog"s);
        request_queue.AddFindRequest("big collar"s);
        request_queue.AddFindRequest("no result"s);

        ASSERT_EQUAL_HINT(request_queue.GetNoResultRequests(), 1, "Incorrect handling of the case with no result at the end of the queue"s);
    }

    {
        SearchServer search_server("and in at"s);
        RequestQueue request_queue(search_server);
        search_server.AddDocument(1, "curly cat curly tail"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(2, "curly dog and fancy collar"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
        search_server.AddDocument(3, "big cat fancy collar "s, DocumentStatus::ACTUAL, { 1, 2, 8 });
        search_server.AddDocument(4, "big dog sparrow Eugene"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
        search_server.AddDocument(5, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, { 1, 1, 1 });

        request_queue.AddFindRequest("no result"s);
        for (int i = 0; i < 1439; ++i) {
             request_queue.AddFindRequest("big collar"s);
        }

        ASSERT_EQUAL_HINT(request_queue.GetNoResultRequests(), 1, "Incorrect handling of the case with no result at the begin of the queue"s);
    }

    {
        SearchServer search_server("and in at"s);
        RequestQueue request_queue(search_server);
        search_server.AddDocument(1, "curly cat curly tail"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(2, "curly dog and fancy collar"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
        search_server.AddDocument(3, "big cat fancy collar "s, DocumentStatus::ACTUAL, { 1, 2, 8 });
        search_server.AddDocument(4, "big dog sparrow Eugene"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
        search_server.AddDocument(5, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
        
        for (int i = 0; i <= 1000; ++i) {
             request_queue.AddFindRequest("big collar"s);
        }

        for (int i = 0; i < 100; ++i) {
             request_queue.AddFindRequest("no result"s);
        }

        for (int i = 0; i < 339; ++i) {
             request_queue.AddFindRequest("big collar"s);
        }

        ASSERT_EQUAL_HINT(request_queue.GetNoResultRequests(), 100, "Incorrect processing of a request without result in the middle of the queue"s);
    }
}

void TestNoFullQueue() {
    {
        SearchServer search_server("and in at"s);
        RequestQueue request_queue(search_server);
        search_server.AddDocument(1, "curly cat curly tail"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(2, "curly dog and fancy collar"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
        search_server.AddDocument(3, "big cat fancy collar "s, DocumentStatus::ACTUAL, { 1, 2, 8 });
        search_server.AddDocument(4, "big dog sparrow Eugene"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
        search_server.AddDocument(5, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
        
        for (int i = 0; i <= 500; ++i) {
             request_queue.AddFindRequest("big collar"s);
        }

        for (int i = 0; i < 100; ++i) {
             request_queue.AddFindRequest("no result"s);
        }

        for (int i = 0; i < 339; ++i) {
             request_queue.AddFindRequest("big collar"s);
        }

        ASSERT_EQUAL_HINT(request_queue.GetNoResultRequests(), 100, "Incorrect processing of a request without result in the middle of the no full queue"s);
    }

    {
        SearchServer search_server("and in at"s);
        RequestQueue request_queue(search_server);
        search_server.AddDocument(1, "curly cat curly tail"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(2, "curly dog and fancy collar"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
        search_server.AddDocument(3, "big cat fancy collar "s, DocumentStatus::ACTUAL, { 1, 2, 8 });
        search_server.AddDocument(4, "big dog sparrow Eugene"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
        search_server.AddDocument(5, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, { 1, 1, 1 });

        for (int i = 0; i < 100; ++i) {
             request_queue.AddFindRequest("no result"s);
        }

        for (int i = 0; i < 339; ++i) {
             request_queue.AddFindRequest("big collar"s);
        }

        ASSERT_EQUAL_HINT(request_queue.GetNoResultRequests(), 100, "Incorrect processing of a request without result in the begin of the no full queue"s);
    }

    {
        SearchServer search_server("and in at"s);
        RequestQueue request_queue(search_server);
        search_server.AddDocument(1, "curly cat curly tail"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(2, "curly dog and fancy collar"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
        search_server.AddDocument(3, "big cat fancy collar "s, DocumentStatus::ACTUAL, { 1, 2, 8 });
        search_server.AddDocument(4, "big dog sparrow Eugene"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
        search_server.AddDocument(5, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, { 1, 1, 1 });

        for (int i = 0; i <= 500; ++i) {
             request_queue.AddFindRequest("big collar"s);
        }

        for (int i = 0; i < 100; ++i) {
             request_queue.AddFindRequest("no result"s);
        }

        ASSERT_EQUAL_HINT(request_queue.GetNoResultRequests(), 100, "Incorrect processing of a request without result in the end of the no full queue"s);
    }

    {
        SearchServer search_server("and in at"s);
        RequestQueue request_queue(search_server);
        search_server.AddDocument(1, "curly cat curly tail"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(2, "curly dog and fancy collar"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
        search_server.AddDocument(3, "big cat fancy collar "s, DocumentStatus::ACTUAL, { 1, 2, 8 });
        search_server.AddDocument(4, "big dog sparrow Eugene"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
        search_server.AddDocument(5, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, { 1, 1, 1 });

        for (int i = 0; i <= 500; ++i) {
             request_queue.AddFindRequest("big collar"s);
        }

        for (int i = 0; i < 100; ++i) {
             request_queue.AddFindRequest("no result"s);
        }

        ASSERT_EQUAL_HINT(request_queue.GetNoResultRequests(), 100, "Incorrect processing of a request without result in the end of the no full queue"s);
    }

    {
        SearchServer search_server("and in at"s);
        RequestQueue request_queue(search_server);
        
        search_server.AddDocument(5, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
        
        request_queue.AddFindRequest("big collar"s);

        ASSERT_EQUAL_HINT(request_queue.GetNoResultRequests(), 0, "Incorrect processing of a request with one non empty result in queue"s);
    }

    {
        SearchServer search_server("and in at"s);
        RequestQueue request_queue(search_server);
        
        search_server.AddDocument(5, "big dog sparrow Vasiliy"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
        
        request_queue.AddFindRequest("no result"s);

        ASSERT_EQUAL_HINT(request_queue.GetNoResultRequests(), 1, "Incorrect processing of a request with one no result in queue"s);
    }
    
}