#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <random>
#include <type_traits>
#include "jxx.util.NoSuchElementException.h"
#include "jxx.util.ArrayList.h"
#include "jxx.util.Random.h"

namespace jxx { namespace util {

class Collections {
public:
    
    template <typename T>
    static void reverse(std::vector<T>& a) {
        std::reverse(a.begin(), a.end());
    }

    template <typename T, typename Cmp = std::less<T>>
    static void sort(std::vector<T>& a, Cmp cmp = Cmp{}) {
        std::sort(a.begin(), a.end(), cmp);
    }

    template <typename T>
    static void shuffle(std::vector<T>& a, jxx::util::Random* rng = nullptr) {
        auto n = (int)a.size();
        if (n <= 1) return;
        if (rng) {
            for (int i=n-1; i>0; --i) {
                int j = rng->nextInt(i+1);
                std::swap(a[(size_t)i], a[(size_t)j]);
            }
        } else {
            std::random_device rd; std::mt19937 gen(rd());
            for (int i=n-1; i>0; --i) {
                std::uniform_int_distribution<int> d(0, i);
                int j = d(gen);
                std::swap(a[(size_t)i], a[(size_t)j]);
            }
        }
    }

    template <typename T>
    static int binarySearch(const std::vector<T>& a, const T& key) {
        auto it = std::lower_bound(a.begin(), a.end(), key);
        size_t pos = (size_t)std::distance(a.begin(), it);
        if (it != a.end() && !(*it < key) && !(key < *it)) return (int)pos; // equal
        return -(int(pos) + 1);
    }

    template <typename T, typename Cmp>
    static int binarySearch(const std::vector<T>& a, const T& key, Cmp cmp) {
        auto it = std::lower_bound(a.begin(), a.end(), key, cmp);
        size_t pos = (size_t)std::distance(a.begin(), it);
        if (it != a.end() && !cmp(*it, key) && !cmp(key, *it)) return (int)pos; // equal
        return -(int(pos) + 1);
    }

    template <typename T>
    static T min(const std::vector<T>& a) {
        if (a.empty()) JXX_THROW(NoSuchElementException, "Collections::min on empty");
        return *std::min_element(a.begin(), a.end());
    }

    template <typename T, typename Cmp>
    static T min(const std::vector<T>& a, Cmp cmp) {
        if (a.empty()) JXX_THROW(NoSuchElementException, "Collections::min on empty");
        return *std::min_element(a.begin(), a.end(), cmp);
    }

    template <typename T>
    static T max(const std::vector<T>& a) {
        if (a.empty()) JXX_THROW(NoSuchElementException, "Collections::max on empty");
        return *std::max_element(a.begin(), a.end());
    }

    template <typename T, typename Cmp>
    static T max(const std::vector<T>& a, Cmp cmp) {
        if (a.empty()) JXX_THROW(NoSuchElementException, "Collections::max on empty");
        return *std::max_element(a.begin(), a.end(), cmp);
    }

    template <typename T>
    static int frequency(const std::vector<T>& a, const T& o) {
        return (int)std::count(a.begin(), a.end(), o);
    }

    template <typename T>
    static bool disjoint(const std::vector<T>& a, const std::vector<T>& b) {
        for (const auto& x : a) for (const auto& y : b) if (x==y) return false; return true;
    }

    template <typename T>
    static void fill(std::vector<T>& a, const T& val) { std::fill(a.begin(), a.end(), val); }

    template <typename T>
    static void copy(std::vector<T>& dest, const std::vector<T>& src) {
        if (dest.size() < src.size()) throw std::out_of_range("Collections::copy dest too small");
        std::copy(src.begin(), src.end(), dest.begin());
    }

    template <typename T>
    static void swap(std::vector<T>& a, int i, int j) {
        if (i<0 || j<0 || (size_t)i>=a.size() || (size_t)j>=a.size()) throw std::out_of_range("Collections::swap index");
        std::swap(a[(size_t)i], a[(size_t)j]);
    }

    template <typename T>
    static void rotate(std::vector<T>& a, int distance) {
        const int n = (int)a.size(); if (n==0) return;
        int k = distance % n; if (k<0) k += n;
        std::rotate(a.rbegin(), a.rbegin()+k, a.rend());
    }

