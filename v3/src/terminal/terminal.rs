pub trait Terminal 
{
    pub fn SetupNonBlockingTerminal() -> void;
    pub fn ClearScreen() -> void;
    pub fn ScanKeyboardInput(&outdata:  Vec<char>) -> void;
    pub fn ReadPlatformNativeTerminalSize(&maxrow: u8, &maxcol: u8) -> void;
}