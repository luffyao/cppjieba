#include <iostream>
#include <ctime>
#include <fstream>
#include "cppjieba/MPSegment.hpp"
#include "cppjieba/HMMSegment.hpp"
#include "cppjieba/MixSegment.hpp"
#include "cppjieba/KeywordExtractor.hpp"
#include "limonp/Colors.hpp"
#include "test_paths.h"
#include "cppjieba/Jieba.hpp"

using namespace cppjieba;

void Cut(size_t times = 50) {
  MixSegment seg(DICT_DIR "/jieba.dict.utf8", DICT_DIR "/hmm_model.utf8");
  vector<string> res;
  string doc;
  ifstream ifs(TEST_DATA_DIR "/weicheng.utf8");
  assert(ifs);
  doc << ifs;
  long beginTime = clock();
  for (size_t i = 0; i < times; i ++) {
    printf("process [%3.0lf %%]\r", 100.0*(i+1)/times);
    fflush(stdout);
    res.clear();
    seg.Cut(doc, res);
  }
  printf("\n");
  long endTime = clock();
  ColorPrintln(GREEN, "Cut: [%.3lf seconds]time consumed.", double(endTime - beginTime)/CLOCKS_PER_SEC);
}

void Extract(size_t times = 400) {
  KeywordExtractor Extractor(DICT_DIR "/jieba.dict.utf8", 
                           DICT_DIR "/hmm_model.utf8", 
                           DICT_DIR "/idf.utf8", 
                           DICT_DIR "/stop_words.utf8");
  vector<string> words;
  string doc;
  ifstream ifs(TEST_DATA_DIR "/review.100");
  assert(ifs);
  doc << ifs;
  long beginTime = clock();
  for (size_t i = 0; i < times; i ++) {
    printf("process [%3.0lf %%]\r", 100.0*(i+1)/times);
    fflush(stdout);
    words.clear();
    Extractor.Extract(doc, words, 5);
    if (i == 0) {
      std::cout << "Extract keywords (top 5):";
      for (size_t j = 0; j < words.size(); j++) {
        std::cout << (j == 0 ? " " : " / ") << words[j];
      }
      std::cout << std::endl;
    }
  }
  printf("\n");
  long endTime = clock();
  ColorPrintln(GREEN, "Extract: [%.3lf seconds]time consumed.", double(endTime - beginTime)/CLOCKS_PER_SEC);
}



void CutForSearch(size_t times = 50) {
  Jieba jieba(DICT_DIR "/jieba.dict.utf8",
    DICT_DIR "/hmm_model.utf8",
    DICT_DIR "/user.dict.utf8",
    DICT_DIR "/idf.utf8",
    DICT_DIR "/stop_words.utf8");
  vector<string> testcases;
  {
    ifstream ifs(TEST_DATA_DIR "/cutforsearch.100");
    assert(ifs);
    string line;
    while (std::getline(ifs, line)) {
      if (!line.empty()) {
        testcases.push_back(line);
      }
    }
  }
  assert(!testcases.empty());

  vector<string> words;
  std::ofstream ofs(TEST_DATA_DIR "/cutforsearch.result.log");
  assert(ofs);
  long beginTime = clock();
  for (size_t i = 0; i < times; i ++) {
    printf("process [%3.0lf %%]\r", 100.0*(i+1)/times);
    fflush(stdout);
    for (size_t k = 0; k < testcases.size(); k++) {
      words.clear();
      jieba.CutForSearch(testcases[k], words, true);
      if (i == 0 && k < testcases.size()) {
        ofs << "CutForSearch case[" << k << "]: "
            << "original: " << testcases[k]
            << ", result: " << limonp::Join(words.begin(), words.end(), "/")
            << std::endl;
      }
    }
  }
  printf("\n");
  long endTime = clock();
  ColorPrintln(GREEN,
               "CutForSearch: [%.3lf seconds]time consumed. cases=%zu, times=%zu. log=cutforsearch.result.log",
               double(endTime - beginTime)/CLOCKS_PER_SEC,
               testcases.size(),
               times);
}

int main(int argc, char ** argv) {
  //Cut();
  //Extract();
  CutForSearch();
  return EXIT_SUCCESS;
}