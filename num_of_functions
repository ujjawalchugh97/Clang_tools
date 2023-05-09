#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

static cl::OptionCategory ToolCategory("FunctionCount Tool Options");

// AST visitor that counts function declarations
class FunctionCountVisitor : public RecursiveASTVisitor<FunctionCountVisitor> {
public:
  bool VisitFunctionDecl(FunctionDecl *funcDecl) {
    if (!funcDecl->isImplicit())
      ++FunctionCount;
    return true;
  }

  int getFunctionCount() const { return FunctionCount; }

private:
  int FunctionCount = 0;
};

// AST consumer that uses the visitor to count function declarations
class FunctionCountConsumer : public ASTConsumer {
public:
  void HandleTranslationUnit(ASTContext &context) override {
    FunctionCountVisitor visitor;
    visitor.TraverseDecl(context.getTranslationUnitDecl());
    llvm::outs() << "Number of function declarations: "
                 << visitor.getFunctionCount() << "\n";
  }
};

// Frontend action that creates the AST consumer
class FunctionCountAction : public ASTFrontendAction {
public:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &compiler,
                                                 StringRef inFile) override {
    return std::make_unique<FunctionCountConsumer>();
  }
};

// Main entry point for the tool
int main(int argc, const char **argv) {
  CommonOptionsParser optionsParser(argc, argv, ToolCategory);
  ClangTool tool(optionsParser.getCompilations(),
                 optionsParser.getSourcePathList());

  return tool.run(newFrontendActionFactory<FunctionCountAction>().get());
}

/*
Compile: 

$ clang++ -std=c++11 -I<path_to_clang_includes> -L<path_to_clang_libs> -lclangTooling -lclangASTMatchers -lclangAST -lclangFrontend -lclangToolingCore -lclangBasic -lLLVM -o functioncount your_tool.cpp

Run:
$ ./functioncount <source_file.cpp>

*/
