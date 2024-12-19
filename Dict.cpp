#include "Dict.h"

bool Dict::isInDict(const std::string &word)
{
    return dictionary.find(word) != dictionary.end();
}

std::optional<std::vector<std::string>> Dict::getSameInDict(const std::string &word)
{
    std::optional<std::vector<std::string>> result;
    result.emplace();
    std::vector<std::string> candidates = generateLevenshteinDistanceOne(word);
    for (const auto &candidate : candidates)
        if (dictionary.find(candidate) != dictionary.end())
            result->push_back(candidate);
    return !result->empty() ? result : std::nullopt;
}

Dict::Dict(const std::string &filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
        throw std::runtime_error("Error opening file: " + filename);

    std::string word;
    while (std::getline(file, word))
        dictionary.insert(word);

    file.close();
}

void Dict::add(const std::string &word)
{
    dictionary.insert(word);
}

std::vector<std::string> Dict::generateLevenshteinDistanceOne(const std::string &word)
{
    std::vector<std::string> result;

    // Вставка одного символа
    for (size_t i = 0; i <= word.size(); ++i)
    {
        for (char c = 'a'; c <= 'z'; ++c)
        {
            std::string newWord = word.substr(0, i) + c + word.substr(i);
            result.emplace_back(std::move(newWord));
        }
    }

    // Удаление одного символа
    for (size_t i = 0; i < word.size(); ++i)
    {
        std::string newWord = word.substr(0, i) + word.substr(i + 1);
        result.emplace_back(std::move(newWord));
    }

    // Замена одного символа
    for (size_t i = 0; i < word.size(); ++i)
    {
        for (char c = 'a'; c <= 'z'; ++c)
        {
            if (c != word[i])
            {
                std::string newWord = word.substr(0, i) + c + word.substr(i + 1);
                result.emplace_back(std::move(newWord));
            }
        }
    }

    return result;
}

std::unordered_set<std::string> &Dict::get()
{
    return dictionary;
}