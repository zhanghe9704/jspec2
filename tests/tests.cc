//#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <algorithm>
#include <complex>
#include <fstream>
#include <sstream>
#include <string>

using std::complex;
using std::string;

TEST_CASE("INITIALIZE DA ENVIRONMENT") {
    int da_dim = 3;
    REQUIRE( da_dim  != 0);
}
//
//TEST_CASE("DA FUNCTIONS") {
//    DAVector x = 1 + da[0] + 2*da[1] + 5*da[2];
//    DAVector y = exp(x);
//    double eps = 1e-14;
//
//    SECTION("EXP") {
//        REQUIRE(compare_da_with_file("exp_da.txt", y, eps));
//    }
//
//	DAVector z;
//    da_substitute(y, 0, 1, z);
//
//    SECTION("SUBSTITUTE A NUMBER") {
//        REQUIRE(compare_da_with_file("substitute_number.txt", z, eps));
//    }
//
//    da_substitute(y, 0, x, z);
//
//    SECTION("SUBSTITUTE A DA VECTOR") {
//        REQUIRE(compare_da_with_file("substitute_da_vector.txt", z, eps));
//    }
//
//	std::vector<DAVector> lv(2);
//    lv.at(0) = sin(x);
//    lv.at(1) = cos(x);
//    std::vector<unsigned int> idx{0,1};
//    da_substitute(y, idx, lv, z);
//    SECTION("SUBSTITUTE MULTIPLE DA VECTORS") {
//        REQUIRE(compare_da_with_file("substitute_multiple_da_vectors.txt", z, eps));
//    }
//
//	std::vector<DAVector> lx(3);
//    std::vector<DAVector> ly(3);
//    lx.at(0) = x;
//    lx.at(1) = y;
//    lx.at(2) = sinh(x);
//
//    da_substitute(lx, idx, lv, ly);
//
//    SECTION("BUNCH SUBSTITUTION") {
//        REQUIRE(compare_da_with_file("bunch_substitution_0.txt", ly.at(0), eps));
//        REQUIRE(compare_da_with_file("bunch_substitution_1.txt", ly.at(1), eps));
//        REQUIRE(compare_da_with_file("bunch_substitution_2.txt", ly.at(2), eps));
//    }
//
//	std::vector<DAVector> lu(3);
//    lu.at(0) = sin(x);
//    lu.at(1) = cos(x);
//    lu.at(2) = tan(x);
//    da_composition(lx, lu, ly);
//
//    SECTION("DA COMPOSITION") {
//        REQUIRE(compare_da_with_file("da_composition_0.txt", ly.at(0), eps));
//        REQUIRE(compare_da_with_file("da_composition_1.txt", ly.at(1), eps));
//        REQUIRE(compare_da_with_file("da_composition_2.txt", ly.at(2), eps));
//    }
//
//}
