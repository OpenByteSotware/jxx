// person.hpp
#pragma once
#include "jxx_object.h"
#include <utility>

class Person
    : public jxx::lang::Cloneable<Person>   // supplies clone()
    , public jxx::lang::Synchronized        // optional: monitor/lock
{
    const std::string name_;
    int age_{};

public:
    Person(const std::string name, int age)
        : name_(std::move(name)), age_(age) {}

    virtual ~Person() {};

    const std::string& name() const noexcept { return name_; }
    int age() const noexcept { return age_; }

    // Equality: same runtime type + same data
    bool equals(const jxx::lang::Object& other) const noexcept override {
        if (this->same(other)) return true;
        auto* o = dynamic_cast<const Person*>(&other);
        return o && age_ == o->age_ && name_ == o->name_;
    }

    // hashCode consistent with equals
    std::size_t hashCode() const noexcept override {
        std::size_t h1 = std::hash<std::string>{}(name_);
        std::size_t h2 = std::hash<int>{}(age_);
        // Simple hash combine
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }

    virtual std::shared_ptr<Object> clone() const override {
        return std::static_pointer_cast<Object>(NEW_O(Person, this->name_, this->age_));
    }

    std::string toString() const override {
        return "Person{name=" + name_ + ", age=" + std::to_string(age_) + "}";
    }
};
