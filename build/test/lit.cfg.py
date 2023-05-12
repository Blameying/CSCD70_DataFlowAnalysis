import lit
import lit.llvm

lit.llvm.initialize(lit_config, config)

from lit.llvm import llvm_config


config.name = "DFA"
config.test_format = lit.formats.ShTest()
config.test_source_root = "/home/blame/Workspace/MICS6000R_Assignment/CSCD70/Assignment2-Dataflow_Analysis/test"
config.test_exec_root = "/home/blame/Workspace/MICS6000R_Assignment/CSCD70/Assignment2-Dataflow_Analysis/build/test"
config.suffixes = [".c", ".ll"]

config.substitutions.append((r"%dylibdir", "/home/blame/Workspace/MICS6000R_Assignment/CSCD70/Assignment2-Dataflow_Analysis/build/lib"))

config.llvm_config_bindir = ""
llvm_config.add_tool_substitutions(
    ["opt", "FileCheck"], config.llvm_config_bindir
)
