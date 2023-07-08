use crate::ast::*;
use crate::lexer::*;
use crate::debug::*;

use crate::TokenType::*;
use crate::LiteralType::*;
use crate::Statement::*;
use crate::Expression::*;

macro_rules! matchtokens 
{
    ($parser:ident, $($token:ident),+) => {{
        let mut result = false;
        $(
            result |= $parser.match_token($token);
        )*
        result
    }}
}

pub struct Parser 
{
    tokens: Vec<Token>,
    debug: Debug,
    index: usize,
    line: u16,
}

impl Parser 
{
    pub fn new(_tokens: Vec<Token>, _debug: Debug) -> Self 
    {
        Self
        {
            tokens: _tokens,
            debug: _debug,
            index: 0,
            line: 1,
        }
    }

    pub fn parse(self: &mut Self) -> Result<Vec<Statement>, String>
    {
        let mut statements = vec![];
        while !self.is_at_end()
        {
            self.line = self.tokens[self.index].line;
            match self.declaration()
            {
                Ok(val) => statements.push(val),
                Err(msg) => self.debug.error(msg),
            }
        }
        if self.debug.contains_error()
        {
            self.debug.output_errors();
            return Err("COMPILETIME ERROR: Program failed".to_string());
        }
        return Ok(statements);
    }
    
    fn declaration(self: &mut Self) -> Result<Statement, String>
    {
        if matchtokens!(self, Let)
        {
            match self.var_declaration()
            {
                Ok(val) => return Ok(val),
                Err(msg) => return Err(msg),
            }
        }
        match self.statement()
        {
            Ok(expr) => return Ok(expr),
            Err(msg) => return Err(msg),
        }
    }

    fn var_declaration(self: &mut Self) -> Result<Statement, String>
    {
        match self.consume(Identifier, format!("PARSER ERROR: Expected variable name on line {}", self.line))
        {
            Ok(val) =>
            {
                if matchtokens!(self, Equal)
                {
                    match self.expression()
                    {
                        Ok(expr) => 
                        {
                            match self.consume(SemiColon, format!("PARSER ERROR: Expected ';' after variable declaration on line {}", self.line))
                            {
                                Ok(x) =>
                                {
                                    match &val.literal 
                                    {
                                        Some(StrLit(x)) => return Ok(VarStatement { name: val, value: Box::from(expr) }),
                                        _ => return Err(format!("PARSER ERROR: Unexpected variable identifier on line {}", val.line)) 
                                    }
                                },
                                Err(msg) => return Err(msg),
                            }
                        },
                        Err(msg) => return Err(msg),
                    }
                }
                return Err(format!("PARSER ERROR: Expected '=' after variable identifier on line {}", self.line));
            },
            Err(msg) => return Err(msg),
        }
    }

    fn statement(self: &mut Self) -> Result<Statement, String>
    {
        if matchtokens!(self, Colon)
        {
            match self.block_statement()
            {
                Ok(expr) => return Ok(expr),
                Err(msg) => return Err(msg),
            }
        }
        match self.expr_statement()
        {
            Ok(val) => return Ok(val),
            Err(msg) => return Err(msg),
        }
    }

    fn expr_statement(self: &mut Self) -> Result<Statement, String>
    {
        match self.expression()
        {
            Ok(expr) =>
            {
                match self.consume(SemiColon, format!("PARSER ERROR: Expected ';' after statement on line {}", self.line))
                {
                    Ok(val) => return Ok(ExprStatement { expr: Box::from(expr) }),
                    Err(msg) => return Err(msg),
                }
            },
            Err(msg) => return Err(msg),
        }
    }

    fn block_statement(self: &mut Self) -> Result<Statement, String>
    {
        let mut stmts = vec![];
        while !self.check(End) && !self.is_at_end()
        {
            match self.declaration()
            {
                Ok(stmt) => stmts.push(Box::from(stmt)),
                Err(msg) => todo!(),
            }
        }
        match self.consume(End, format!("PARSER ERROR: Expected 'end' after block on line {}", self.line))
        {
            Ok(val) => return Ok(BlockStatement { statements: stmts }),
            Err(msg) => return Err(msg),
        }
    }

    fn expression(self: &mut Self) -> Result<Expression, String>
    {
        match self.assignment()
        {
            Ok(val) => return Ok(val),
            Err(msg) => return Err(msg),
        };
    }

    fn assignment(self: &mut Self) -> Result<Expression, String>
    {
        match self.equality()
        {
            Ok(expr) => 
            {
                if matchtokens!(self, Equal)
                {
                    match self.assignment()
                    {
                        Ok(val) => 
                        {
                            match expr 
                            {
                                Expression::Variable { name } => return Ok(Assign { name: name, value: Box::from(val) }),
                                _ => return Err(format!("Invalid assignment target on line {}", self.line)),
                            }
                        },
                        Err(msg) => return Err(msg),
                    }
                }
                return Ok(expr);
            },
            Err(msg) => return Err(msg),
        }
    }

