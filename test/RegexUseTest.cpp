#include <gtest/gtest.h>
#include <regex>
#include <string>
#include <vector>
#include <algorithm>

struct RegexUseTest : ::testing::Test {
using strings = std::vector<std::string>;

const std::regex sep_pattern{"[[:s:]]*[,;.:][[:s:]]*"};
};

TEST_F(RegexUseTest, BasicSearchTest) {

  const std::string prefix{"tag starts now: "};
  const std::string suffix{": that ended the tags"};
  const std::string html_tag{"<h1>the value<h1>"};
  const std::string source{prefix + html_tag + suffix};

  const std::regex pattern{"<(.*)>(.*)<(\\1)>"};
  std::smatch match;

  EXPECT_TRUE(std::regex_search(source, match, pattern));
  EXPECT_EQ(4, match.size()); // 0the element is all that matched the pattern
  EXPECT_EQ(html_tag, match.str());
  EXPECT_EQ(prefix, match.prefix());
  EXPECT_EQ(suffix, match.suffix());

  EXPECT_EQ("h1", match[1].str());
  EXPECT_EQ("the value", match[2].str());
  EXPECT_EQ("h1", match[3].str());
}

TEST_F(RegexUseTest, BasicMatchTest) {
  const std::string html_tag {"<h1>the value<h1>"};
  const std::regex pattern{"<(.*)>(.*)<(\\1)>"};
  std::smatch match;

  EXPECT_TRUE(std::regex_match(html_tag, match, pattern));
  EXPECT_EQ(4, match.size()); // 0the element is all that matched the pattern
  EXPECT_EQ(html_tag, match.str());
  EXPECT_EQ(0, match.prefix().length());
  EXPECT_EQ(0, match.suffix().length());

  // match (actually std::match_results) inherits from std::vector<std::sub_match>
  // but inheritance level is private, hence match::at() is not accessible here
  EXPECT_EQ("h1", match[1].str());
  EXPECT_EQ("the value", match[2].str());
  EXPECT_EQ("h1", match[3].str());
}

TEST_F(RegexUseTest, BasicTokenTest) {
  const std::string source{"entity1, entity2; entity3: entity4. entity5, entity6"};

  std::sregex_token_iterator tokenIterator{source.begin(), source.end(), sep_pattern, {-1}};
  std::sregex_token_iterator tokenIteratorEnd{};

  // std::sregex_token_iterator::value_type is sub_match
  strings entities{tokenIterator, tokenIteratorEnd};
  const strings expected{"entity1", "entity2", "entity3", "entity4", "entity5", "entity6"};
  EXPECT_EQ(expected, entities);
}

TEST_F(RegexUseTest, TokenTestForSepAtEnd) {
  const std::string source_c{"entity1,    entity2; entity3: entity4. entity5, entity6 ,  "};
  std::sregex_token_iterator tokenIterator{source_c.begin(), source_c.end(), sep_pattern, {-1}};
  strings entities{tokenIterator, std::sregex_token_iterator{}};
  const strings expected{"entity1", "entity2", "entity3", "entity4", "entity5", "entity6"};
  EXPECT_EQ(expected, entities);
}

