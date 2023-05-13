#pragma once // NOLINT(llvm-header-guard)

#include "Framework.h"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Instruction.h>

/// @todo(CSCD70) Please instantiate for the backward pass, similar to the
///               forward one.
/// @sa @c ForwardAnalysis
namespace dfa {

/// @todo(CSCD70) Please modify the traversal ranges.

typedef llvm::iterator_range<llvm::const_succ_iterator>
    BackwardMeetBBConstRange_t;
typedef llvm::iterator_range<
    llvm::Function::BasicBlockListType::const_reverse_iterator>
    BackwardBBConstRange_t;
typedef llvm::iterator_range<
    llvm::BasicBlock::InstListType::const_reverse_iterator>
    BackwardInstConstRange_t;

template <typename TDomainElem, typename TValue, typename TMeetOp>
class BackwardAnalysis
    : public Framework<TDomainElem, TValue, TMeetOp, BackwardMeetBBConstRange_t,
                       BackwardBBConstRange_t, BackwardInstConstRange_t> {
protected:
  using Framework_t =
      Framework<TDomainElem, TValue, TMeetOp, BackwardMeetBBConstRange_t,
                BackwardBBConstRange_t, BackwardInstConstRange_t>;
  using typename Framework_t::AnalysisResult_t;
  using typename Framework_t::BBConstRange_t;
  using typename Framework_t::InstConstRange_t;
  using typename Framework_t::MeetBBConstRange_t;

  using Framework_t::BVs;
  using Framework_t::DomainIdMap;
  using Framework_t::DomainVector;
  using Framework_t::InstDomainValMap;

  using Framework_t::getBoundaryVal;
  using Framework_t::getName;
  using Framework_t::run;
  using Framework_t::stringifyDomainWithMask;

  void printInstDomainValMap(const llvm::Instruction &Inst) const final {
    using llvm::errs;
    using llvm::outs;
    const llvm::BasicBlock *const ParentBB = Inst.getParent();

    if (&Inst == &(ParentBB->back())) {
      errs() << "\n";
      LOG_ANALYSIS_INFO << "\t"
                        << stringifyDomainWithMask(getBoundaryVal(*ParentBB));
    } // if (&Inst == &(*ParentBB->begin()))
    outs() << Inst << "\n";
    LOG_ANALYSIS_INFO << "\t"
                      << stringifyDomainWithMask(InstDomainValMap.at(&Inst));
  }

  MeetBBConstRange_t
  getMeetBBConstRange(const llvm::BasicBlock &BB) const final {
    return llvm::successors(&BB);
  }
  InstConstRange_t getInstConstRange(const llvm::BasicBlock &BB) const final {
    return make_range(BB.getInstList().rbegin(), BB.getInstList().rend());
  }
  BBConstRange_t getBBConstRange(const llvm::Function &F) const final {
    return make_range(F.getBasicBlockList().rbegin(),
                      F.getBasicBlockList().rend());
  }
};

} // namespace dfa
