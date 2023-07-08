import Data.List.Split
import Interpreter.Lexer 
import Interpreter.Parser

test_string = "defun return"
new_string = splitOn " " test_string

main :: IO ()
main = 
    print (tokenizeResult (tokenize new_string))
            
        
    

