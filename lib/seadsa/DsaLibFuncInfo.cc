
#include "llvm/ADT/Twine.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/PassRegistry.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Registry.h"

#include "seadsa/DsaLibFuncInfo.hh"
#include "seadsa/Graph.hh"
#include "seadsa/InitializePasses.hh"
#include "seadsa/support/Debug.h"
#include <iostream>

static llvm::cl::list<std::string>
    XSpecFiles("sea-dsa-specfile", llvm::cl::desc("<Input spec bitcode file>"),
               llvm::cl::ZeroOrMore, llvm::cl::value_desc("filename"));

namespace seadsa {

void DsaLibFuncInfo::getAnalysisUsage(llvm::AnalysisUsage &AU) const {
  AU.addRequired<seadsa::AllocWrapInfo>();
  AU.addRequired<llvm::TargetLibraryInfoWrapperPass>();
  AU.setPreservesAll();
}

void DsaLibFuncInfo::initialize() const {

  using namespace llvm::sys::fs;
  using namespace llvm;

  m_awi = &getAnalysis<AllocWrapInfo>();
  m_tliWrapper = &getAnalysis<TargetLibraryInfoWrapperPass>();

  if (isInitialized || XSpecFiles.empty()) return;
  isInitialized = true;

  SMDiagnostic err;

  for (auto &specFile : XSpecFiles) {
    m_modules.push_back(parseIRFile(specFile, err, m_ctx));

    auto &spec_funcs = m_modules.back()->getFunctionList();

    for (llvm::Function &func : spec_funcs) {
      if (func.isDeclaration() || func.empty()) continue;
      m_funcs.insert({func.getName(), &func});
    }
  }
} // namespace seadsa

bool DsaLibFuncInfo::hasSpecFunc(const llvm::Function &F) const {
  return m_funcs.count(F.getName());
}

llvm::Function *DsaLibFuncInfo::getSpecFunc(const llvm::Function &F) const {
  auto it = m_funcs.find(F.getName());
  assert(it != m_funcs.end());
  return it->second;
}

char DsaLibFuncInfo::ID = 0;
} // namespace seadsa

namespace seadsa {
llvm::Pass *createDsaLibFuncInfoPass() { return new DsaLibFuncInfo(); }
} // namespace seadsa

using namespace seadsa;
using namespace llvm;

INITIALIZE_PASS_BEGIN(DsaLibFuncInfo, "seadsa-spec-graph-info",
                      "Creates local analysis from spec", false, false)
INITIALIZE_PASS_DEPENDENCY(AllocWrapInfo)
INITIALIZE_PASS_DEPENDENCY(TargetLibraryInfoWrapperPass)
INITIALIZE_PASS_END(DsaLibFuncInfo, "seadsa-spec-graph-info",
                    "Creates local analysis from spec", false, false)
