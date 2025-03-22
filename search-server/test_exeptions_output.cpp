#include "test_exeptions_output.h"

using namespace std;
void TestExeptionForConstructorWithStopWords() {
    try {
        int number_of_unsuccessful_exceptions_thrown = 0;
        
        try {
            std::vector<string> stop_words = {"-\x12"s, "в"s, "на"s};
        
            SearchServer search_server(stop_words);    
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(invalid_argument& error) {
        }
        
        try {
            std::vector<string> stop_words = {"и"s, "в", "ск\x12рец"s};
        
            SearchServer search_server(stop_words);    
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(invalid_argument& error) {
        }
        
        try {
            std::vector<string> stop_words = {"и"s, "в ", "      \x12  "s};
        
            SearchServer search_server(stop_words);    
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(invalid_argument& error) {
        }
        
        throw number_of_unsuccessful_exceptions_thrown;
        
    } catch(int number_of_unsuccessful_exceptions_thrown) { 
        if(number_of_unsuccessful_exceptions_thrown == 0) {
            std::cerr << "Exception output test for constructor with container parameter completed successfully"s 
                      << std::endl;
        }   
    }
    
    try {
        int number_of_unsuccessful_exceptions_thrown = 0;
        
        try {
            SearchServer search_server("и в \x12"s);
            INCORRECT_EXEPTION_HANDLING;
        } catch(const invalid_argument& error) {
        }
        
        try {
            SearchServer search_server("и в           ск\x12рец"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(const invalid_argument& error) {
        }
        
        try {
            SearchServer search_server("-\x12 и в"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(const invalid_argument& error) {}
        
        throw number_of_unsuccessful_exceptions_thrown;
        
    } catch(int number_of_unsuccessful_exceptions_thrown){ 
        if(number_of_unsuccessful_exceptions_thrown == 0) {
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
        } catch(const invalid_argument& error) {
        }
           
        try { 
            search_server.AddDocument(-1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2});
            INCORRECT_EXEPTION_HANDLING;
                
            ++number_of_unsuccessful_exceptions_thrown;
         } catch(const invalid_argument& error) {
         }
        
        throw number_of_unsuccessful_exceptions_thrown;
            
    } catch(int number_of_unsuccessful_exceptions_thrown) {
        if(number_of_unsuccessful_exceptions_thrown == 0) {
            std::cerr << "Exception output test for all documents with negative index completed successfully"s 
                      << std::endl;
        }   
    }
}
void TestExeptionForDocumentsWithSpecialCharacters() {
    try {
        SearchServer search_server("и в на"s);
        int number_of_unsuccessful_exceptions_thrown = 0;
        
        try {
            search_server.AddDocument(15, "пушистый кот пуш\x00тый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7}); 
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(const invalid_argument& error) {
        }
        
        try {
            search_server.AddDocument(12, "\x16  пушистый        кот пуштый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7}); 
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(const invalid_argument& error) {
        }
        
        try {
            search_server.AddDocument(45, "пушистый кот пуштый хвост        \x15"s, DocumentStatus::ACTUAL, {7, 2, 7}); 
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(const invalid_argument& error) {
        }
        
        throw number_of_unsuccessful_exceptions_thrown;
        
    } catch(int number_of_unsuccessful_exceptions_thrown) {
        if(number_of_unsuccessful_exceptions_thrown == 0) {
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
        } catch(const invalid_argument& error) {
        }
        
        try {
            search_server.FindTopDocuments("кот выгл\x01дывает в окно"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(const invalid_argument& error) {
        }
        
        try {
            search_server.FindTopDocuments("\x12          кот выглядывает в окно"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(const invalid_argument& error) {
        }
        
        try {
            search_server.FindTopDocuments("           \x12           "s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(const invalid_argument& error) {
        }
        
        throw number_of_unsuccessful_exceptions_thrown;
        
    } catch(int number_of_unsuccessful_exceptions_thrown) {
        if(number_of_unsuccessful_exceptions_thrown == 0) {
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
        } catch(const invalid_argument& error) {
        }
        
        try {
            search_server.FindTopDocuments("кот выглядывает в --окно"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(const invalid_argument& error) {
        }
        
        try {
            search_server.FindTopDocuments("--кот выглядывает в окно"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(const invalid_argument& error) {
        }
        
        try {
            search_server.FindTopDocuments("кот выглядывает в -"s);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(const invalid_argument& error) {
        }
        
        //MatchDocuments
        try {
            search_server.MatchDocument("пушистый --кот выглядывает в -окно"s, 1);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(const invalid_argument& error) {
        }
        
        try {
            search_server.MatchDocument("кот выглядывает в --окно"s, 1) ;
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(const invalid_argument& error) {
        }
        
        try {
            search_server.MatchDocument("--кот выглядывает в окно"s, 1);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(const invalid_argument& error) {
        }
        
        try {
            search_server.MatchDocument("кот выглядывает в -"s, 1);
            INCORRECT_EXEPTION_HANDLING;
            
            ++number_of_unsuccessful_exceptions_thrown;
        } catch(const invalid_argument& error) {
        }
        
        //В данном случае осуществляется обратная проверка, где, в случае
        //наличия в середине слова дефиса, исключение срабатывать не должно
        try {
            search_server.MatchDocument("Кот-Леопольд выглядывает в окно"s, 1);
        } catch(const invalid_argument& error) {
            INCORRECT_EXEPTION_HANDLING;
            ++number_of_unsuccessful_exceptions_thrown;
        }
        
        try {
            search_server.MatchDocument("Кот----Леопольд выглядывает в окно"s, 1);
        } catch(const invalid_argument& error) {
            INCORRECT_EXEPTION_HANDLING;
            ++number_of_unsuccessful_exceptions_thrown;
        }
        
        try {
            search_server.MatchDocument("Кот-Леопольд выглядывает- в окно"s, 1);
        } catch(const invalid_argument& error) {
            INCORRECT_EXEPTION_HANDLING;
            ++number_of_unsuccessful_exceptions_thrown;
        }
        
        throw number_of_unsuccessful_exceptions_thrown;
        
    } catch(int number_of_unsuccessful_exceptions_thrown) {
        if(number_of_unsuccessful_exceptions_thrown == 0){
            std::cerr << "Exception output test for all queryes with minus words completed successfully"s 
                      << std::endl;
        }
    }
}

void TestExeptionForGetDocumentId() {
    try {
        SearchServer search_server("и в на"s);
        
        search_server.AddDocument(32, "пушистый кот пуштый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
        search_server.GetDocumentId(32);
        
        INCORRECT_EXEPTION_HANDLING;
    } catch(const out_of_range& e) {
       std::cerr << "Exception output test for function \"GetDocumentId()\" completed successfully"s
                 << std::endl;
   }
}