use crate::TokenType::*;
use crate::LiteralType::*;
use crate::debug::*;

use std::collections::HashMap;


#[derive(Debug, Clone)]
pub enum LiteralType
{
    IntLit(i32), FloatLit(f32), StrLit(String), BoolLit(bool),
}

#[derive(Debug, Clone, PartialEq)]
pub enum TokenType
{
    Def, Return, End,
    LeftParen, RightParen, LeftBrace, RightBrace, LeftBracket, RightBracket,
    Arrow, Colon, ColonColon, Comma, SemiColon,
    If, Else, Match, For, While, Let,
    Plus, Minus, Star, Slash, 
    Bang, BangEqual, Equal, EqualEqual, 
    Greater, GreaterEqual, Less, LessEqual,
    Identifier, IntTok, FloatTok, StrTok, BoolTok, True, False,
    EofTok,
}

#[derive(Debug, Clone)]
pub struct Token 
{
    pub tokentype: TokenType,
    pub literal: Option<LiteralType>,
    pub line: u16,
}

impl Token 
{
    pub fn new(ttype: TokenType, lit: Option<LiteralType>, _line: u16) -> Self 
    {
        Self 
        {
            tokentype: ttype,
            literal: lit,
            line: _line,
        }
    }
}

pub struct Lexer 
{
    pub tokens: Vec<Token>,
    pub source: String,
    pub start: usize,
    pub current: usize,
    pub line: u16,
    pub debug: Debug,
    pub keywords: HashMap<String, TokenType>,
}

impl Lexer 
{
    pub fn new(_source: String) -> Self 
    {
        Self 
        {
            tokens: vec![],
            source: _source,
            start: 0,
            current: 0,
            line: 1,
            debug: Debug::new(),
            keywords: HashMap::from([
                ("def".to_string(), Def),
                ("return".to_string(), Return),
                ("end".to_string(), End),
                ("if".to_string(), If),
                ("else".to_string(), Else),
                ("match".to_string(), Match),
                ("for".to_string(), For),
                ("while".to_string(), While),
                ("let".to_string(), Let),
            ]),
        }
    }

    pub fn scan_tokens(self: &mut Self) -> (Vec<Token>, Debug)
    {
        while !self.is_at_end()
        {
            self.start = self.current;
            self.scan_token();
        }
        self.tokens.push(Token::new(EofTok, None, self.line));
        return (self.tokens.clone(), self.debug.clone());
    }

    fn is_at_end(self: &Self) -> bool 
    {
        return self.current >= self.source.len();
    }

    fn match_token(self: &mut Self, expected: char) -> bool 
    {
        if self.is_at_end() || self.source.chars().nth(self.current).unwrap() != expected
        {
            return false;
        }
        self.current += 1;
        return true;
    }

    fn is_digit(self: &Self, c: char) -> bool 
    {
        return c >= '0' && c <= '9';
    }

