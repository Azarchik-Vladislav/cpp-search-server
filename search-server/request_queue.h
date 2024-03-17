#pragma once
#include <cstdint>
#include <deque>
#include <string>
#include <vector>
#include <utility>
#include "document.h"
#include "search_server.h"


class RequestQueue {
public:

    explicit RequestQueue(const SearchServer& search_server);
    
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        auto buffer = search_server_.FindTopDocuments(raw_query, document_predicate);
 
        if (requests_.size() != 1440) {
            AddEmptyResult(buffer);
            requests_.push_back({ buffer.size() });
        } else {
            if (requests_.front().count_finded_queries == 0) {
                --invalid_results_;
            }
            AddEmptyResult(buffer);
            requests_.pop_front();
            requests_.push_back({ buffer.size() });
        }

        return buffer;
    }

    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);

    int16_t GetNoResultRequests() const;
private:
    struct QueryResult {
        size_t count_finded_queries = 0;
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    size_t invalid_results_ = 0;
    const SearchServer& search_server_;
    
    void AddEmptyResult(std::vector<Document>& buffer);
};
