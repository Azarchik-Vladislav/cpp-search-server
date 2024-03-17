#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <utility>


#include "assert.h"
#include "document.h"
#include "search_server.h"



void TestExcludeStopWordsFromAddedDocumentContent();
void TestExcludeDocumentsIfMinusWords();
void TestSortAndCalculateByRelevance();
void TestCalculateRatingFromAddDocuments();
void TestOutDocumentsWithPredicate();
void TestOutDicumentWithQueryStatus();
void TestReturnStatusFromMatchDocument();


