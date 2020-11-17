#pragma once
#include <initializer_list>
#include <iostream>
#include <map>

namespace Groebner {
class Monomial {
public:
    using IndexType = std::int64_t;
    using DegreeType = std::uint64_t;

    Monomial() = default;

    Monomial(std::initializer_list<std::pair<IndexType, DegreeType>> list) {
        for (auto pair : list) {
            if (pair.second != 0) {
                auto result = degrees_.try_emplace(pair.first, pair.second);
                assert(result.second);
            }
        }
    }

    DegreeType degree_of_variable(IndexType index) const {
        auto it = degrees_.find(index);
        if (it != degrees_.end()) {
            return it->second;
        }
        return 0;
    }

    const std::map<IndexType, DegreeType>& get_degrees() const {
        return degrees_;
    }

    Monomial& operator*=(const Monomial& factor) {
        for (const auto& degree : factor.degrees_) {
            degrees_[degree.first] += degree.second;
        }
        return *this;
    }

    bool is_divisible_by(const Monomial& other) const {
        for (const auto& degree : other.degrees_) {
            if (degree_of_variable(degree.first) < degree.second) {
                return false;
            }
        }
        return true;        
    }

    Monomial& operator/=(const Monomial& divider) {
        assert(is_divisible_by(divider));
        for (const auto& degree : divider.degrees_) {
            degrees_[degree.first] -= degree.second;
            if (degrees_[degree.first] == 0) {
                degrees_.erase(degree.first);
            }
        }
        return *this;
    }

    static Monomial gcd(const Monomial& first, const Monomial& second) {
        Monomial result;
        for (const auto& degree : first.degrees_) {
            auto gcd_degree = std::min(degree.second, second.degree_of_variable(degree.first));
            if (gcd_degree > 0)
            result.degrees_.emplace(degree.first, gcd_degree);
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& out, const Monomial& current) {
        for (auto& degree : current.degrees_) {
            assert(degree.second);
            out << "x_" << degree.first;
            if (degree.second != 1) {
                out << "^" << degree.second;
            }
        }
        return out;
    }

private:
    std::map<IndexType, DegreeType> degrees_;
};

Monomial operator*(const Monomial& first, const Monomial& second) {
    Monomial result = first;
    result *= second;
    return result;
}

Monomial operator/(const Monomial& first, const Monomial& second) {
    Monomial result = first;
    result /= second;
    return result;
}
}