    fn is_alpha(self: &Self, c: char) -> bool 
    {
         return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    fn is_alpha_numeric(self: &mut Self, c: char) -> bool 
    {
        return self.is_alpha(c) || self.is_digit(c);
    }

    fn scan_token(self: &mut Self)
    {
        let c = self.advance();
        match c 
        {
            '(' => self.add_token(LeftParen, None),
            ')' => self.add_token(RightParen, None),
            '[' => self.add_token(LeftBracket, None),
            ']' => self.add_token(RightBracket, None),
            '{' => self.add_token(LeftBrace, None),
            '}' => self.add_token(RightBrace, None),
            ',' => self.add_token(Comma, None),
            '*' => self.add_token(Star, None),
            '+' => self.add_token(Plus, None),
            ';' => self.add_token(SemiColon, None),
            '"' => self.lex_string(),
            '\n' => self.line += 1,
            '-' => 
            {
                match self.match_token('>')
                {
                    true => self.add_token(Arrow, None),
                    false => self.add_token(Minus, None),
                }
            },
            '/' =>
            {
                if self.match_token('/')
                {
                    while self.peek() != '\n' && !self.is_at_end()
                    {
                        self.advance();
                    }
                }
                else 
                {
                    self.add_token(Slash, None);
                }
            },
            '!' =>
            {
                match self.match_token('=')
                {
                    true => self.add_token(BangEqual, None),
                    false => self.add_token(Bang, None),
                }
            },
            '=' =>
            {
                match self.match_token('=')
                {
                    true => self.add_token(EqualEqual, None),
                    false => self.add_token(Equal, None),
                }
            },
            '>' =>
            {
                match self.match_token('=')
                {
                    true => self.add_token(GreaterEqual, None),
                    false => self.add_token(Greater, None),
                }
            },
            '<' => 
            {
                match self.match_token('=')
                {
                    true => self.add_token(LessEqual, None),
                    false => self.add_token(Less, None),
                }
            },
            ':' =>
            {
                match self.match_token(':')
                {
                    true => self.add_token(ColonColon, None),
                    false => self.add_token(Colon, None),
                }
            },
            _ => 
            {
                if c.is_whitespace()
                {
                    return;
                }
                if self.is_digit(c)
                {
                    self.lex_number();
                }
                else if self.is_alpha(c)
                {
                    self.lex_identifier();
                }
                else 
                {
                    self.debug.error("ERROR: Unexpected character".to_string());
                }
            }
        }
    }

    fn lex_string(self: &mut Self)
    {
        while self.peek() != '"' && !self.is_at_end()
        {
            if self.peek() == '\n'
            {
                self.line += 1;
            }
            self.advance();
        }
        if self.is_at_end()
        {
            self.debug.error("ERROR: Unterminated string".to_string());
            return;
        }
        self.advance();
        let temp: String = self.source.chars().skip(self.start + 1).take(self.current - 2 - self.start).collect();
        self.add_token(StrTok, Some(StrLit(temp)));
    }

    fn lex_number(self: &mut Self)
    {
        let mut is_float = false;
        while self.is_digit(self.peek())
        {
            self.advance();
        }
        if self.peek() == '.' && self.is_digit(self.peek_next())
        {
            is_float = true;
            self.advance();
            while self.is_digit(self.peek())
            {
                self.advance();
            }
        }
        if !is_float
        {
            let int: String = self.source.chars().skip(self.start).take(self.current - self.start).collect();
            match int.parse::<i32>()
            {
                Ok(int_res) => self.add_token(IntTok, Some(IntLit(int_res))),
                Err(msg) => println!("INT ERROR: {}, STRING {}", msg.to_string(), int),
            }
        }
        else 
        {
            let float: String = self.source.chars().skip(self.start).take(self.current - self.start).collect();
            match  float.parse::<f32>()
            {
                Ok(float_res) => self.add_token(FloatTok, Some(FloatLit(float_res))),
                Err(msg) => println!("FLOAt ERROR: {}", msg.to_string()),
            }
        }
    }

    fn lex_identifier(self: &mut Self)
    {
        while self.is_alpha_numeric(self.peek())
        {
            self.advance();
        }
        let text: String = self.source.chars().skip(self.start).take(self.current - self.start).collect();
        match self.keywords.get(&text)
        {
            Some(contents) => self.add_token(contents.clone(), None),
            None => self.add_token(Identifier, Some(StrLit(text))),
        }
    }
    
    fn add_token(self: &mut Self, token: TokenType, literal: Option<LiteralType>)
    {
        self.tokens.push(Token::new(token, literal, self.line));
    }

    fn advance(self: &mut Self) -> char
    {
        self.current += 1;
        match self.source.chars().nth(self.current - 1)
        {
            Some(content) => return content,
            None => return '\0',
        }
    }

    fn peek(self: &Self) -> char
    {
        if self.is_at_end()
        {
            return '\0';
        }
        let x = self.source.chars().nth(self.current).unwrap();
        return x;
    }

    fn peek_next(self: &Self) -> char 
    {
        if self.current + 1 >= self.source.len()
        {
            return '\0';
        }
        return self.source.chars().nth(self.current + 1).unwrap();
    }

}