    fn equality(self: &mut Self) -> Result<Expression, String>
    {
        match self.comparison()
        {
            Ok(expr) =>
            {
                while matchtokens!(self, BangEqual, EqualEqual)
                {
                    let op = self.previous();
                    match self.comparison()
                    {
                        Ok(right) => return Ok(Binary { left: Box::from(expr), op: op, right: Box::from(right) }),
                        Err(msg) => return Err(msg),
                    }
                }
                return Ok(expr);
            },
            Err(msg) => return Err(msg),
        }
    }

    fn comparison(self: &mut Self) -> Result<Expression, String>
    {
        match self.term()
        {
            Ok(expr) =>
            {
                while matchtokens!(self, Greater, GreaterEqual, Less, LessEqual)
                {
                    let op = self.previous();
                    match self.term()
                    {
                        Ok(right) => return Ok(Binary { left: Box::from(expr), op: op, right: Box::from(right) }),
                        Err(msg) => return Err(msg),
                    }
                }
                return Ok(expr);
            },
            Err(msg) => return Err(msg),
        };
    }

    fn term(self: &mut Self) -> Result<Expression, String>
    {
        match self.factor()
        {
            Ok(expr) =>
            {
                while matchtokens!(self, Minus, Plus)
                {
                    let op = self.previous();
                    match self.factor()
                    {
                        Ok(right) => return Ok(Binary { left: Box::from(expr), op: op, right: Box::from(right) }),
                        Err(msg) => return Err(msg),
                    }
                }
                return Ok(expr);
            },
            Err(msg) => return Err(msg),
        };
    }

    fn factor(self: &mut Self) -> Result<Expression, String>
    {
        match self.unary()
        {
            Ok(expr) => 
            {
                while matchtokens!(self, Slash, Star)
                {
                    let op = self.previous();
                    match self.unary()
                    {
                        Ok(right) => return Ok(Binary { left: Box::from(expr), op: op, right: Box::from(right) }),
                        Err(msg) => return Err(msg),
                    }
                }
                return Ok(expr);
            },
            Err(msg) => return Err(msg),
        };
    }

    fn unary(self: &mut Self) -> Result<Expression, String>
    {
        if matchtokens!(self, Bang, Minus)
        {
            let op = self.previous();
            match self.unary()
            {
                Ok(right) => return Ok(Unary { op: op, right: Box::from(right) }),
                Err(msg) => return Err(msg),
            }
        }
        match self.primary()
        {
            Ok(val) => return Ok(val),
            Err(msg) => return Err(msg),
        };
    }

    fn primary(self: &mut Self) -> Result<Expression, String>
    {
        if matchtokens!(self, True)
        {
            return Ok(Literal { value: self.previous(), ttype: BoolTok });
        }
        else if matchtokens!(self, False)
        {
            return Ok(Literal { value: self.previous(), ttype: BoolTok });
        }
        else if matchtokens!(self, IntTok)
        {
            return Ok(Literal { value: self.previous(), ttype: IntTok });
        }
        else if matchtokens!(self, FloatTok)
        {
            return Ok(Literal { value: self.previous(), ttype: FloatTok });
        }
        else if matchtokens!(self, StrTok)
        {
            return Ok(Literal { value: self.previous(), ttype: StrTok });
        }
        else if matchtokens!(self, LeftParen)
        {
            match self.expression()
            {
                Ok(val) => 
                {
                    match self.consume(RightParen, format!("ERROR: Expected ')' for expression on line {}", self.line))
                    {
                        Ok(cont) => return Ok(Grouping { expr: Box::from(val) }),
                        Err(msg) => return Err(msg),
                    }
                },
                Err(err_msg) => return Err(err_msg),
            }
        }
        else if matchtokens!(self, Identifier)
        {
            let tok = self.previous();
            match &tok.literal 
            {
                Some(StrLit(x)) => return Ok(Variable { name: tok }),
                _ => return Err(format!("PARSER ERROR: Unexpected variable identifier on line {}", tok.line)),
            }
        }
        return Err("PARSE LOOP ERROR".to_string());
    }

    fn consume(self: &mut Self, token: TokenType, msg: String) -> Result<Token, String>
    {
        if self.check(token)
        {
            return Ok(self.advance());
        }
        return Err(msg);
    }

    pub fn match_token(self: &mut Self, token: TokenType) -> bool 
    {
        if self.check(token)
        {
            self.advance();
            return true;
        }
        else
        {
            return false;
        }
    }

    fn check(self: &mut Self, token: TokenType) -> bool 
    {
        if self.is_at_end() 
        {
            return false;
        }
        return self.peek().tokentype == token;
    }

    fn previous(self: &mut Self) -> Token 
    {
        return self.tokens[self.index - 1].clone();
    }

    fn advance(self: &mut Self) -> Token 
    {
        if !self.is_at_end() 
        {
            self.index += 1;
        }
        return self.previous();
    }

    fn is_at_end(self: &mut Self) -> bool 
    {
        return self.peek().tokentype == EofTok;
    }

    fn peek(self: &mut Self) -> Token 
    {
        return self.tokens[self.index].clone();
    }
}

