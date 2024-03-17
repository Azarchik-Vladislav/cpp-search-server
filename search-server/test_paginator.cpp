#include "test_paginator.h"


using namespace std::string_literals;

void TestCountOfPages() {
    
    
    {
        SearchServer search_server("and with"s);
        search_server.AddDocument(1, "funny pet and nasty rat"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(2, "funny pet with curly hair"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
        search_server.AddDocument(3, "big cat nasty hair"s, DocumentStatus::ACTUAL, { 1, 2, 8 });
        search_server.AddDocument(4, "big dog cat Vladislav"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
        search_server.AddDocument(5, "big dog hamster Borya"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
        const auto search_results = search_server.FindTopDocuments("curly dog"s);

        int page_size = 2;
        
        const auto pages = Paginate(search_results, page_size);
        //Ожидаемое количестов страниц == 2
        ASSERT_EQUAL_HINT(pages.size(), 2, "Incorrect pagination"s);
    }

    {
        SearchServer search_server("and with"s);
        search_server.AddDocument(1, "funny pet and nasty rat"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(2, "funny pet with curly hair"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
        search_server.AddDocument(3, "big cat nasty hair"s, DocumentStatus::ACTUAL, { 1, 2, 8 });
        search_server.AddDocument(4, "big dog cat Vladislav"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
        search_server.AddDocument(5, "big dog hamster Borya"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
        const auto search_results = search_server.FindTopDocuments("curly dog"s);

        int page_size = 0;

        const auto pages = Paginate(search_results, page_size);
        //Ожидаемое количестов страниц == 0
        ASSERT_EQUAL_HINT(pages.size(), 0, "Incorrect pagination"s);
    }

    {
        SearchServer search_server("and with"s);
        search_server.AddDocument(1, "funny pet and nasty rat"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(2, "funny pet with curly hair"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
        search_server.AddDocument(3, "big cat nasty hair"s, DocumentStatus::ACTUAL, { 1, 2, 8 });
        search_server.AddDocument(4, "big dog cat Vladislav"s, DocumentStatus::BANNED, { 1, 3, 2 });
        search_server.AddDocument(5, "big dog hamster Borya"s, DocumentStatus::ACTUAL, { 1, 1, 1 });
        const auto search_results = search_server.FindTopDocuments("curly dog"s);
        
        int page_size = 2;

        const auto pages = Paginate(search_results, page_size);
        //Ожидаемое количестов страниц == 1
        ASSERT_EQUAL_HINT(pages.size(), 1, "Incorrect pagination"s);
    }
    
}

//Проверка случая, когда количество запросов в последней странице меньше размера самой страницы
void TestEndPage() {

    {
        SearchServer search_server("and with"s);
        search_server.AddDocument(1, "funny pet and nasty rat"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(3, "big cat nasty hair"s, DocumentStatus::ACTUAL, { 1, 2, 8 });
        search_server.AddDocument(4, "big dog cat Vladislav"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
        const auto search_results = search_server.FindTopDocuments("curly dog"s);

        int page_size = 2;

        const auto pages = Paginate(search_results, page_size);
        const auto page = *prev(pages.end());
        //Ожидаемое количество документов в странице == 1
        ASSERT_EQUAL_HINT(page.size_page(), 1, "Incorrect pagination"s);
    }

}