use crate::lexer::*;
use crate::TokenType::*;
use crate::LiteralType::*;
use crate::Expression::*;

#[derive(Debug)]
pub enum Expression 
{
    Binary { left: Box<Expression>, op: Token, right: Box<Expression> },
    Unary { op: Token, right: Box<Expression> },
    Literal { value: Token, ttype: TokenType },
    Grouping { expr: Box<Expression> },
    Assign { name: Token, value: Box<Expression> },
    Variable { name: Token, },
}

impl Expression
{
    pub fn evaluate(self: &mut Self) -> Result<LiteralType, String>
    {
        match self 
        {
            Self::Binary { left, op, right } => 
            {
                match (left.evaluate(), right.evaluate(), &op.tokentype)
                {
                    (Ok(IntLit(x)), Ok(IntLit(y)), Plus) => return Ok(IntLit(x + y)),
                    (Ok(IntLit(x)), Ok(IntLit(y)), Minus) => return Ok(IntLit(x - y)),
                    (Ok(IntLit(x)), Ok(IntLit(y)), Star) => return Ok(IntLit(x * y)),
                    (Ok(IntLit(x)), Ok(IntLit(y)), Slash) => return Ok(IntLit(x / y)),
                    (Ok(IntLit(x)), Ok(IntLit(y)), Greater) => return Ok(BoolLit(x > y)),
                    (Ok(IntLit(x)), Ok(IntLit(y)), GreaterEqual) => return Ok(BoolLit(x >= y)),
                    (Ok(IntLit(x)), Ok(IntLit(y)), Less) => return Ok(BoolLit(x < y)),
                    (Ok(IntLit(x)), Ok(IntLit(y)), LessEqual) => return Ok(BoolLit(x <= y)),
                    (Ok(IntLit(x)), Ok(IntLit(y)), EqualEqual) => return Ok(BoolLit(x == y)),
                    (Ok(IntLit(x)), Ok(IntLit(y)), BangEqual) => return Ok(BoolLit(x != y)),
                    (Ok(FloatLit(x)), Ok(FloatLit(y)), Plus) => return Ok(FloatLit(x + y)),
                    (Ok(FloatLit(x)), Ok(FloatLit(y)), Minus) => return Ok(FloatLit(x - y)),
                    (Ok(FloatLit(x)), Ok(FloatLit(y)), Star) => return Ok(FloatLit(x * y)),
                    (Ok(FloatLit(x)), Ok(FloatLit(y)), Slash) => return Ok(FloatLit(x / y)),
                    (Ok(FloatLit(x)), Ok(FloatLit(y)), Greater) => return Ok(BoolLit(x > y)),
                    (Ok(FloatLit(x)), Ok(FloatLit(y)), GreaterEqual) => return Ok(BoolLit(x >= y)),
                    (Ok(FloatLit(x)), Ok(FloatLit(y)), Less) => return Ok(BoolLit(x < y)),
                    (Ok(FloatLit(x)), Ok(FloatLit(y)), LessEqual) => return Ok(BoolLit(x <= y)),
                    (Ok(FloatLit(x)), Ok(FloatLit(y)), EqualEqual) => return Ok(BoolLit(x == y)),
                    (Ok(FloatLit(x)), Ok(FloatLit(y)), BangEqual) => return Ok(BoolLit(x != y)),
                    (Ok(IntLit(_)), _, _) => return Err(format!("RUNTIME ERROR: Cannot do operations on non-matching types on line {}", op.line)),
                    (Ok(FloatLit(_)), _, _) =>return Err(format!("RUNTIME ERROR: Cannot do operations on non-matching types on line {}", op.line)),
                    (Ok(StrLit(_)), _, _) => return Err(format!("RUNTIME ERROR: Cannot do binary operations on type STRING on line {}", op.line)),
                    (Ok(BoolLit(_)), _, _) => return Err(format!("RUNTIME ERROR: Cannot do binary operations on type BOOL on line {}", op.line)),
                    (Err(left), _, _,) => return Err(left),
                    (_, Err(right), _) => return Err(right),
                    (Err(left), Err(right), _) => return Err(format!("{}\n{}", left, right)),
                    _ => todo!(),
                }
            },
            Self::Unary { op, right } => 
            {
                match op.tokentype 
                {
                    Bang => todo!(),
                    _ => todo!(),
                }
            },
            Self::Literal { value, ttype } => 
            {
                match &value.literal 
                {
                    Some(IntLit(val)) => return Ok(IntLit(*val)),
                    Some(FloatLit(val)) => return Ok(FloatLit(*val)),
                    Some(StrLit(val)) => return Ok(StrLit(val.to_string())),
                    Some(BoolLit(val)) => return Ok(BoolLit(*val)),
                    None => todo!(),
                }
            },
            Self::Grouping { expr } => 
            {
                match expr.evaluate()
                {
                    Ok(val) => return Ok(val),
                    Err(msg) => return Err(msg),
                }
            },
            _ => todo!(),
        }
    }

}

#[derive(Debug)]
pub enum Statement
{
    ExprStatement { expr: Box::<Expression> },
    VarStatement { name: Token,  value: Box<Expression> },
    BlockStatement { statements: Vec<Box<Statement>> },
}

impl Statement
{
    pub fn evaluate(self: &mut Self) -> Result<LiteralType, String>
    {
        match self 
        {
            Self::ExprStatement { expr } => 
            {
                match expr.evaluate()
                {
                    Ok(val) => return Ok(val),
                    Err(msg) => return Err(msg),
                }
            },
            Self::VarStatement { name, value } =>
            {
                match value.evaluate()
                {
                    Ok(val) => return Ok(val),
                    Err(msg) => return Err(msg),
                }
            },
            Self::BlockStatement { statements } => todo!(),
        }
    }
}
