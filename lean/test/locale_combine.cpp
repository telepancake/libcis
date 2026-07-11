// Regression: locale(other, one, category) must reset the selected category to
// `one`'s facets even when `one` only IMPLICITLY holds the classic facet (never
// materialized via use_facet). Under lazy classic-facet registration the copy
// path must resolve the classic facet through the same fallback use_facet uses,
// or the destination silently keeps `other`'s custom facet. (base <locale>,
// exercised through the lean overlay build; both profiles use base <locale>.)
#include <locale>
#include "lean_test.h"

struct CommaPunct : std::numpunct<char> {
  char do_decimal_point() const override { return ','; }
};
struct RevCollate : std::collate<char> {
  int do_compare(const char* lo1, const char* hi1,
                 const char* lo2, const char* hi2) const override {
    return -std::collate<char>::do_compare(lo1, hi1, lo2, hi2);
  }
};

void test_numeric_reset_to_classic() {
  std::locale custom(std::locale::classic(), new CommaPunct);
  CHECK(std::use_facet<std::numpunct<char>>(custom).decimal_point() == ',');
  // No prior use_facet<numpunct>(classic): the classic facet is only implicit.
  std::locale reset(custom, std::locale::classic(), std::locale::numeric);
  CHECK(std::use_facet<std::numpunct<char>>(reset).decimal_point() == '.');
}

void test_collate_reset_to_classic() {
  std::locale custom(std::locale::classic(), new RevCollate);
  const auto& cc = std::use_facet<std::collate<char>>(custom);
  const char* a = "abc"; const char* b = "abd";
  CHECK(cc.compare(a, a + 3, b, b + 3) > 0);  // reversed
  std::locale reset(custom, std::locale::classic(), std::locale::collate);
  const auto& rc = std::use_facet<std::collate<char>>(reset);
  CHECK(rc.compare(a, a + 3, b, b + 3) < 0);  // classic order restored
}

void test_custom_survives_when_not_reset() {
  std::locale custom(std::locale::classic(), new CommaPunct);
  // resetting a DIFFERENT category must leave numeric's custom facet intact
  std::locale keep(custom, std::locale::classic(), std::locale::collate);
  CHECK(std::use_facet<std::numpunct<char>>(keep).decimal_point() == ',');
}

int main() {
  test_numeric_reset_to_classic();
  test_collate_reset_to_classic();
  test_custom_survives_when_not_reset();
  return 0;
}
