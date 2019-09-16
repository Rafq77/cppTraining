#include <unordered_set>
#include <string>
#include <utility>
#include <functional>
#include <iostream>

template <typename T>
size_t doHash(T const &t)
{
    return std::hash<T>()(t);
}

class Person {
public:
    Person(std::string const &name_, std::string const &role_)
      : name(name_)
      , role(role_)
    {}

    Person(std::string const &&name_, std::string const &&role_)
      : name(std::move(name_))
      , role(std::move(role_))
    {}

    size_t hash() const
    {
        return doHash(name) ^ (doHash(role) >> 1);
    }

    void print(std::ostream &o) const
    {
        o << "Name: " << name << "   Role: " << role <<'\n';
    }

    bool isEqual(Person const &rhs) const
    {
        return name == rhs.name && role == rhs.role;
    }

private:
    std::string name;
    std::string role;
};
namespace std
{
template <>
struct hash<Person>
{
    typedef Person argument_type;
    typedef size_t result_type;

    size_t operator()(const Person&p) const
    {
        return p.hash();
    }
};
} // namespace std
bool operator==(Person const &p1, Person const &p2)
{
    return p1.isEqual(p2);
}
std::ostream &operator<<(std::ostream &o, Person const &p)
{
    p.print(o);
    return o;
}


int main()
{
    std::unordered_set<Person> people;
    people.insert(Person("Elizabeth", "Queen"));
    people.emplace("Elizabeth", "Queen");
    people.emplace("Obama", "President");
    people.emplace("James Dean", "Movie star");

    for (auto const &p: people)
    {
        std::cout << p;
    }

    return 0;
}
