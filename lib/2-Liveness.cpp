#include "DFA.h"
#include "DFA/Domain/Variable.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/raw_ostream.h>
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

  const llvm::BasicBlock *parent = Inst.getParent();
  const Instruction *last_inst = parent->getTerminator();
  if (last_inst) {
    unsigned number_of_successors = last_inst->getNumSuccessors();
    llvm::outs() << "nums: " << number_of_successors << "\n";
    for (unsigned is = 0; is < number_of_successors; is++) {
      const llvm::BasicBlock *successor = last_inst->getSuccessor(is);
      // Process the successor block
      if (successor) {
        for (const Instruction &inst : *successor) {
          if (inst.getOpcode() == Instruction::PHI) {
            const llvm::PHINode *node = dyn_cast<llvm::PHINode>(&inst);
            unsigned numbers = node->getNumIncomingValues();
            for (unsigned i = 0; i < numbers; i++) {
              auto label = node->getIncomingBlock(i);
              if (label != parent) {
                auto value = node->getIncomingValue(i);
                auto id = DomainIdMap.find(value);
                if (id != DomainIdMap.end()) {
                  TmpDV[id->second].Value = false;
                }
              }
            }
          } else {
            break;
          }
        }
      }
    }
  }

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
