#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

using KeyValue = pair<string, int>;

class WordCountMapReduce {
 public:
  map<string, int> Run(const string& input_path) {
    const vector<string> lines = ReadLines(input_path);
    const vector<KeyValue> mapped = Map(lines);
    const map<string, vector<int>> grouped = Shuffle(mapped);
    return Reduce(grouped);
  }

 private:
  vector<string> ReadLines(const string& input_path) {
    ifstream input_file(input_path);
    if (!input_file.is_open()) {
      throw runtime_error("Could not open input file: " + input_path);
    }

    vector<string> lines;
    string line;
    while (getline(input_file, line)) {
      lines.push_back(line);
    }
    return lines;
  }

  vector<KeyValue> Map(const vector<string>& lines) {
    vector<KeyValue> intermediate_pairs;

    for (const string& line : lines) {
      istringstream stream(line);
      string word;

      while (stream >> word) {
        word = Normalize(word);
        if (!word.empty()) {
          intermediate_pairs.push_back({word, 1});
        }
      }
    }

    return intermediate_pairs;
  }

  map<string, vector<int>> Shuffle(
      const vector<KeyValue>& intermediate_pairs) {
    map<string, vector<int>> grouped_values;

    for (const auto& [word, count] : intermediate_pairs) {
      grouped_values[word].push_back(count);
    }

    return grouped_values;
  }

  map<string, int> Reduce(
      const map<string, vector<int>>& grouped_values) {
    map<string, int> final_counts;

    for (const auto& [word, counts] : grouped_values) {
      int total = 0;
      for (int count : counts) {
        total += count;
      }
      final_counts[word] = total;
    }

    return final_counts;
  }

  string Normalize(const string& word) {
    string cleaned;

    for (unsigned char ch : word) {
      if (isalnum(ch)) {
        cleaned.push_back(static_cast<char>(tolower(ch)));
      }
    }

    return cleaned;
  }
};

void PrintResults(const map<string, int>& counts) {
  cout << "Word counts:\n";
  for (const auto& [word, count] : counts) {
    cout << word << " -> " << count << '\n';
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: ./mapreduce_demo <input_file>\n";
    return 1;
  }

  try {
    WordCountMapReduce map_reduce;
    const map<string, int> counts = map_reduce.Run(argv[1]);
    PrintResults(counts);
  } catch (const exception& error) {
    cerr << "Error: " << error.what() << '\n';
    return 1;
  }

  return 0;
}
