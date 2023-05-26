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
            
            QualType returnType = functionDecl->getReturnType();
            if (!returnType->isVoidType()) {
                std::string variableName;
                QualType variableType;

                // Traverse parent nodes to find assignment or initialization expressions
                Stmt *parentStmt = findParentStmt(callExpr);
                if (parentStmt) {
                    if (auto *varDeclStmt = dyn_cast<DeclStmt>(parentStmt)) {
                        for (Decl *decl : varDeclStmt->decls()) {
                            if (auto *varDecl = dyn_cast<VarDecl>(decl)) {
                                variableName = varDecl->getNameAsString();
                                variableType = varDecl->getType();
                                break;
                            }
                        }
                    } else if (auto *binaryOperator = dyn_cast<BinaryOperator>(parentStmt)) {
                        if (binaryOperator->isAssignmentOp()) {
                            Expr *lhs = binaryOperator->getLHS();
                            if (auto *declRefExpr = dyn_cast<DeclRefExpr>(lhs)) {
                                if (auto *varDecl = dyn_cast<VarDecl>(declRefExpr->getDecl())) {
                                    variableName = varDecl->getNameAsString();
                                    variableType = varDecl->getType();
                                }
                            }
                        }
                    }
                }

                if (!variableName.empty()) {
                    llvm::outs() << "Return Variable: " << variableName << "\n";
                    llvm::outs() << "Return Type: " << variableType.getAsString() << "\n";
                }
            }
        }
        return true;
    }

private:
    /*std::string getSourceText(Expr *expr) {
        SourceManager &sourceMgr = expr->getASTContext().getSourceManager();
        SourceRange sourceRange = expr->getSourceRange();
        return Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), sourceMgr, expr->getASTContext().getLangOpts()).str();
    }*/
    std::string getSourceText(Expr *expr) {
        SourceManager &sourceMgr = expr->getBeginLoc().getManager();
        SourceRange sourceRange = expr->getSourceRange();
        return Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), sourceMgr, LangOptions()).str();
    }
    Stmt *findParentStmt(Expr *expr) {
        Stmt *parentStmt = nullptr;
        if (auto *parent = dyn_cast<Stmt>(expr->getParent())) {
            parentStmt = parent;
        } else if (auto *initExpr = dyn_cast<InitListExpr>(expr->getParent())) {
            if (auto *initExprParent = dyn_cast<Stmt>(initExpr->getParent()))
                parentStmt = initExprParent;
        }
        return parentStmt;
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
