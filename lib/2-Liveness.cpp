#include "DFA.h"
#include "DFA/Domain/Variable.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <llvm/IR/Argument.h>
#include <llvm/Support/Casting.h>
#include <utility>
/// @todo(CSCD70) Please complete this file.

using namespace llvm;

AnalysisKey Liveness::Key;

void Liveness::initializeDomainFromInst(const llvm::Instruction &Inst) {
  for (const auto &Op : Inst.operands()) {
    /* Only care the instruction-define value and Argument */
    if (isa<Instruction>(Op) || isa<Argument>(Op)) {
      dfa::Variable VAR(Op);
      if (DomainIdMap.count(VAR) == 0) {
        DomainVector.push_back(VAR);
        DomainIdMap.insert(std::make_pair(VAR, DomainVector.size() - 1));
      }
    }
  }
}

bool Liveness::transferFunc(const Instruction &Inst, const DomainVal_t &IDV,
                            DomainVal_t &ODV) {

  /// @todo(CSCD70) Please complete this method.
  DomainVal_t TmpDV = IDV;

  const Value *ValueInst = dyn_cast<llvm::Value>(&Inst);

  for (auto &Var : DomainVector) {
    if (Var.Var == ValueInst) {
      TmpDV[DomainIdMap.find(Var)->second].Value = false;
    }
  }

  for (auto &Op : Inst.operands()) {
    if (isa<Instruction>(Op) || isa<Argument>(Op)) {
      TmpDV[DomainIdMap.find(dfa::Variable(Op))->second].Value = true;
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
