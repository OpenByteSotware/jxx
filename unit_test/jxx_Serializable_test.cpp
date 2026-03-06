#include <gtest/gtest.h>
#include "jxx.h"

using namespace jxx::lang;

class Person: public jxx::lang::Cloneable<Person>, public jxx::io::Serializable 
{
    // --- Fields you want to serialize ---
    std::string name_;
    int age_{};

public:
    Person() = default; // required for factory construction during read
    Person(std::string name, int age) : name_(std::move(name)), age_(age) {}

    const std::string& name() const noexcept { return name_; }
    int age() const noexcept { return age_; }

    // Object API
    bool equals(const jxx::lang::Object& other) const noexcept override {
        if (this->same(other)) return true;
        auto* o = dynamic_cast<const Person*>(&other);
        return o && age_ == o->age_ && name_ == o->name_;
    }

    // Java-like custom serialization hooks
    void writeObject(class jxx::io::ObjectOutputStream& out) const override {
    
    }
    
    void readObject(class jxx::io::ObjectInputStream& in, std::uint64_t storedUid) override {
    
    }

    virtual std::shared_ptr<Object> clone() const override {
        return JXX_NEW<Person>(this->name_, this->age_);
    }

    std::size_t hashCode() const noexcept override {
        std::size_t h1 = std::hash<std::string>{}(name_);
        std::size_t h2 = std::hash<int>{}(age_);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }

    std::string toString() const override {
        return "Person{name=" + name_ + ", age=" + std::to_string(age_) + "}";
    }
    /*
    // Serializable API
    // Name & UID: pick your package-style name and a stable UID (like Java)
    JXX_SERIALIZABLE(Person, "com.example.Person", 0xA4C9B5D2CC771122ULL)

        // Write fields
        void writeObject(jxx::io::BinaryWriter& out) const override {
        out.write(name_);
        out.write(age_);
    }

    // Read fields; 'storedUid' is the UID read from the stream
    void readObject(jxx::BinaryReader& in, std::uint64_t storedUid) override {
        (void)storedUid; // You can branch on version to migrate
        name_ = in.read<std::string>();
        age_ = in.read<int>();
    }
    */
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
    std::string name = "Sue";
    auto ixx = JXX_NEW<Person>(name, age);
    EXPECT_EQ(age, ixx->age());
}
