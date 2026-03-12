#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/codegen.h"
#include "include/ast_printer.h" 

// ── pretty divider ────────────────────────────────────────────────────────────
void divider(const std::string& title) {
    std::string line(60, '=');
    std::cout << "\n" << line << "\n";
    std::cout << "  " << title << "\n";
    std::cout << line << "\n";
}

// ── print tokens table ────────────────────────────────────────────────────────
void printTokens(const std::vector<Token>& tokens) {
    std::cout << "  " << std::left
              << std::setw(6)  << "LINE"
              << std::setw(8)  << "COL"
              << std::setw(16) << "TYPE"
              << "VALUE\n";
    std::cout << "  " << std::string(56, '-') << "\n";
    for (auto& t : tokens) {
        if (t.type == TokenType::EOF_TOKEN) break;
        std::cout << "  " << std::left
                  << std::setw(6)  << t.line
                  << std::setw(8)  << t.col
                  << std::setw(16) << t.typeName()
                  << t.value << "\n";
    }
}

// ── run compiler ──────────────────────────────────────────────────────────────
void compile(const std::string& source, const std::string& programName) {
    std::cout << "\n";
    std::string topLine(60, '*');
    std::cout << topLine << "\n";
    std::cout << "  COMPILING: " << programName << "\n";
    std::cout << topLine << "\n";

    // ── Source ─────────────────────────────────────────────────────────────
    divider("SOURCE CODE");
    std::istringstream ss(source);
    std::string ln;
    int lineNo = 1;
    while (std::getline(ss, ln))
        std::cout << "  " << std::setw(3) << lineNo++ << " | " << ln << "\n";

    // ══════════════════════════════════════════════════════════════════════
    // STAGE 1: LEXER
    // ══════════════════════════════════════════════════════════════════════
    divider("STAGE 1 — LEXER  (Tokenization)");
    try {
        Lexer lexer(source);
        auto tokens = lexer.tokenize();
        std::cout << "  Tokens produced: " << tokens.size() - 1 << "\n\n";
        printTokens(tokens);

        // ══════════════════════════════════════════════════════════════════
        // STAGE 2: PARSER → AST
        // ══════════════════════════════════════════════════════════════════
        divider("STAGE 2 — PARSER  (AST Construction)");
        try {
            Parser parser(tokens);
            auto program = parser.parse();

            ASTPrinter printer;
            std::cout << printer.print(program.get());

            // ══════════════════════════════════════════════════════════════
            // STAGE 3: CODE GENERATOR
            // ══════════════════════════════════════════════════════════════
            divider("STAGE 3 — CODE GENERATOR  (TAC + Optimization)");
            CodeGen cg;

            // 3a. Generate TAC
            auto tac = cg.generateTAC(program.get());
            std::cout << "  [Before optimization: " << tac.size() << " instructions]\n\n";
            std::cout << cg.tacToString(tac);

            // 3b. Optimize
            divider("STAGE 3 — OPTIMIZER  (Constant Folding + Dead Code Elim)");
            size_t before = tac.size();
            cg.optimize(tac);
            size_t after = tac.size();
            std::cout << "  Instructions before: " << before << "\n";
            std::cout << "  Instructions after:  " << after  << "\n";
            std::cout << "  Eliminated:          " << (before - after) << "\n\n";
            std::cout << cg.tacToString(tac);

            // 3c. Emit assembly
            divider("STAGE 3 — ASSEMBLY EMISSION  (pseudo-x86)");
            std::cout << cg.emitAssembly(tac);

            std::cout << "\n  [OK] Compilation successful: " << programName << "\n";

        } catch (const ParseError& e) {
            std::cerr << "\n  [PARSE ERROR] " << e.what() << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "\n  [LEXER ERROR] " << e.what() << "\n";
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// TEST PROGRAMS
// ═════════════════════════════════════════════════════════════════════════════
int main() {
    // ── Test 1: Fibonacci ─────────────────────────────────────────────────
    std::string fib = R"(
func int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

func int main() {
    int result = fibonacci(10);
    print(result);
    return 0;
}
)";

    // ── Test 2: Loops + Arithmetic ────────────────────────────────────────
    std::string loops = R"(
func int sumTo(int n) {
    int sum = 0;
    int i = 1;
    while (i <= n) {
        sum = sum + i;
        i = i + 1;
    }
    return sum;
}

func void printTable(int n) {
    int i;
    for (i = 1; i <= 10; i = i + 1) {
        int val = n * i;
        print(val);
    }
}

func int main() {
    int s = sumTo(100);
    print(s);
    printTable(5);
    return 0;
}
)";

    // ── Test 3: Constant folding demo ─────────────────────────────────────
    std::string constFold = R"(
func int main() {
    int a = 3 + 4;
    int b = 10 * 2;
    int c = a + b;
    bool flag = 5 > 3;
    if (flag) {
        print(c);
    }
    return 0;
}
)";

    compile(fib,       "fibonacci.src");
    compile(loops,     "loops.src");
    compile(constFold, "constant_folding.src");

    return 0;
}
