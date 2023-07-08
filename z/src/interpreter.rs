use crate::ast::*;
use crate::Expression::*;
use crate::Statement::*;
use crate::env::*;
use crate::lexer::LiteralType;

pub struct Interpreter
{
    env: Environment,
}

impl Interpreter
{
    pub fn new() -> Self 
    {
        Self 
        {
            env: Environment::new(),
        }
    }

    pub fn interpret(self: &mut Self, stmt: &mut Statement) -> Result<LiteralType, String>
    {
        match stmt 
        {
            ExprStatement { expr } => 
            {
                match self.execute_expr_stmt(expr)
                {
                    Ok(val) => return Ok(val),
                    Err(msg) => return Err(msg),
                }
            },
            VarStatement { name, value } => 
            {
                match self.execute_var_stmt(value)
                {
                    Ok(val) => 
                    {
                        match &name.literal
                        {
                            Some(LiteralType::StrLit(s)) => 
                            {
                                match self.env.define(s.to_string(), val.clone())
                                {
                                    Some(x) => return Ok(x),
                                    None => return Ok(val),
                                }
                            },
                            _ => return Err(format!("RUNTIME ERROR: Unexpected variable name on line {}", name.line)),                            
                        }
                    },
                    Err(msg) => return Err(msg),
                }
            },
            BlockStatement { statements } => 
            {
                todo!();
            },
            _ => todo!(),
        }
    }

    fn execute_expr_stmt(self: &Self, expr: &mut Box<Expression>) -> Result<LiteralType, String>
    {
        match expr.evaluate()
        {
            Ok(val) => return Ok(val),
            Err(msg) => return Err(msg),
        }
    }

    fn execute_var_stmt(self: &Self, expr: &mut Box<Expression>) -> Result<LiteralType, String>
    {
        match expr.evaluate()
        {
            Ok(val) => return Ok(val),
            Err(msg) => return Err(msg),
        }
    }

    fn execute_block_stmt(self: &Self, expr: &mut Box<Expression>) -> Result<LiteralType, String>
    {
        match expr.evaluate()
        {
            Ok(val) => return Ok(val),
            Err(msg) => return Err(msg),
        }
    }

}
