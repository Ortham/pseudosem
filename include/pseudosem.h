#ifndef PSEUDOSEM
#define PSEUDOSEM

#include <string>
#include <vector>
#include <algorithm>

#include <iostream>

namespace pseudosem {
    namespace detail {
        // Trim start and end of a string, removing any of the given characters.
        void trim(std::string& str, const std::string& toTrim) {
            size_t pos = str.find_first_not_of(toTrim);
            if (pos != 0)
                str.erase(0, str.find_first_not_of(toTrim));

            str.erase(str.find_last_not_of(toTrim) + 1);
        }

        // Split a string on any of the given characters.
        std::vector<std::string> split(const std::string& str,
                                       const std::string& splitOn) {
            std::vector<std::string> tokens;
            size_t tokenStart = 0;
            size_t tokenEnd = 0;

            do {
                tokenEnd = str.find_first_of(splitOn, tokenStart);

                if (tokenEnd == std::string::npos) {
                    tokens.push_back(str.substr(tokenStart));
                }
                else if (tokenEnd != tokenStart)
                    tokens.push_back(str.substr(tokenStart, tokenEnd - tokenStart));

                tokenStart = tokenEnd + 1;
            } while (tokenEnd != std::string::npos);

            return tokens;
        }

        // Split a version string into release and pre-release parts.
        void decompose(const std::string& ver,
                       std::vector<unsigned long>& release,
                       std::vector<std::string>& preRelease) {
            if (ver.empty())
                return;

            release.clear();
            preRelease.clear();

            // Find first '+' and trim to it.
            std::string version(ver.substr(0, ver.find('+')));

            // Split release string.
            size_t pos = version.find_first_of(" :_-");
            std::string relStr(version.substr(0, pos));
            trim(relStr, ".");

            // Split release portion of version into what should be digit strings.
            std::vector<std::string> temp(split(relStr, "."));

            if (!temp.empty()) {
                // Function to test if string does not contains only digits.
                auto notDigits = [](const std::string& str) {
                    return !std::all_of(str.begin(), str.end(), ::isdigit);
                };

                // Unary string to unsigned long converter.
                auto toul = [](const std::string& str) {
                    return std::stoul(str);
                };

                auto firstNonInt = std::find_if(temp.begin(), temp.end(), notDigits);

                std::transform(temp.begin(),
                               firstNonInt,
                               std::back_inserter(release),
                               toul);

                if (firstNonInt != temp.end())
                    preRelease.assign(firstNonInt, temp.end());
            }

            // Now split the pre-release portion of the version string.
            if (pos != std::string::npos) {
                std::string preRelStr(version.substr(pos));
                trim(preRelStr, ". :_-");
                temp = split(preRelStr, ". :_-");

                preRelease.insert(preRelease.end(), temp.begin(), temp.end());
            }
        }
    }

    int compare(const std::string& ver1, const std::string ver2) {
        /* Version strings have a wide variety of possible formats.
        The precedence rules set out by Semantic Versioning <http://semver.org>
        are sufficient for comparisons, with the following extensions:

        1. Strings must be lowercased before comparison.
        2. Spaces (" "), colons (":") and underscores ("_") should be treated as
        separator characters between prerelease version identifiers.
        3. Version integers should be allowed to contain leading zeroes.
        4. An arbitrary number of version integers should be supported.
        5. If two strings contain a different number of version integers,
        their version integers should be padded (preceding any pre-release
        version or metadata) to equal length before comparison.
        */

        std::vector<unsigned long> v1Release, v2Release;
        std::vector<std::string> v1PreRelease, v2PreRelease;

        detail::decompose(ver1, v1Release, v1PreRelease);
        detail::decompose(ver2, v2Release, v2PreRelease);

        // Compare lengths of release vectors, and pad shorter with zeroes.
        if (v1Release.size() < v2Release.size())
            v1Release.resize(v2Release.size(), 0);
        else if (v2Release.size() < v1Release.size())
            v2Release.resize(v1Release.size(), 0);

        // Now compare release numbers one by one.
        for (size_t i = 0; i < v1Release.size(); ++i) {
            if (v1Release[i] < v2Release[i])
                return -1;

            if (v2Release[i] < v1Release[i])
                return 1;
        }

        // Release numbers are the same. Check pre-release strings. If one has
        // pre-release strings while the other does not, the first is less.
        if (v1PreRelease.empty() != v2PreRelease.empty()) {
            if (!v1PreRelease.empty())
                return -1;
            else
                return 1;
        }

        if (v1PreRelease.empty())
            return 0;

        auto isDigits = [](const std::string& str) {
            return std::all_of(str.begin(), str.end(), ::isdigit);
        };

        // Compare pre-release strings one by one.
        size_t i = 0;
        while (i < v1PreRelease.size() && i < v2PreRelease.size()) {
            bool v1IsInt = isDigits(v1PreRelease[i]);
            bool v2IsInt = isDigits(v2PreRelease[i]);

            // Integers have lower precedence than non-integer strings.
            if (v1IsInt != v2IsInt) {
                if (v1IsInt)
                    return -1;
                else
                    return 1;
            }

            if (v1IsInt) {
                // Compare integer values.
                unsigned long v1Int = std::stoul(v1PreRelease[i]);
                unsigned long v2Int = std::stoul(v2PreRelease[i]);

                if (v1Int < v2Int)
                    return -1;
                if (v2Int < v1Int)
                    return 1;
            }
            else {
                // Compare string values.
                if (v1PreRelease[i] < v2PreRelease[i])
                    return -1;

                if (v2PreRelease[i] < v1PreRelease[i])
                    return 1;
            }

            ++i;
        }

        // Have reached the end of one or both pre-release string vectors.
        // If only the end of one was reached, it is less.
        if (v1PreRelease.size() == v2PreRelease.size())
            return 0;
        else if (i == v1PreRelease.size())
            return -1;
        else
            return 1;
    }
}

#endif
