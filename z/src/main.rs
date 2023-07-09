use std::fs;

pub mod debug;
use crate::debug::*;

pub mod lexer;
use crate::lexer::*;

pub mod ast;
use crate::ast::*;

pub mod parser;
use crate::parser::*;

pub mod env;
use crate::env::*;

pub mod interpreter;
use crate::interpreter::*;

fn main() 
{
    let file = fs::read_to_string("test.txt");
    let mut file_str: String = " ".to_string();
    match file 
    {
        Ok(contents) => file_str = contents,
        Err(msg) => println!("{:#?}", msg.to_string()),
    }

    let mut lexer = Lexer::new(file_str);
    let (tokens, debug) = lexer.scan_tokens();

    let mut parser = Parser::new(tokens, debug);
    match parser.parse()
    {
        Ok(val) => 
        {
            println!("PARSING OVER");
            let mut interpreter = Interpreter::new();
            for mut x in val 
            {
                match interpreter.interpret(&mut x)
                {
                    Ok(output) => println!("{:#?}", output),
                    Err(msg) => println!("{}", msg),
                }
            }
        },
        Err(msg) => println!("{}", msg),
    }
}
