struct ScreenPos
{
    let _col: u32;
    let _row: u32;
}

impl ScreenPos
{
    fn set_col(&self, col: u32)
    {
        if (col >= 1)
        {
            self._col = col;
            return
        }
        panic!(format!("col out of bound: {col}", col = col))        
    }

    fn set_row(&self, row: u32)
    {
        if (row >= 1)
        {
            self._row = row;
            return
        }
        panic!(format!("row out of bound: {row}", row = row))
        
    }
}