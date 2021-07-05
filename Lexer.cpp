#include "Lexer.h"
#include "ColonAutomaton.h"
#include "ColonDashAutomaton.h"
#include "MatchAutomaton.h"
#include "SchemesAutomaton.h"
#include "FactsAutomaton.h"
#include "RulesAutomaton.h"
#include "QueriesAutomaton.h"
#include "IdAutomaton.h"
#include "StringAutomaton.h"
#include "CommentAutomaton.h"
#include <cctype>

Lexer::Lexer() {
    CreateAutomata();
}

Lexer::~Lexer() {
    // TODO: need to clean up the memory in `automata` and `tokens`
    for (unsigned int i = 0; i <automata.size(); i++) {
        delete automata.at(i);
    }
    automata.clear();
    for (unsigned int i = 0; i <tokens.size(); i++) {
        delete tokens.at(i);
    }
    tokens.clear();
}

void Lexer::CreateAutomata() {
    automata.push_back(new ColonAutomaton());
    automata.push_back(new ColonDashAutomaton());
    automata.push_back(new MatchAutomaton());
    automata.push_back(new SchemesAutomaton());
    automata.push_back(new FactsAutomaton());
    automata.push_back(new RulesAutomaton());
    automata.push_back(new QueriesAutomaton());
    automata.push_back(new IdAutomaton());
    automata.push_back(new StringAutomaton());
    automata.push_back(new CommentAutomaton());
}

void Lexer::Run(std::string& input) {
    // TODO: convert this pseudo-code with the algorithm into actual C++ code
    int lineNumber = 1;
    while (input.size() > 0) {
        int maxRead = 0;
        Automaton* maxAutomaton = automata.at(0);
        if (!isspace(input.at(0))){
            for (unsigned int i = 0; i < automata.size(); i++) {
                 int inputRead = automata.at(i)->Start(input);
                 if (inputRead > maxRead) {
                     maxRead = inputRead;
                     maxAutomaton = automata.at(i);
                 }
            }
            if (maxRead > 0) {
                Token* newToken = maxAutomaton->CreateToken(input.substr(0,maxRead),lineNumber);
                lineNumber += maxAutomaton->NewLinesRead();
                tokens.push_back(newToken);
                //std::cout << newToken->ToString() << std::endl;
            } else {
                maxRead = 1;
                Token* newToken = new Token(TokenType::UNDEFINED, input.substr(0,maxRead), lineNumber);
                tokens.push_back(newToken);
            }
            input.erase(input.begin(), input.begin()+maxRead);
        } else {
            if (input.at(0) == '\n') {
                lineNumber++;
            }
            input.erase(input.begin(), input.begin()+1);
        }
    }
    tokens.push_back(new Token(TokenType::EOF_TYPE, "" , lineNumber));
    /*
    set lineNumber to 1
    // While there are more characters to tokenize
    loop while input.size() > 0 {
        set maxRead to 0
        set maxAutomaton to the first automaton in automata

        // TODO: you need to handle whitespace inbetween tokens

        // Here is the "Parallel" part of the algorithm
        //   Each automaton runs with the same input
        foreach automaton in automata {
            inputRead = automaton.Start(input)
            if (inputRead > maxRead) {
                set maxRead to inputRead
                set maxAutomaton to automaton
            }
        }
        // Here is the "Max" part of the algorithm
        if maxRead > 0 {
            set newToken to maxAutomaton.CreateToken(...)
                increment lineNumber by maxAutomaton.NewLinesRead()
                add newToken to collection of all tokens
        }
        // No automaton accepted input
        // Create single character undefined token
        else {
            set maxRead to 1
                set newToken to a  new undefined Token
                (with first character of input)
                add newToken to collection of all tokens
        }
        // Update `input` by removing characters read to create Token
        remove maxRead characters from input
    }
    add end of file token to all tokens
    */
    for(unsigned int i = 0; i < tokens.size(); i++) {
        std::cout << tokens.at(i)->ToString() << std::endl;
    }
    std::cout << "Total Tokens = " << tokens.size();
}
