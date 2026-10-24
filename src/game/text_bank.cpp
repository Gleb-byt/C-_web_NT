#include "text_bank.hpp"

using reference = const char &;

MyIterator& MyIterator::operator++() {
    target->advance();
    return *this;
}

bool MyIterator::operator!=(const MyIterator & rhs) const {
    return rhs.target != target;
}

reference MyIterator::operator*() const {
    return target->get_current();
}

