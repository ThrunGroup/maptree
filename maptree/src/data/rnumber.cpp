#include <cassert>

#include "data/rnumber.h"

#ifdef _MSC_VER
#  include <intrin.h>
#  define __builtin_popcountll __popcnt64
#endif

BLOCK RNumber::get() const {
    assert(head_ < values_.size());
    return values_[head_];
}

void RNumber::set(BLOCK value) {
    assert(head_ < values_.size());
    values_[head_] = value;
}

void RNumber::update(BLOCK value) {
    assert(head_ < values_.size());
    values_[++head_] = value;
}

void RNumber::intersect(BLOCK other) {
    assert(head_ < values_.size());
    values_[head_ + 1] = other & get();
    head_++;
}

void RNumber::reverse() {
    assert(head_ > 0);
    head_--;
}

void RNumber::reset() {
    head_ = 0;
}

int RNumber::countBits() const {
    return __builtin_popcountll(get());
}

int RNumber::countBitsAtIntersection(BLOCK other) const {
    return __builtin_popcountll(get() & other);
}

bool RNumber::isSubset(BLOCK other) const {
    return (get() & other) == get();
}

bool RNumber::empty() const {
    return get() == 0;
}

bool operator==(const RNumber& lhs, const RNumber& rhs) {
    return lhs.get() == rhs.get();
}