#include <gtest/gtest.h>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Cloneable.h"
#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.String.h"

using namespace jxx::lang;
using namespace jxx::io;

#pragma once

#include <cstdint>
#include <utility>

#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.Serializable.h"

#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

class Person final
    : public jxx::lang::Object
    , public virtual jxx::lang::Cloneable
    , public virtual jxx::io::Serializable {
private:
    /*
     * This cannot be const because readObject() must restore it.
     */
    jxx::Ptr<jxx::lang::String> name_;
    jxx::lang::jint age_ = 0;

public:
    /*
     * Required if the deserialization framework first creates an
     * empty object and then invokes readObject().
     */
    Person()
        : name_(nullptr)
        , age_(0) {}

    Person(
        jxx::Ptr<jxx::lang::String> name,
        jxx::lang::jint age)
        : name_(std::move(name))
        , age_(age) {}

    virtual ~Person() = default;

    jxx::Ptr<jxx::lang::String>
        name() const noexcept {
        return name_;
    }

    jxx::lang::jint
        age() const noexcept {
        return age_;
    }

    /*
     * Object.equals(Object)
     */
    jxx::lang::jbool equals(
        jxx::Ptr<jxx::lang::Object> other)
        const noexcept override {

        if (this->same(other)) {
            return static_cast<
                jxx::lang::jbool>(true);
        }

        auto person =
            jxx::CAST<Person>(other);

        if (person == nullptr) {
            return static_cast<
                jxx::lang::jbool>(false);
        }

        if (age_ != person->age_) {
            return static_cast<
                jxx::lang::jbool>(false);
        }

        if (name_ == nullptr ||
            person->name_ == nullptr) {

            return static_cast<
                jxx::lang::jbool>(
                    name_ ==
                    person->name_);
        }

        return name_->equals(
            jxx::CAST<jxx::lang::Object>(
                person->name_));
    }

    /*
     * Object.hashCode()
     *
     * Uses Java-style 32-bit wraparound.
     */
    jxx::lang::jint hashCode()
        const noexcept override {

        const std::uint32_t nameHash =
            name_ == nullptr
            ? 0U
            : static_cast<std::uint32_t>(
                name_->hashCode());

        std::uint32_t result = 1U;

        result =
            result * 31U +
            nameHash;

        result =
            result * 31U +
            static_cast<std::uint32_t>(
                age_);

        return static_cast<
            jxx::lang::jint>(
                static_cast<std::int32_t>(
                    result));
    }

    /*
     * Object.toString()
     */
    jxx::Ptr<jxx::lang::String>
        toString() const override {

        if (name_ == nullptr) {
            return jxx::NEW<
                jxx::lang::String>(
                    "Person{name=null, age=" +
                    std::to_string(age_) +
                    "}");
        }

        return jxx::NEW<
            jxx::lang::String>(
                "Person{name=" +
                name_->utf8() +
                ", age=" +
                std::to_string(age_) +
                "}");
    }

    /*
     * Serializable custom write hook.
     *
     * Change these stream calls if your ObjectOutputStream API uses
     * different names.
     */
    void writeObject(
        jxx::Ptr<
        jxx::io::ObjectOutputStream> out)
        override {

        if (out == nullptr) {
            throw jxx::lang::
                NullPointerException();
        }

        out->writeObject(
            jxx::CAST<
            jxx::lang::Object>(
                name_));

        out->writeInt(age_);
    }

    /*
     * Serializable custom read hook.
     */
    void readObject(
        jxx::Ptr<
        jxx::io::ObjectInputStream> in)
        override {

        if (in == nullptr) {
            throw jxx::lang::
                NullPointerException();
        }

        name_ =
            jxx::CAST<
            jxx::lang::String>(
                in->readObject());

        age_ =
            in->readInt();
    }

    /*
     * This is likely the missing pure virtual method that made
     * Person abstract.
     */
    void readObjectNoData() override {
        name_ = nullptr;
        age_ = 0;
    }

protected:
    /*
     * Java clone semantics normally make a shallow copy.
     *
     * String is immutable, so sharing the String reference is safe
     * and matches Java's default shallow-clone behavior.
     */
    jxx::Ptr<jxx::lang::Object>
        cloneImpl() const override {

        auto copy =
            jxx::NEW<Person>(
                name_,
                age_);

        return jxx::CAST<
            jxx::lang::Object>(
                copy);
    }
};

// Define static registrar
//JXX_SERIALIZABLE_REGISTER(Person)
// Demonstrate some basic assertions.
TEST(PersonTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(PersonTest, IntValueTest) {
    //const std::string input_filepath = "this/package/testdata/myinputfile.dat";
    //const std::string output_filepath = "this/package/testdata/myoutputfile.dat";
    int age = 10;
    auto name = jxx::NEW<String>("test");
    auto ixx = jxx::NEW<Person>(name, age);
    EXPECT_EQ(age, ixx->age());
}

TEST(PersonTest, Clone) {
    //const std::string input_filepath = "this/package/testdata/myinputfile.dat";
    //const std::string output_filepath = "this/package/testdata/myoutputfile.dat";
    int age = 10;
    auto name = jxx::NEW<String>("Sue");
    auto p1 = jxx::NEW<Person>(name, age);
    EXPECT_EQ(age, p1->age());

    auto p2 = p1->clone();

    EXPECT_NE(p1.get(), p2.get());
    auto p2_sub = std::dynamic_pointer_cast<Person>(p2);
    
}