TEST_F(RegexUseTest, TokenTestForSepAtBeginning) {
  /* the separator at the end of source is handled by token_iterator as expected
   * but separator at the beginning results into an additional empty string at the beginning.
   * this test shows an work around for this unexpected behavior*/

  const std::string source_c{" ,  entity1,    entity2; entity3: entity4. entity5, entity6"};
  std::sregex_token_iterator tokenIterator{source_c.begin(), source_c.end(), sep_pattern, {-1}};
  strings entities{tokenIterator, std::sregex_token_iterator{}};
  const strings expected{"entity1", "entity2", "entity3", "entity4", "entity5", "entity6"};
  EXPECT_NE(expected, entities); // As there is an empty string at the beginning

  // this is what is in the entities
  auto expected_copy = expected;
  expected_copy.insert(expected_copy.begin(), "");
  EXPECT_EQ(expected_copy, entities);

  // the fix is to ignore the empty element
  strings fixed_entities{};
  // std::sregex_token_iterator::value_type is sub_match
  // lambda is using an implicit conversion operator of sub_match to get std::string
  // in the parameter
  std::copy_if(tokenIterator, std::sregex_token_iterator{}, std::back_inserter(fixed_entities),
    [](const std::string& s){ return !s.empty();});
  EXPECT_EQ(expected, fixed_entities);

  fixed_entities.clear();
  // here is a version of lambda without using any implicit conversion operator
  // auto resolves to std::sub_match
  std::copy_if(tokenIterator, std::sregex_token_iterator{}, std::back_inserter(fixed_entities),
    [](const auto& sub_match) {return sub_match.length() != 0; });
  EXPECT_EQ(expected, fixed_entities);
}

TEST_F(RegexUseTest, SimpleReplaceTest) {
  const std::string html_tag{"<h1>the value<h1>"};
  std::regex pattern{"<(.*)>(.*)<(\\1)>"};
  auto modified = std::regex_replace(html_tag, pattern, "<$1 value=\"$2\">");
  EXPECT_EQ("<h1 value=\"the value\">", modified);
}

TEST_F(RegexUseTest, ReplaceTest) {
  const std::string html_tag_h1{"<h1>the value 1<h1>"};
  const std::string html_tag_h2{"<h2>the value 2<h2>"};
  const std::string html_tag{html_tag_h1 + html_tag_h2};

  std::regex pattern{"<(.*)>(.*)<(\\1)>"};
  auto all_modified = std::regex_replace(html_tag, pattern, "<$1 value=\"$2\">");
  EXPECT_EQ("<h1 value=\"the value 1\"><h2 value=\"the value 2\">", all_modified);

  // formatting the first one only
  auto first_modified = std::regex_replace(html_tag, pattern, "<$1 value=\"$2\">",
    std::regex_constants::format_first_only);
  EXPECT_EQ("<h1 value=\"the value 1\"><h2>the value 2<h2>", first_modified);

  // getting only matched and modified part
  auto only_first_modified = std::regex_replace(html_tag, pattern, "<$1 value=\"$2\">",
    std::regex_constants::format_first_only|std::regex_constants::format_no_copy);
  EXPECT_EQ("<h1 value=\"the value 1\">", only_first_modified);
}

TEST_F(RegexUseTest, IgnoreCaseTest) {
  const std::string src = "Comments, comments and comments!";
  std::regex pattern{"(Comments)"};

  // std::sregex_iterator::value_type is match_results
  std::sregex_iterator itr{src.begin(), src.end(), pattern};
  strings found_strings{};

  // std::transform is needed to fill found_strings as value_type of std::sregex_iterator
  // is match_results. Had it been sub_match (like that of std::sregex_token_iterator),
  // passing the iterators in constructor would have worked,
  std::transform(itr, std::sregex_iterator{}, std::back_inserter(found_strings),
    [](const auto& matched_results){ return matched_results.str(); });
  EXPECT_EQ(1, found_strings.size());
  EXPECT_EQ("Comments", found_strings[0]);

  std::regex ignore_case_pattern{"(comments)", std::regex_constants::icase|std::regex_constants::ECMAScript};
  std::sregex_iterator ignore_case_itr{src.begin(), src.end(), ignore_case_pattern};
  strings ignore_case_found_strings{};
  std::transform(ignore_case_itr, std::sregex_iterator{}, std::back_inserter(ignore_case_found_strings),
    [](const auto& matched_results){ return matched_results.str();});
  EXPECT_EQ(3, ignore_case_found_strings.size());
  const strings expected{"Comments", "comments", "comments"};
  EXPECT_EQ(expected, ignore_case_found_strings);
}