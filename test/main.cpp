#include "pseudosem.h"

#include <gtest/gtest.h>

TEST(Basic, anEmptyStringShouldBeEqualToAVersionOfZero) {
    std::string version1;
    std::string version2("0");

    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));
}

TEST(Basic, anEmptyStringShouldBeLessThanANonZeroVersion) {
    std::string version1;
    std::string version2("1");

    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));
}

TEST(Basic, SingleDigits) {
    std::string version1, version2;
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));

    version1 = std::string("5");
    version2 = std::string("5");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));

    version1 = std::string("4");
    version2 = std::string("5");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));
}

TEST(SemVer, Patch) {
    std::string version1, version2;

    version1 = std::string("0.0.1");
    version2 = std::string("0.0.1");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.1");
    version2 = std::string("0.0.2");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.2");
    version2 = std::string("0.0.10");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));
}

TEST(SemVer, Minor) {
    std::string version1, version2;

    version1 = std::string("0.0.2");
    version2 = std::string("0.1.0");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.1.0");
    version2 = std::string("0.1.0");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.1.0");
    version2 = std::string("0.2.0");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.2.0");
    version2 = std::string("0.10.0");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));
}

TEST(SemVer, Major) {
    std::string version1, version2;

    version1 = std::string("0.2.0");
    version2 = std::string("1.0.0");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("1.0.0");
    version2 = std::string("1.0.0");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.1.0");
    version2 = std::string("0.2.0");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.2.0");
    version2 = std::string("0.10.0");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));
}

TEST(SemVer, PreRelease) {
    std::string version1, version2;

    version1 = std::string("0.0.1-1");
    version2 = std::string("0.0.1-1.alpha");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.1-1.alpha");
    version2 = std::string("0.0.1-2");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.1-2");
    version2 = std::string("0.0.1-alpha");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.1-alpha");
    version2 = std::string("0.0.1-alpha.1");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.1-alpha.1");
    version2 = std::string("0.0.1-alpha.beta");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.1-alpha.beta");
    version2 = std::string("0.0.1-beta");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.1-beta.2");
    version2 = std::string("0.0.1-beta.11");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.1-rc.1");
    version2 = std::string("0.0.1");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.0-alpha");
    version2 = std::string("0.0.1");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));
}

TEST(SemVer, Metadata) {
    // Metadata should be ignored.
    std::string version1, version2;

    version1 = std::string("0.0.1+alpha");
    version2 = std::string("0.0.1+beta");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.1+alpha");
    version2 = std::string("0.0.1+1");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.1+1");
    version2 = std::string("0.0.1+2");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));
}

TEST(Extended, LeadingZeroes) {
    std::string version1, version2;

    version1 = std::string("0.0.1-01");
    version2 = std::string("0.0.1-1");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.1-01");
    version2 = std::string("0.0.1-02");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.1-02");
    version2 = std::string("0.0.1-010");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.01");
    version2 = std::string("0.0.1");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.01");
    version2 = std::string("0.0.02");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.02");
    version2 = std::string("0.0.010");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.01.0");
    version2 = std::string("0.1.0");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.01.0");
    version2 = std::string("0.02.0");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.02.0");
    version2 = std::string("0.010.0");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("01.0.0");
    version2 = std::string("1.0.0");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));

    version1 = std::string("01.0.0");
    version2 = std::string("02.0.0");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("02.0.0");
    version2 = std::string("010.0.0");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));
}

TEST(Extended, ArbitraryPeriods) {
    std::string version1, version2;

    version1 = std::string("1.0.0.0.0.0");
    version2 = std::string("1.0.0.0.0.1");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("1.0.0.01.0.0");
    version2 = std::string("1.0.0.1.0.0");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));
}

TEST(Extended, Padding) {
    std::string version1, version2;

    version1 = std::string("1.0.0.0.0");
    version2 = std::string("1");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.1.0.0-beta");
    version2 = std::string("0.1-beta");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.1.0.0-alpha");
    version2 = std::string("0.1-beta");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.1-alpha");
    version2 = std::string("0.1.0.0-beta");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("0.0.0.1.0.0");
    version2 = std::string("0.0.0.1");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));

    version1 = std::string("1.0.0.0.0.0");
    version2 = std::string("1.0.0.1");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));
}

TEST(Extended, Separators) {
    std::string version1, version2;

    version1 = std::string("1.0.0-alpha.1.2.3");
    version2 = std::string("1.0.0 alpha:1-2_3");
    EXPECT_EQ(0, pseudosem::compare(version1, version2));
    EXPECT_EQ(0, pseudosem::compare(version2, version1));
}

TEST(Extended, lettersAfterReleaseVersionNumbersWithNoSeparatorShouldBeTreatedAsPartOfTheReleaseVersion) {
    std::string version1, version2;

    version1 = std::string("1.0.0a");
    version2 = std::string("1.0.0b");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("1.0.0");
    version2 = std::string("1.0.0a");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("1.0.2");
    version2 = std::string("1.0.4b");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("1.0.0alpha.2");
    version2 = std::string("1.0.0beta");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("1.0.0a");
    version2 = std::string("1.0.0a.5");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));
}

TEST(Extended, versionsShouldBeComparedCaseInsensitively) {
    std::string version1, version2;

    version1 = std::string("1.0.0-Alpha");
    version2 = std::string("1.0.0-beta");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));

    version1 = std::string("1.0.0A");
    version2 = std::string("1.0.0b");
    EXPECT_GT(0, pseudosem::compare(version1, version2));
    EXPECT_LT(0, pseudosem::compare(version2, version1));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
