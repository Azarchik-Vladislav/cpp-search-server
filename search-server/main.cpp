#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#define EPSILON 1e-6

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    Document() = default;

    Document(int id, double relevance, int rating)
        : id(id)
        , relevance(relevance)
        , rating(rating) {
    }

    int id = 0;
    double relevance = 0.0;
    int rating = 0;
};

static bool IsValidWords(const string& word) {
    // A valid word must not contain special characters
    return none_of(word.begin(), word.end(), [](char c) {
        return c >= '\0' && c < ' ';
        });
}

static bool IsValidWord(const string& word) {
    return word.empty() || word.at(0) == '-';
}

template <typename StringContainer>
set<string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    set<string> non_empty_strings;
    
    for (const string& str : strings) {
        if(!IsValidWords(str)){
            throw invalid_argument("Список стоп-слов содержит запрещенные символы"s);
        }
        
        if (!str.empty()) {
            non_empty_strings.insert(str);
        }
    }
    
    return non_empty_strings;
}

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
public:
    template <typename StringContainer>
    
    explicit SearchServer(const StringContainer& stop_words)
        : stop_words_(MakeUniqueNonEmptyStrings(stop_words)) {
    }

    explicit SearchServer(const string& stop_words_text)
        : SearchServer(SplitIntoWords(stop_words_text)) {
            if(!IsValidWords(stop_words_text)){
               throw invalid_argument("Список стоп-слов содержит запрещенные символы"s); 
            }      
    }

    void AddDocument(int document_id, const string& document, DocumentStatus status,
                                   const vector<int>& ratings) {
        vector<string> words;

        if (document_id < 0){
            throw invalid_argument ("Документ не был добавлен, так как его id отрицательный"s);
        }
        
        if(documents_.contains(document_id)){
            throw invalid_argument ("Документ не был добавлен, так как его id совпадает с уже имеющимся"s);
        }
        
        if(!SplitIntoWordsNoStop(document, words)){
            throw invalid_argument ("Документ не был добавлен, так как содержит спецсимволы"s);
        }
        
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        
        documents_.emplace(document_id, DocumentData{ ComputeAverageRating(ratings), status });
        id_documents_.push_back(document_id);
    }

    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& raw_query,
        DocumentPredicate document_predicate) const {
        Query query;

        if (!ParseQuery(raw_query, query)) { 
             throw invalid_argument("Запрос содержит спецсимволы или некорректные искючения для поиска документов"s);
       }
           
      vector<Document> result = FindAllDocuments(query, document_predicate);
      sort(result.begin(), result.end(), [](const Document& lhs, const Document& rhs) {
                if (abs(lhs.relevance - rhs.relevance) < EPSILON) {
                        return lhs.rating > rhs.rating;
                    } else {
                        return lhs.relevance > rhs.relevance;
                    }
       });

       if (result.size() > MAX_RESULT_DOCUMENT_COUNT) {
            result.resize(MAX_RESULT_DOCUMENT_COUNT);
       }
        
       return result;
 
}

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status 
                                                               = DocumentStatus::ACTUAL) const {
        return FindTopDocuments(raw_query, [status](int document_id,
                                                    DocumentStatus document_status,
                                                    int rating) {
                return document_status == status;
            });

    }

   tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const { 
       Query query;
       
       if (!ParseQuery(raw_query, query)) { 
             throw invalid_argument("Запрос содержит спецсимволы или некорректные искючения для поиска документов"s);
       }
 
      vector<string> matched_words;     
      for (const string& word : query.plus_words) {
          if (word_to_document_freqs_.count(word) == 0) {
              continue;
          }
          
          if (word_to_document_freqs_.at(word).count(document_id)) {
                  matched_words.push_back(word);
          }
      }

      for (const string& word : query.minus_words) {
          if (word_to_document_freqs_.count(word) == 0) {
              continue;
          }
          
          if (word_to_document_freqs_.at(word).count(document_id)) {
              matched_words.clear();
              break;
          }
      }
       
      return {matched_words, documents_.at(document_id).status};
  }
    
  int GetDocumentCount() const {
      return documents_.size();
  }

  int GetDocumentId(int index) const {
      if (index < 0 || index >= id_documents_.size()) {
          throw out_of_range("Введен некорректный индекс"s);
      }
      
     return id_documents_[index];
  }
