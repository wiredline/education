//this file only for tests

#include <gtest/gtest.h>
#include <memory>
#include <mathfunc.h>
#include "../main.cpp"

//JsonParse
TEST(JsonParseTest, ValidJson){
    JsonParse parser;
    parser.Parse(R"({"operation": "+", "args": [3,5]})");
    
    EXPECT_EQ(parser.GetOperation(), "+");
    EXPECT_EQ(parser.GetArgs().size(), 2);
    EXPECT_EQ(parser.GetArgs().at(0),3);
    EXPECT_EQ(parser.GetArgs().at(1),5);
}

TEST(JsonParseTest, InvalidJsonFormat) {
    JsonParse parser;

    EXPECT_THROW(
        parser.Parse("{operation: +}"),
        std::exception
    );
}

TEST(JsonParseTest, MissingFields) {
    JsonParse parser;

    EXPECT_THROW(
        parser.Parse(R"({"operation": "+"})"),
        std::exception
    );
}

//JsonCheck

TEST(JsonCheckTest, ValidAddition) {
    JsonCheck checker;

    EXPECT_NO_THROW(
        checker.Validate("+", {3, 5})
    );
}

TEST(JsonCheckTest, InvalidOperation) {
    JsonCheck checker;

    EXPECT_THROW(
        checker.Validate("%", {3, 5}),
        std::invalid_argument
    );
}

TEST(JsonCheckTest, WrongArgsCount) {
    JsonCheck checker;

    EXPECT_THROW(
        checker.Validate("+", {1}),
        std::invalid_argument
    );
}

TEST(JsonCheckTest, DivisionByZero) {
    JsonCheck checker;

    EXPECT_THROW(
        checker.Validate("/", {10, 0}),
        std::runtime_error
    );
}

TEST(JsonCheckTest, FactorialNegative) {
    JsonCheck checker;

    EXPECT_THROW(
        checker.Validate("!", {-5}),
        std::invalid_argument
    );
}


//MathMetods

TEST(MathTest, Sum) {
    math::SumMath op;
    EXPECT_EQ(op.calculate({3, 5}), 8);
}

TEST(MathTest, Subtraction) {
    math::SubMath op;
    EXPECT_EQ(op.calculate({10, 4}), 6);
}

TEST(MathTest, Multiplication) {
    math::MulMath op;
    EXPECT_EQ(op.calculate({6, 7}), 42);
}

TEST(MathTest, Division) {
    math::DivMath op;
    EXPECT_EQ(op.calculate({20, 4}), 5);
}

TEST(MathTest, Power) {
    math::MaxMath op;
    EXPECT_EQ(op.calculate({2, 3}), 8);
}

TEST(MathTest, Factorial) {
    math::FactMath op;
    EXPECT_EQ(op.calculate({5}), 120);
}

//DataCalculate

TEST(DataCalculate, CreateSum) {
    auto op = DataCalculate::Create("+");
    ASSERT_NE(op, nullptr);
    EXPECT_EQ(op->calculate({2, 3}), 5);
}

TEST(DataCalculate, CreateDivision) {
    auto op = DataCalculate::Create("/");
    ASSERT_NE(op, nullptr);
    EXPECT_EQ(op->calculate({10, 2}), 5);
}

TEST(DataCalculate, UnknownOperation) {
    EXPECT_THROW(
        DataCalculate::Create("%"),
        std::invalid_argument
    );
}