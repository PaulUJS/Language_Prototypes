#[derive(Debug, Clone)]
pub struct Debug 
{
    errors: Vec<String>,
}

impl Debug 
{
    pub fn new() -> Self
    {
        Self 
        {
            errors: vec![],
        }
    }

    pub fn error(self: &mut Self, msg: String)
    {
        self.errors.push(msg);
    }

    pub fn output_errors(self: &mut Self)
    {
        for x in &self.errors 
        {
            println!("{}", x);
        }
    }

    pub fn contains_error(self: &mut Self) -> bool 
    {
        if self.errors.is_empty()
        {
            return false;
        }
        return true;
    }
}
