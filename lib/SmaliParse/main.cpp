//===- main.cpp - ART-LEX --------------------------------------*- C++ -*-===//
//
//                     ANDROID REVERSE TOOLKIT
//
// This file is distributed under the GNU GENERAL PUBLIC LICENSE
// V3 License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// The file defines a test to SmaliParse.
//
//===----------------------------------------------------------------------===//

#include <iostream>
#include "Lexer.h"
#include <sstream>
#include <fstream>
#include "Parser.hpp"
using namespace std;
using namespace Analysis;

int main() {

    ifstream ifile;
    ifile.open ("/home/f8left/CodeSrc/ART/res/StubApplication.smali");
    // test Lexer
    if (ifile.is_open ()) {
        Lexer lexer;
        lexer.switch_streams (&ifile,nullptr);
        while(true) {
            auto token = lexer.get_next_token ();
            if(token.token () == 0)
                break;
            if(token.token () == Parser::token::TOKEN_EOL) {
                continue;
            }
            cout << " line :" << lexer.line ()
                 << " column :" << lexer.column ()
                 << " token :" << lexer.curTokenText () << endl;
//                 << " text :" << lexer.text() << endl;
        }
    }
    return 0;
}