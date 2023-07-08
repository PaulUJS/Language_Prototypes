module Interpreter.Lexer
(
    tokenizeResult
    , tokenize
    , matchToken
) where 

import Data.Either
import Text.Read 
import Data.Typeable

data Token = 
    DEFUN Int | RETURN Int | EOF Int |
    PLUS Int | MINUS Int | STAR Int | SLASH Int |
    S_LIT Int [Char] | NUM_LIT Int Double | TRUE Int | FALSE Int |
    LET Int | IDENTIFIER Int [Char] | NUM Int | STRING Int | BOOL Int 
    deriving(Show)

tokenizeResult :: ([[Char]], [Token]) -> Either [[Char]] [Token]
tokenizeResult r = 
    if fst r == []
        then Right (snd r)
    else Left (fst r)

tokenize :: [[Char]] -> ([[Char]], [Token])
tokenize s = partitionEithers (fmap matchToken s)

matchToken :: [Char] -> Either [Char] Token
matchToken c
    | c == "defun" = Right (DEFUN 1)
    | c == "return" = Right (RETURN 1)
    | c == "let" = Right (LET 1)
    | c == "num" = Right (NUM 1)
    | c == "true" = Right (TRUE 1)
    | c == "false" = Right (FALSE 1)
    | c == "bool" = Right (BOOL 1)
    | c == "string" = Right (STRING 1)
    | c == "+" = Right (PLUS 1)
    | c == "-" = Right (MINUS 1)
    | c == "*" = Right (STAR 1)
    | c == "/" = Right (SLASH 1)
    | otherwise = Left ("UNKNOWN TOKEN " <> c)