private:
   struct DocumentData {
       int rating;
       DocumentStatus status;
   };

    const set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;
    vector<int> id_documents_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    bool SplitIntoWordsNoStop(const string& text, vector<string>& words) const {
        if (IsValidWords(text)) {
            for (const string& word : SplitIntoWords(text)) {
                if (!IsStopWord(word)) {
                    words.push_back(word);
                }
            }
            return true;
        }

        return false;
    }

    static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        
        int rating_sum = accumulate(ratings.begin(), ratings.end(), 0);
        return rating_sum / static_cast<int>(ratings.size());
    }

    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return { text, is_minus, IsStopWord(text) };
    }

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    bool ParseQuery(const string& text, Query& query) const {
       
        if (IsValidWords(text)) {
            int current = 0;
            for (const string& word : SplitIntoWords(text)) {
                const QueryWord query_word = ParseQueryWord(word);
                
                if (!query_word.is_stop) {
                    if (!IsValidWord(query_word.data) && query_word.is_minus) {
                        query.minus_words.insert(query_word.data);
                    } else if (!query_word.is_minus){
                        query.plus_words.insert(query_word.data);
                    } else {
                        ++current;
                        break;
                    }
                }
            }
            return current == 0;
        }
       
       return false; 
    }

    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query,
        DocumentPredicate document_predicate) const {
        
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto& [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                
                const auto& document_data = documents_.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating)) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto& [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto& [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back(
                { document_id, relevance, documents_.at(document_id).rating });
        }
        
        return matched_documents;
    }

    
};

/*Для тестировании системы работы исключений мной было принято решение создать специальные "тестирующие функции".*/
/*Работа функций основана на ожидании выдачи исключения без использования сравнения, т.к. сравнивать нечего.*/
/*В большинсвте случаев тестирования используется переменная "number_of_unsuccessful_exceptions_thrown" выступающая в 
качестве счетчика некорректных обработок исключения, в случае, если хотя бы один из кейсов увеличит значение счетчика, 
то сообщение о прохождении теста выведено не будет, вместо этого будет использован макрос, дающий подсказку в функции
и какой строке произошла ошибка для каждого из некорректно исполненных кейсов.*/

#define INCORRECT_EXEPTION_HANDLING std::cerr << "Incorrect exception handling for function: "s << __FUNCTION__ << ": " << __LINE__ << std::endl;

