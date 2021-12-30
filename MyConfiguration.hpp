/**
 * @file MyConfiguration.hpp
 * @author Ghazal Khalili (khalili.ghazal.97@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <map>
#include <algorithm>

using namespace std;

/**
 * @brief Class of config;
 *  To read the configuration file.
 */
class config
{
public:
    /**
     * @brief Constructs a new config object.
     * @param _file_name Name of the configuration file to read.
     */
    config(const string &);

    /**
     * @brief Exception to be thrown if the file cannot be opened.
     */
    class file_notfound : public invalid_argument
    {
    public:
        file_notfound() : invalid_argument("Cannot open a file with the given name!\n"){};
    };

    /**
     * @brief Exception to be thrown if any line misses the key.
     */
    class key_notfound : public invalid_argument
    {
    public:
        key_notfound() : invalid_argument("Key is not found!\n"){};
    };

    /**
     * @brief Exception to be thrown if the line is not in the valid format.
     */
    class incompatible_format : public invalid_argument
    {
    public:
        incompatible_format() : invalid_argument("The acceptable format is: \"Key = Value; //any comment if needed\".\n"){};
    };

    /**
     * @brief Exception to be thrown if any line misses the value.
     */
    class value_notfound : public invalid_argument
    {
    public:
        value_notfound() : invalid_argument("Value is not found!\n"){};
    };

    /**
     * @brief Exception to be thrown if a number value is not valid.
     */
    class num_invalid : public invalid_argument
    {
    public:
        num_invalid() : invalid_argument("The requested number is not valid and cannot be converted!\n"){};
    };

    /**
     * @brief Gets the non-string value of a key;
     *  The value is inverted to a number.
     * @param key The key.
     * @param default_v The pre-defined value.
     * @return The requested value, if found; otherwise the default value is returned.
     */
    double get_value(const string &, const double &);

    /**
     * @brief Gets the string value of a key.
     * @param key The key.
     * @return The requested value.
     */
    string get_value_string(const string &);

    /**
     * @brief Prints all the keys, and values of the configuration file.
     */
    void print_content() const;

private:
    /**
     * @brief Checks whether it is a valid number.
     * @param value_st The string of the value.
     * @return True if it is valid.
     * @return False invalid.
     */
    bool is_valid_number(const string &);

    /**
     * @brief The name of the configuration file
     */
    string confile;
    /**
     * @brief A map of {key, value}
     */
    map<string, string> content;
};

// ==============
// Implementation
// ==============

config::config(const string &_file_name) : confile(_file_name)
{
    // Reading the data
    ifstream input(confile);
    if (!input.is_open())
    {
        throw typename config::file_notfound();
    }
    // Reading line by line
    string line;
    while (getline(input, line))
    {
        // Skipping the empty lines
        if (line.size() <= 2) // The "\n" at the end of each line is 2 characters in length
        {
            continue;
        }

        string key;
        string value;

        // Removing every comment after ";" (if any).
        if (line.find(';') != line.npos)
        {
            line.erase(line.find(';'));
        }
        else // Each line must end with a ";"
        {
            cout << "End with \";\"\n";
            throw typename config::incompatible_format();
        }

        // Ignoring white sapaces before and after the first word
        line.erase(remove_if(line.begin(), line.end(), [](const char &c)
                             { return c == ' '; }),
                   line.end());

        // Finding the equal sign "="
        if (line.find_first_of('=') != line.find_last_of('=')) // If more than one is found
        {
            cout << "More than one '=' found\n";
            throw typename config::incompatible_format();
        }
        else if (line.find('=') == line.npos) // If not found
        {
            cout << "use = sign;\n";
            throw typename config::incompatible_format();
        }
        else if (line.find('=') == 0) // If it is in the begining, it means we do not have a key
        {
            throw typename config::key_notfound();
        }
        else if (line.find('=') == line.size() - 1) // If it is in the end, it means that we do not have a value
        {
            throw typename config::value_notfound();
        }
        else
        {
            key = line.substr(0, line.find('='));
            value = line.substr(line.find('=') + 1, line.size());
        }
        // Adding to the map
        content[key] = value;
    }
    if (input.eof())
        cout << "\nConfiguration file is received.\n";
    else if (input.fail())
    {
        cout << "Encountered an error in input!\n";
        input.close();
        throw typename config::file_notfound();
    }
    input.close();
}

double config::get_value(const string &key, const double &default_v)
{
    // The value saved as a string in the map before
    string value_st;
    // Getting the value of the given key
    try
    {
        value_st = content.at(key);
    }
    catch (const exception &e)
    {
        cout << "The parameter " << key << " is not defined in the configuration file; \n+ the default value of "
             << default_v << " is used.\n";
        return default_v;
    }

    // Checking whether it is a valid number
    if (is_valid_number(value_st))
    {

        return stod(value_st);
    }
    else
    {
        throw typename config::num_invalid();
    }
}

inline string config::get_value_string(const string &key)
{
    string value_st; // String of the value saved before
    try
    {
        value_st = content.at(key);
    }
    catch (const exception &e)
    {
        cout << "Requesting " << key << ": ";
        throw typename config::key_notfound();
    }
    return value_st;
}

inline void config::print_content() const
{
    cout << "Key   | Value\n";
    for (const auto &i : content)
    {
        cout << i.first << " | " << i.second << '\n';
    }
}

bool config::is_valid_number(const string &value_st)
{
    // Breaking down the string to characters
    vector<char> c_in_str(value_st.c_str(), value_st.c_str() + value_st.size() + 1);
    uint64_t size = c_in_str.size() - 1;

    uint64_t dot = 0;
    for (uint64_t i = 0; i < size; i++)
    {
        if (c_in_str[i] == '.') // The number is allowed to have only one decimal point
        {
            dot++;
            if (dot > 1)
            {
                return false;
            }
            continue;
        }
        if (i == 0 && c_in_str[i] == '-') // In case of a negative number, skips the minus sign in the begining only
        {
            continue;
        }
        // The other characters should be only digits
        if (isdigit(c_in_str[i]) == 0)
        {
            return false;
        }
    }
    return true;
}

// =============================
// End of Config Implementation
// =============================
