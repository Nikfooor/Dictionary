#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <sstream>
#include <regex>
#include "Dict.h"
#include <stdexcept>

struct Token
{
    std::string value;
    bool is_word;
};

std::vector<Token> readTokensFromFile(const std::string &filename)
{
    std::vector<Token> tokens;
    std::ifstream file(filename);

    if (!file.is_open())
        throw std::runtime_error("Error opening file: " + filename);

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::regex word_regex(R"(\w+)");
    std::sregex_token_iterator it(content.begin(), content.end(), word_regex, {-1, 0});
    std::sregex_token_iterator end;

    for (; it != end; ++it)
        tokens.push_back({it->str(), std::regex_match(it->str(), word_regex)});

    return tokens;
}

void replaceWords(std::vector<Token> &tokens, const std::string &old_word, const std::string &new_word)
{
    for (auto &token : tokens)
        if (token.is_word && (token.value == old_word))
            token.value = new_word;
}

void writeTokensToFile(const std::vector<Token> &tokens, const std::string &filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
        throw std::runtime_error("Error opening file: " + filename);

    for (const auto &token : tokens)
        file << token.value;

    file.close();
}

void writeDictionaryToFile(const std::unordered_set<std::string> &dictionary, const std::string &filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
        throw std::runtime_error("Error opening file: " + filename);

    for (const auto &word : dictionary)
        file << word << "\n";

    file.close();
}

class InvalidInputException : public std::runtime_error
{
public:
    explicit InvalidInputException(const std::string &message)
        : std::runtime_error(message) {}
};

int safeInputInt(const std::string &prompt, int minValue, int maxValue)
{
    int value;
    while (true)
    {
        try
        {
            std::cout << prompt;
            std::cin >> value;
            if (std::cin.fail() || value < minValue || value > maxValue)
            {
                throw InvalidInputException("Incorrect input. Try again.");
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        catch (const InvalidInputException &exc)
        {
            std::cout << exc.what() << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

int main()
{
    Dict d("C:\\University\\C++\\dictionary\\google-10000-english.txt");

    std::vector<Token> tokens = readTokensFromFile("C:\\University\\C++\\dictionary\\text.txt");

    for (auto &token : tokens)
    {
        if (!token.is_word)
            continue;

        std::string word = token.value;
        if (d.isInDict(word))
            std::cout << word << " ";
        else
        {
            std::cout << std::endl
                      << "\"" << word << "\"" << +" cant find in dictionary." << std::endl;
            std::cout << "0. Ignore" << std::endl;
            std::cout << "1. Add to dictionary" << std::endl;
            std::cout << "2. Replace with a similar one from the dictionary" << std::endl;
            int choice = safeInputInt("Select what you want to do (0-2): ", 0, 2);
            std::optional<std::vector<std::string>> result;
            switch (choice)
            {
            case 0:
                break;
            case 1:
                d.add(word);
                break;
            case 2:
                result = d.getSameInDict(word);
                if (result.has_value())
                {
                    int idx = 0;
                    for (const auto &dict_word : result.value())
                    {
                        std::cout << idx++ << ")" << dict_word << std::endl;
                    }
                    int choice_word = safeInputInt("Select word (0-" + std::to_string(idx - 1) + "): ", 0, idx - 1);
                    token.value = result.value()[choice_word];
                }
                else
                    std::cout << "No similar words were found in the dictionary" << std::endl;
                break;
            default:
                break;
            }
        }
    }
    for (auto &token : tokens)
        std::cout << token.value;

    writeTokensToFile(tokens, "C:\\University\\C++\\dictionary\\edited_text.txt");
    writeDictionaryToFile(d.get(), "C:\\University\\C++\\dictionary\\new_dict.txt");
}
