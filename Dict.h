#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <sstream>
#include <vector>
#include <regex>
#include <optional>

class Dict
{
public:
    Dict(const std::string &filename);
    void add(const std::string &word);
    bool isInDict(const std::string &word);
    std::optional<std::vector<std::string>> getSameInDict(const std::string &word);
    std::unordered_set<std::string> &get();

private:
    std::vector<std::string> generateLevenshteinDistanceOne(const std::string &word);
    std::unordered_set<std::string> dictionary;
};
