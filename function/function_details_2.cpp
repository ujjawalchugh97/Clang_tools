#include "clang/AST/ASTConsumer.h"
#include "clang/AST/Decl.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

class FunctionCallVisitor : public RecursiveASTVisitor<FunctionCallVisitor> {
public:
    bool VisitCallExpr(CallExpr *callExpr) {
        // Extract function name
        FunctionDecl *functionDecl = callExpr->getDirectCallee();
        if (functionDecl != nullptr) {
            llvm::outs() << "Function Name: " << functionDecl->getNameAsString() << "\n";

            // Extract function arguments
            llvm::outs() << "Arguments:\n";
            unsigned numArgs = callExpr->getNumArgs();
            for (unsigned i = 0; i < numArgs; ++i) {
                Expr *arg = callExpr->getArg(i);
                QualType argType = arg->getType();
                llvm::outs() << "  - Type: " << argType.getAsString() << "\n";
                llvm::outs() << "    Value: " << getSourceText(arg) << "\n";
            }
        }
        
        // Check if the function call returns something

            QualType returnType = functionDecl->getReturnType();

            if (!returnType->isVoidType()) {

                Expr *calleeExpr = callExpr->getCallee();

                if (isa<DeclRefExpr>(calleeExpr)) {

                    DeclRefExpr *declRefExpr = cast<DeclRefExpr>(calleeExpr);

                    ValueDecl *valueDecl = declRefExpr->getDecl();

                    llvm::outs() << "Return Variable: " << valueDecl->getNameAsString() << "\n";

                    llvm::outs() << "Return Type: " << returnType.getAsString() << "\n";

                }

            }

        return true;
    }

private:
    std::string getSourceText(Expr *expr) {
        SourceManager &sourceMgr = expr->getASTContext().getSourceManager();
        SourceRange sourceRange = expr->getSourceRange();
        return Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), sourceMgr, expr->getASTContext().getLangOpts()).str();
    }
    std::string getSourceText(Expr *expr) {
        SourceManager &sourceMgr = expr->getBeginLoc().getManager();
        SourceRange sourceRange = expr->getSourceRange();
        return Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), sourceMgr, LangOptions()).str();
    }
};

class FunctionCallConsumer : public ASTConsumer {
public:
    bool HandleTopLevelDecl(DeclGroupRef declGroup) override {
        for (Decl *decl : declGroup) {
            FunctionCallVisitor visitor;
            visitor.TraverseDecl(decl);
        }
        return true;
    }
};

extern "C" FrontendPluginRegistry::Add<FunctionCallConsumer>
X("function-call-printer", "Print function call details");
