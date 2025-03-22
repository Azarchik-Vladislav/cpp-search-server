#include "document.h"


Document::Document() = default;

Document::Document(int id, double relevance, int rating)
    : id(id)
    , relevance(relevance)
    , rating(rating) {
}

using namespace std::literals::string_literals;

std::ostream& operator<<(std::ostream& out, Document document) {
    out << "{ "s;
    out << "document_id = "s << document.id << ", "s;
    out << "relevance = "s << document.relevance << ", "s;
    out << "rating = "s << document.rating << " }";

    return out;
}