void TestExeptionForConstructorWithStopWords() {
    try {
        int number_of_unsuccessful_exceptions_thrown = 0;
        
        try {
            std::vector<string> stop_words = {"-\x12"s, "в", "на"s};
        
            SearchServer search_server(stop_words);    
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (invalid_argument& error) {}
        
        try {
            std::vector<string> stop_words = {"и"s, "в", "ск\x12рец"s};
        
            SearchServer search_server(stop_words);    
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (invalid_argument& error) {}
        
        try {
            std::vector<string> stop_words = {"и"s, "в ", "      \x12  "s};
        
            SearchServer search_server(stop_words);    
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (invalid_argument& error) {}
        
        throw number_of_unsuccessful_exceptions_thrown;
        
    } catch (const int& number_of_unsuccessful_exceptions_thrown){ 
        if(number_of_unsuccessful_exceptions_thrown == 0){
            std::cerr << "Exception output test for constructor with container parameter completed successfully"s 
                      << std::endl;
        }   
    }
    
    try{
        int number_of_unsuccessful_exceptions_thrown = 0;
        
        try {
            SearchServer search_server("и в \x12"s);
            INCORRECT_EXEPTION_HANDLING;
        } catch (const invalid_argument& error) {}
        
        try {
            SearchServer search_server("и в           ск\x12рец"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        try {
            SearchServer search_server("-\x12 и в"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        throw number_of_unsuccessful_exceptions_thrown;
        
    } catch (const int& number_of_unsuccessful_exceptions_thrown){ 
        if(number_of_unsuccessful_exceptions_thrown == 0){
            std::cerr << "Exception output test for constructor with string parameter completed successfully"s 
                      << std::endl;
        }   
    }
        
}

void TestExeptionForDocumentsWithSameIndex() {
    SearchServer search_server("и в на"s);
    search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
    
    try {
        
        search_server.AddDocument(1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2});
        INCORRECT_EXEPTION_HANDLING;
    } catch (const invalid_argument& error) {
        std::cerr << "Exception output test for documents with same index completed successfully"s 
                  << std::endl;
    }    
}

void TestExeptionForDocumentsWithNegativeIndex() { 
    try {
        SearchServer search_server("и в на"s);
        int number_of_unsuccessful_exceptions_thrown = 0;
            
        try {
            search_server.AddDocument(-2147483648, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7}); 
            INCORRECT_EXEPTION_HANDLING;
                
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
           
        try { 
            search_server.AddDocument(-1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2});
            INCORRECT_EXEPTION_HANDLING;
                
            ++number_of_unsuccessful_exceptions_thrown;
         } catch (const invalid_argument& error) {}
        
        throw number_of_unsuccessful_exceptions_thrown;
            
    } catch (const int& number_of_unsuccessful_exceptions_thrown) {
        if(number_of_unsuccessful_exceptions_thrown == 0){
            std::cerr << "Exception output test for all documents with negative index completed successfully"s 
                      << std::endl;
        }   
    }
}
void TestExeptionForDocumentsWithSpecialCharacters() {
    try{
        SearchServer search_server("и в на"s);
        int number_of_unsuccessful_exceptions_thrown = 0;
        
        try {
            search_server.AddDocument(15, "пушистый кот пуш\x00тый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7}); 
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        try {
            search_server.AddDocument(12, "\x16  пушистый        кот пуштый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7}); 
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        try {
            search_server.AddDocument(45, "пушистый кот пуштый хвост        \x15"s, DocumentStatus::ACTUAL, {7, 2, 7}); 
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        throw number_of_unsuccessful_exceptions_thrown;
        
    } catch (const int& number_of_unsuccessful_exceptions_thrown) {
        if(number_of_unsuccessful_exceptions_thrown == 0){
            std::cerr << "Exception output test for all documents with special characters completed successfully"s 
                      << std::endl;
        }   
    }
    
}

void TestExeptionForQueryWithSpecialCharacters() {
    try {
        SearchServer search_server("и в на"s);
        int number_of_unsuccessful_exceptions_thrown = 0;
        
        try {
            search_server.FindTopDocuments("кот выглядывает в окно    \x05"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        try {
            search_server.FindTopDocuments("кот выгл\x01дывает в окно"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        try {
            search_server.FindTopDocuments("\x12          кот выглядывает в окно"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        try {
            search_server.FindTopDocuments("           \x12           "s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        throw number_of_unsuccessful_exceptions_thrown;
        
    } catch (const int& number_of_unsuccessful_exceptions_thrown){
        if(number_of_unsuccessful_exceptions_thrown == 0){
            std::cerr << "Exception output test for all queryes with special characters completed successfully"s 
                      << std::endl;
        }
    } 
}

//В данном тесте осуществляется проверка сразу двух методов
// FindTopDocuments и MatchDocument
void TestExeptionForQueryWithMinusWords() {
    try {
        SearchServer search_server("и в на"s);
        int number_of_unsuccessful_exceptions_thrown = 0;
        search_server.AddDocument(1, "пушистый кот пуштый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7}); 
        //FindTopDocuments
        try {
            search_server.FindTopDocuments("пушистый --кот выглядывает в -окно"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        try {
            search_server.FindTopDocuments("кот выглядывает в --окно"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        try {
            search_server.FindTopDocuments("--кот выглядывает в окно"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        try {
            search_server.FindTopDocuments("кот выглядывает в -"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        //MatchDocuments
        try {
            search_server.MatchDocument("пушистый --кот выглядывает в -окно"s, 1);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        try {
            search_server.MatchDocument("кот выглядывает в --окно"s, 1) ;
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        try {
            search_server.MatchDocument("--кот выглядывает в окно"s, 1);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        try {
            search_server.MatchDocument("кот выглядывает в -"s, 1);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch (const invalid_argument& error) {}
        
        //В данном случае осуществляется обратная проверка, где, в случае
        //наличия в середине слова дефиса, исключение срабатывать не должно
        try {
            search_server.MatchDocument("Кот-Леопольд выглядывает в окно"s, 1);
        } catch (const invalid_argument& error) {
            INCORRECT_EXEPTION_HANDLING;
            ++number_of_unsuccessful_exceptions_thrown;
        }
        
        try {
            search_server.MatchDocument("Кот----Леопольд выглядывает в окно"s, 1);
        } catch (const invalid_argument& error) {
            INCORRECT_EXEPTION_HANDLING;
            ++number_of_unsuccessful_exceptions_thrown;
        }
        
        try {
            search_server.MatchDocument("Кот-Леопольд выглядывает- в окно"s, 1);
        } catch (const invalid_argument& error) {
            INCORRECT_EXEPTION_HANDLING;
            ++number_of_unsuccessful_exceptions_thrown;
        }
        
        throw number_of_unsuccessful_exceptions_thrown;
        
    } catch(const int& number_of_unsuccessful_exceptions_thrown) {
        if(number_of_unsuccessful_exceptions_thrown == 0){
            std::cerr << "Exception output test for all queryes with minus words completed successfully"s 
                      << std::endl;
        }
    }
}

void TestExeptionForGetDocumentId(){
    try{
        SearchServer search_server("и в на"s);
        
        search_server.AddDocument(32, "пушистый кот пуштый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
        search_server.GetDocumentId(32);
        
        INCORRECT_EXEPTION_HANDLING;
    } catch (const out_of_range& e) {
       std::cerr << "Exception output test for function \"GetDocumentId()\" completed successfully"s
                 << std::endl;
   }
}

void CheckExeptionOutput() {
    TestExeptionForConstructorWithStopWords();
    TestExeptionForDocumentsWithSameIndex();
    TestExeptionForDocumentsWithNegativeIndex();
    TestExeptionForDocumentsWithSpecialCharacters();
    TestExeptionForQueryWithSpecialCharacters();
    TestExeptionForQueryWithMinusWords();
    TestExeptionForGetDocumentId();
}

void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating << " }"s << endl;
}

int main() {
    CheckExeptionOutput();
    try{
        SearchServer search_server("и в на"s);
        search_server.AddDocument(32, "пушистый кот пуштый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
        search_server.AddDocument(15, "пушистый Кот-Леопольд смоторит в окно"s, DocumentStatus::BANNED, {8, 2, 4});
        search_server.AddDocument(17, "пушистый пёс ходит по городу"s, DocumentStatus::ACTUAL, {1, 2, 3});
        search_server.AddDocument(19, "Нью-Йорк город, который никогда не спит"s, DocumentStatus::ACTUAL, {-5, 12, -9});

        cout << "ACTUAL by default:"s << endl; 
        for (const Document& document : search_server.FindTopDocuments("пушистый ухоженный кот"s)) { 
            PrintDocument(document); 
        } 
        cout << "BANNED:"s << endl; 
        for (const Document& document : search_server.FindTopDocuments("пушистый ухоженный Кот-Леопольд"s, 
                                                                       DocumentStatus::BANNED)) { 
            PrintDocument(document); 
        } 
        cout << "Even ids:"s << endl; 
        for (const Document& document : search_server.FindTopDocuments("пушистый ухоженный кот"s, [](int document_id, 
                                                                                                     DocumentStatus status, 
                                                                                                     int rating) 
            { return document_id % 2 == 0; })) { 
            PrintDocument(document); 
        } 
    } catch (const invalid_argument& error) {
        std::cout << "Invalid argument: "s << error.what() << std::endl;
    } catch (const out_of_range& error) {
        std::cout << "Out of range: "s << error.what() << std::endl;
    }
 
    return 0; 
}
  
