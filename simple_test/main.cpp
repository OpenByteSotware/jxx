#include <iostream>
#include <memory>
#include "person.h"
#include "jxx_integer.h"
#include "jxx_object.h"

int main() {
    auto p1 = NEW(Person,"Alice", 30);
    auto p2 = NEW(Person, "Alice", 30);
    auto p3 = NEW(Person, "Bob", 31);

    std::cout << p1->toString() << "\n";                 // Person{name=Alice, age=30}
    std::cout << (p1->equals(*p2) ? "eq" : "neq") << "\n";  // eq
    std::cout << (p1->same(*p2) ? "same" : "not same") << "\n"; // not same

    // Polymorphic set keyed by virtual equals/hashCode
    jxx::lang::PolySet<> people;
    people.insert(p1);
    people.insert(p2); // considered equal -> set may hold only one logical person
    people.insert(p3);

    std::cout << "set size: " << people.size() << "\n"; // likely 2

    // Clone
    auto copy = p1->clone();
    std::cout << "clone: " << copy->toString() << "\n";

    // java link synchronized
    SYNCHRONIZE(p1, [&] {
        std::cout << "Thread-safe read 1: " << p1->toString() << "\n";
        return 0;
        });

    // c++ version
    p1->synchronized([&] {
        std::cout << "Thread-safe read: " << p1->toString() << "\n";
        return 0;
        });

    // Boxed Integer
    jxx::lang::PolySet<> objs;
    objs.insert(std::make_shared<jxx::lang::Integer>(42));
    objs.insert(std::make_shared<jxx::lang::Integer>(42)); // equal -> de-duplicated

    std::cout << "objs size: " << objs.size() << "\n"; // 1

    return 0;
}
