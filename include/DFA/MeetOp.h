#pragma once // NOLINT(llvm-header-guard)

#include <vector>

namespace dfa {

template <typename TValue> struct MeetOpBase {
  using DomainVal_t = std::vector<TValue>;
  /// @brief Apply the meet operator using two operands.
  /// @param LHS
  /// @param RHS
  /// @return
  virtual DomainVal_t operator()(const DomainVal_t &LHS,
                                 const DomainVal_t &RHS) const = 0;
  /// @brief Return a domain value that represents the top element, used when
  ///        doing the initialization.
  /// @param DomainSize
  /// @return
  virtual DomainVal_t top(const std::size_t DomainSize) const = 0;
};

template <typename TValue> struct Intersect final : MeetOpBase<TValue> {
  using DomainVal_t = typename MeetOpBase<TValue>::DomainVal_t;

  DomainVal_t operator()(const DomainVal_t &LHS,
                         const DomainVal_t &RHS) const final {

    /// @todo(CSCD70) Please complete this method.
    DomainVal_t tmpDV = DomainVal_t(LHS.size());
    assert(LHS.size() == RHS.size());

    for (std::size_t i = 0; i < LHS.size(); i++) {
      if (LHS[i] && RHS[i]) {
        tmpDV[i] = TValue{true};
      }
    }

    return tmpDV;
  }

  DomainVal_t top(const std::size_t DomainSize) const final {

    /// @todo(CSCD70) Please complete this method.

    return DomainVal_t(DomainSize, TValue{true});
  }
};

/// @todo(CSCD70) Please add another subclass for the Union meet operator.

template <typename TValue> struct Union final : MeetOpBase<TValue> {
  using DomainVal_t = typename MeetOpBase<TValue>::DomainVal_t;

  DomainVal_t operator()(const DomainVal_t &LHS,
                         const DomainVal_t &RHS) const final {

    /// @todo(CSCD70) Please complete this method.
    DomainVal_t tmpDV = DomainVal_t(LHS.size());
    assert(LHS.size() == RHS.size());

    for (std::size_t i = 0; i < LHS.size(); i++) {
      if (LHS[i] || RHS[i]) {
        tmpDV[i] = TValue{true};
      }
    }

    return tmpDV;
  }

  DomainVal_t top(const std::size_t DomainSize) const final {

    /// @todo(CSCD70) Please complete this method.

    return DomainVal_t(DomainSize, TValue{false});
  }
};

} // namespace dfa
