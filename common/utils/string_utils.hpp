/***
    This file is part of snapcast
    Copyright (C) 2014-2021  Johannes Pohl

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <algorithm>
#include <iomanip>
#include <map>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#ifdef WINDOWS
#include <cctype>
#endif

namespace utils
{
namespace string
{

// trim from start
static inline std::string& ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
    return s;
}

// trim from end
static inline std::string& rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string& trim(std::string& s)
{
    return ltrim(rtrim(s));
}

// trim from start
static inline std::string ltrim_copy(const std::string& s)
{
    std::string str(s);
    return ltrim(str);
}

// trim from end
static inline std::string rtrim_copy(const std::string& s)
{
    std::string str(s);
    return rtrim(str);
}

// trim from both ends
static inline std::string trim_copy(const std::string& s)
{
    std::string str(s);
    return trim(str);
}

// decode %xx to char
static std::string uriDecode(const std::string& src)
{
    std::string ret;
    char ch;
    for (size_t i = 0; i < src.length(); i++)
    {
        if (int(src[i]) == 37)
        {
            unsigned int ii;
            sscanf(src.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i += 2;
        }
        else
        {
            ret += src[i];
        }
    }
    return (ret);
}


static void split_left(const std::string& s, char delim, std::string& left, std::string& right)
{
    auto pos = s.find(delim);
    if (pos != std::string::npos)
    {
        left = s.substr(0, pos);
        right = s.substr(pos + 1);
    }
    else
    {
        left = s;
        right = "";
    }
}


static std::string split_left(const std::string& s, char delim, std::string& right)
{
    std::string left;
    split_left(s, delim, left, right);
    return left;
}



static std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}


static std::vector<std::string> split(const std::string& s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


static std::map<std::string, std::string> split_pairs(const std::string& s, char pair_delim, char key_value_delim)
{
    std::map<std::string, std::string> result;
    auto keyValueList = split(s, pair_delim);
    for (auto& kv : keyValueList)
    {
        auto pos = kv.find(key_value_delim);
        if (pos != std::string::npos)
        {
            std::string key = trim_copy(kv.substr(0, pos));
            std::string value = trim_copy(kv.substr(pos + 1));
            result[key] = value;
        }
    }
    return result;
}

static std::string url_encode(const std::string& value)
{
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i)
    {
        std::string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char)c);
        escaped << std::nouppercase;
    }

    return escaped.str();
}


} // namespace string
} // namespace utils

#endif
