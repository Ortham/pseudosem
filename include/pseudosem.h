#ifndef PSEUDOSEM
#define PSEUDOSEM

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

namespace pseudosem {
    namespace detail {
        struct VersionParts {
            // Split a version string into release and pre-release parts.
            VersionParts(const std::string& ver) {
                if (ver.empty())
                    return;

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
                                   std::back_inserter(releaseNumbers),
                                   toul);

                    if (firstNonInt != temp.end())
                        preReleaseStrings.assign(firstNonInt, temp.end());
                }

                // Now split the pre-release portion of the version string.
                if (pos != std::string::npos) {
                    std::string preRelStr(version.substr(pos));
                    trim(preRelStr, ". :_-");
                    temp = split(preRelStr, ". :_-");

                    preReleaseStrings.insert(preReleaseStrings.end(),
                                             temp.begin(),
                                             temp.end());
                }
            }

            int compare(VersionParts& other) {
                // First compare release numbers.
                int result = compareReleaseNumbers(other);

                if (result != 0)
                    return result;

                // Release strings are the same. Check pre-release strings.
                return compareStrings(preReleaseStrings, other.preReleaseStrings);
            }

        private:
            std::vector<unsigned long> releaseNumbers;
            std::vector<std::string> preReleaseStrings;

            void normaliseReleaseNumbers(VersionParts& other) {
                if (releaseNumbers.size() < other.releaseNumbers.size())
                    releaseNumbers.resize(other.releaseNumbers.size(), 0);
                else if (other.releaseNumbers.size() < releaseNumbers.size())
                    other.releaseNumbers.resize(releaseNumbers.size(), 0);
            }

            int compareReleaseNumbers(VersionParts& other) {
                normaliseReleaseNumbers(other);

                for (size_t i = 0; i < releaseNumbers.size(); ++i) {
                    if (releaseNumbers[i] < other.releaseNumbers[i])
                        return -1;

                    if (other.releaseNumbers[i] < releaseNumbers[i])
                        return 1;
                }

                return 0;
            }

            static int compareStrings(const std::vector<std::string>& strings1,
                                      const std::vector<std::string>& strings2) {
                       // If one strings is empty while the other does not, the latter is less.
                if (strings1.empty() != strings2.empty()) {
                    if (!strings1.empty())
                        return -1;
                    else
                        return 1;
                }

                if (strings1.empty())
                    return 0;

                auto isDigits = [](const std::string& str) {
                    return std::all_of(str.begin(), str.end(), ::isdigit);
                };

                // Compare pre-release strings one by one.
                size_t i = 0;
                while (i < strings1.size() && i < strings2.size()) {
                    bool v1IsInt = isDigits(strings1[i]);
                    bool v2IsInt = isDigits(strings2[i]);

                    // Integers have lower precedence than non-integer strings.
                    if (v1IsInt != v2IsInt) {
                        if (v1IsInt)
                            return -1;
                        else
                            return 1;
                    }

                    if (v1IsInt) {
                        // Compare integer values.
                        unsigned long v1Int = std::stoul(strings1[i]);
                        unsigned long v2Int = std::stoul(strings2[i]);

                        if (v1Int < v2Int)
                            return -1;
                        if (v2Int < v1Int)
                            return 1;
                    }
                    else {
                        // Compare string values.
                        if (strings1[i] < strings2[i])
                            return -1;

                        if (strings2[i] < strings1[i])
                            return 1;
                    }

                    ++i;
                }

                // Have reached the end of one or both pre-release string vectors.
                // If only the end of one was reached, it is less.
                if (strings1.size() == strings2.size())
                    return 0;
                else if (i == strings1.size())
                    return -1;
                else
                    return 1;
            }

            // Trim start and end of a string, removing any of the given characters.
            static void trim(std::string& str, const std::string& toTrim) {
                size_t pos = str.find_first_not_of(toTrim);
                if (pos != 0)
                    str.erase(0, str.find_first_not_of(toTrim));

                str.erase(str.find_last_not_of(toTrim) + 1);
            }

            // Split a string on any of the given characters.
            static std::vector<std::string> split(const std::string& str,
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
        };
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

        detail::VersionParts v1(ver1);
        detail::VersionParts v2(ver2);

        return v1.compare(v2);
    }
}

#endif
