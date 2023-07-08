use std::borrow::BorrowMut;
use std::collections::HashMap;
use std::rc::Rc;

use crate::lexer::*;
use crate::LiteralType::*;
use crate::ast::*;
use crate::Expression::*;
use crate::Statement::*;
use crate::debug::*;

pub struct Environment
{
    values: HashMap<String, LiteralType>,
    env: Option<Box::<Environment>>,
}

impl Environment
{
    pub fn new() -> Self 
    {
        Self 
        {
            values: HashMap::new(),
            env: None,
        }
    }

    pub fn new_scope(self: &mut Self, env: Environment)
    {
        self.env = Some(Box::from(env));
    }

    pub fn define(self: &mut Self, key: String, value: LiteralType) -> Option<LiteralType>
    {
        match self.values.insert(key, value) 
        {
            Some(x) => return Some(x),
            None => return None,
        }
    }

    pub fn assign(self: &mut Self, key: String, value: LiteralType) -> Result<LiteralType, String>
    {
        let val = self.values.get(&key);
        match (val, &mut self.env)
        {
            (Some(_), _) => return Ok(self.values.insert(key, value).unwrap()),
            (None, Some(env)) =>
            {
                match env.assign(key, value)
                {
                    Ok(new_val) => return Ok(new_val),
                    Err(msg) => return Err(msg),
                }
            },
            (_, _) => return Err(format!("RUNTIME ERROR: Undefined variable {}", key)),
        }
    }

    pub fn check(self: &mut Self, key: &String) -> Result<LiteralType, String>
    {
        match self.values.get(key)
        {
            Some(content) => return Ok(content.clone()),
            None => return Err(format!("There is no variable '{}'", key)),
        }
    }
}