    template <typename T>
    static std::vector<T> nCopies(int n, const T& o) {
        if (n < 0) throw std::out_of_range("Collections::nCopies negative n");
        return std::vector<T>((size_t)n, o);
    }

    template <typename T>
    static bool replaceAll(std::vector<T>& a, const T& oldVal, const T& newVal) {
        bool modified = false;
        for (auto& x : a) if (x == oldVal) { x = newVal; modified = true; }
        return modified;
    }

    template <typename T>
    static int indexOfSubList(const std::vector<T>& source, const std::vector<T>& target) {
        if (target.empty()) return 0;
        if (source.size() < target.size()) return -1;
        const size_t end = source.size() - target.size();
        for (size_t i=0; i<=end; ++i) {
            bool match = true;
            for (size_t j=0; j<target.size(); ++j) {
                if (!(source[i+j] == target[j])) { match = false; break; }
            }
            if (match) return (int)i;
        }
        return -1;
    }

    template <typename T>
    static int lastIndexOfSubList(const std::vector<T>& source, const std::vector<T>& target) {
        if (target.empty()) return (int)source.size();
        if (source.size() < target.size()) return -1;
        for (size_t i = source.size() - target.size() + 1; i-- > 0; ) {
            bool match = true;
            for (size_t j=0; j<target.size(); ++j) {
                if (!(source[i+j] == target[j])) { match = false; break; }
            }
            if (match) return (int)i;
            if (i==0) break;
        }
        return -1;
    }

    // NEW: reverseOrder comparators
    template <typename T>
    static auto reverseOrder() {
        return [](const T& a, const T& b){ return b < a; };
    }

    template <typename Cmp>
    static auto reverseOrder(Cmp cmp) {
        return [cmp](const auto& a, const auto& b){ return cmp(b, a); };
    }

    // ----- ArrayList<T> variants -----
    template <typename T>
    static void reverse(ArrayList<T>& list) {
        int i=0, j=list.size()-1; while (i<j) { T tmp=list.get(i); list.set(i, list.get(j)); list.set(j, tmp); ++i; --j; }
    }

    template <typename T, typename Cmp = std::less<T>>
    static void sort(ArrayList<T>& list, Cmp cmp = Cmp{}) {
        std::vector<T> tmp; tmp.reserve((size_t)list.size());
        for (int i=0;i<list.size();++i) tmp.push_back(list.get(i));
        std::sort(tmp.begin(), tmp.end(), cmp);
        for (int i=0;i<list.size();++i) list.set(i, tmp[(size_t)i]);
    }

    template <typename T>
    static void shuffle(ArrayList<T>& list, jxx::util::Random* rng = nullptr) {
        int n = list.size(); if (n<=1) return;
        if (rng) {
            for (int i=n-1;i>0;--i) { int j = rng->nextInt(i+1); T tmp=list.get(i); list.set(i, list.get(j)); list.set(j, tmp); }
        } else {
            std::random_device rd; std::mt19937 gen(rd());
            for (int i=n-1;i>0;--i) { std::uniform_int_distribution<int> d(0,i); int j=d(gen); T tmp=list.get(i); list.set(i, list.get(j)); list.set(j, tmp); }
        }
    }

    template <typename T>
    static int binarySearch(const ArrayList<T>& list, const T& key) {
        int low=0, high=list.size()-1;
        while (low<=high) {
            int mid=(low+high)>>1; T midVal = list.get(mid);
            if (midVal < key) low = mid+1; else if (key < midVal) high = mid-1; else return mid;
        }
        return -(low + 1);
    }

    template <typename T, typename Cmp>
    static int binarySearch(const ArrayList<T>& list, const T& key, Cmp cmp) {
        int low=0, high=list.size()-1;
        while (low<=high) {
            int mid=(low+high)>>1; T midVal = list.get(mid);
            if (cmp(midVal, key)) low = mid+1; else if (cmp(key, midVal)) high = mid-1; else return mid;
        }
        return -(low + 1);
    }

    template <typename T>
    static T min(const ArrayList<T>& list) {
        if (list.size()==0) throw NoSuchElementException("Collections::min on empty");
        T best = list.get(0); for (int i=1;i<list.size();++i) if (list.get(i) < best) best = list.get(i); return best;
    }

