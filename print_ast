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

static cl::OptionCategory ToolCategory("ASTPrinter Tool Options");

// AST visitor that prints information about each AST node

class ASTPrinterVisitor : public RecursiveASTVisitor<ASTPrinterVisitor> {

public:

  bool VisitStmt(Stmt *s) {

    llvm::outs() << "Statement: " << s->getStmtClassName() << "\n";

    return true;

  }

  bool VisitType(Type *t) {

    llvm::outs() << "Type: " << t->getTypeClassName() << "\n";

    return true;

  }

  bool VisitDecl(Decl *d) {

    llvm::outs() << "Declaration: " << d->getDeclKindName() << "\n";

    return true;

  }

};

// AST consumer that uses the visitor to traverse the AST

class ASTPrinterConsumer : public ASTConsumer {

public:

  void HandleTranslationUnit(ASTContext &context) override {

    ASTPrinterVisitor visitor;

    visitor.TraverseDecl(context.getTranslationUnitDecl());

  }

};

// Frontend action that creates the AST consumer

class ASTPrinterAction : public ASTFrontendAction {

public:

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &compiler,

                                                 StringRef inFile) override {

    return std::make_unique<ASTPrinterConsumer>();

  }

};

// Main entry point for the tool

int main(int argc, const char **argv) {

  CommonOptionsParser optionsParser(argc, argv, ToolCategory);

  ClangTool tool(optionsParser.getCompilations(),

                 optionsParser.getSourcePathList());

  return tool.run(newFrontendActionFactory<ASTPrinterAction>().get());

}

