
#include "request_queue.h"

RequestQueue::RequestQueue(const SearchServer& search_server) : search_server_(search_server) {}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
    return AddFindRequest(
        raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
            return document_status == status;
        });
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    return AddFindRequest(raw_query, DocumentStatus::ACTUAL);
}

int16_t RequestQueue::GetNoResultRequests() const {
    return invalid_results_;
}

void RequestQueue::AddEmptyResult(std::vector<Document>& buffer){
        if(buffer.empty()){
            ++invalid_results_;
        } else {
            return;
        }
    }