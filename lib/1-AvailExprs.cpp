#include "DFA.h"
#include "DFA/Domain/Expression.h"
#include "DFA/Flow/Framework.h"
#include <algorithm>
#include <iterator>
#include <llvm/IR/InstrTypes.h>
#include <utility>

using namespace llvm;

AnalysisKey AvailExprs::Key;

void AvailExprs::initializeDomainFromInst(const llvm::Instruction &Inst) {
  if (isa<BinaryOperator>(Inst)) {
    dfa::Expression expr(*dyn_cast<BinaryOperator>(&Inst));
    if (DomainIdMap.count(expr) == 0) {
      DomainVector.push_back(expr);
      DomainIdMap.insert(std::make_pair(expr, DomainVector.size() - 1));
    }
  }
}

bool AvailExprs::transferFunc(const Instruction &Inst, const DomainVal_t &IDV,
                              DomainVal_t &ODV) {

  /// @todo(CSCD70) Please complete this method.
  DomainVal_t TmpDV = IDV;

  if (isa<BinaryOperator>(Inst)) {
    dfa::Expression expr(*dyn_cast<BinaryOperator>(&Inst));

    auto iter = DomainIdMap.find(expr);
    if (iter != DomainIdMap.end()) {
      TmpDV[iter->second].Value = true;
    }
  }

  if (TmpDV.size() != ODV.size()) {
    ODV = TmpDV;
    return true;
  }

  for (size_t i = 0; i < TmpDV.size(); i++) {
    if (TmpDV[i].Value != ODV[i].Value) {
      ODV = TmpDV;
      return true;
    }
  }

  ODV = TmpDV;

  return false;
}