    template <typename T, typename Cmp>
    static T min(const ArrayList<T>& list, Cmp cmp) {
        if (list.size()==0) throw NoSuchElementException("Collections::min on empty");
        T best = list.get(0); for (int i=1;i<list.size();++i) if (cmp(list.get(i), best)) best = list.get(i); return best;
    }

    template <typename T>
    static T max(const ArrayList<T>& list) {
        if (list.size()==0) throw NoSuchElementException("Collections::max on empty");
        T best = list.get(0); for (int i=1;i<list.size();++i) if (best < list.get(i)) best = list.get(i); return best;
    }

    template <typename T, typename Cmp>
    static T max(const ArrayList<T>& list, Cmp cmp) {
        if (list.size()==0) throw NoSuchElementException("Collections::max on empty");
        T best = list.get(0); for (int i=1;i<list.size();++i) if (cmp(best, list.get(i))) best = list.get(i); return best;
    }

    template <typename T>
    static int frequency(const ArrayList<T>& list, const T& o) {
        int c=0; for (int i=0;i<list.size();++i) if (list.get(i)==o) ++c; return c;
    }

    template <typename T>
    static bool disjoint(const ArrayList<T>& a, const ArrayList<T>& b) {
        for (int i=0;i<a.size();++i) for (int j=0;j<b.size();++j) if (a.get(i)==b.get(j)) return false; return true;
    }

    template <typename T>
    static void fill(ArrayList<T>& list, const T& val) { for (int i=0;i<list.size();++i) list.set(i, val); }

    template <typename T>
    static void copy(ArrayList<T>& dest, const ArrayList<T>& src) {
        if (dest.size() < src.size()) throw std::out_of_range("Collections::copy dest too small");
        for (int i=0;i<src.size();++i) dest.set(i, src.get(i));
    }

    template <typename T>
    static void swap(ArrayList<T>& list, int i, int j) {
        if (i<0 || j<0 || i>=list.size() || j>=list.size()) throw std::out_of_range("Collections::swap index");
        T tmp=list.get(i); list.set(i, list.get(j)); list.set(j, tmp);
    }

    template <typename T>
    static void rotate(ArrayList<T>& list, int distance) {
        int n=list.size(); if (n==0) return; int k = distance % n; if (k<0) k += n;
        auto rev=[&](int l,int r){ while (l<r){ T tmp=list.get(l); list.set(l, list.get(r)); list.set(r, tmp); ++l; --r; } };
        rev(0,n-1); rev(0,k-1); rev(k,n-1);
    }

    // indexOfSubList / lastIndexOfSubList for ArrayList
    template <typename T>
    static int indexOfSubList(const ArrayList<T>& source, const ArrayList<T>& target) {
        if (target.size()==0) return 0;
        if (source.size() < target.size()) return -1;
        int end = source.size() - target.size();
        for (int i=0; i<=end; ++i) {
            bool match = true;
            for (int j=0; j<target.size(); ++j) {
                if (!(source.get(i+j) == target.get(j))) { match = false; break; }
            }
            if (match) return i;
        }
        return -1;
    }

    template <typename T>
    static int lastIndexOfSubList(const ArrayList<T>& source, const ArrayList<T>& target) {
        if (target.size()==0) return source.size();
        if (source.size() < target.size()) return -1;
        for (int i = source.size() - target.size(); i >= 0; --i) {
            bool match = true;
            for (int j=0; j<target.size(); ++j) {
                if (!(source.get(i+j) == target.get(j))) { match = false; break; }
            }
            if (match) return i;
        }
        return -1;
    }

    // reverseOrder comparator helpers for ArrayList sorts
    template <typename T>
    static auto reverseOrderList() { return [](const T& a, const T& b){ return b < a; }; }
    template <typename Cmp>
    static auto reverseOrderList(Cmp cmp) { return [cmp](const auto& a, const auto& b){ return cmp(b, a); }; }

    // Convenience creators
    template <typename T>
    static std::vector<T> emptyList() { return {}; }
    template <typename T>
    static std::vector<T> singletonList(const T& v) { return {v}; }
};

}} 
