module Interpreter.Parser
(
) where

import Interpreter.Lexer

data AST = 
    Expr AST Char AST | Unary Char AST 
