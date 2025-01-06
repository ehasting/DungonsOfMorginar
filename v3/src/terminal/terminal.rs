pub trait Terminal 
{
    fn SetupNonBlockingTerminal() -> void;
    fn ClearScreen() -> void;
    fn ScanKeyboardInput(&outdata:  Vec<char>) -> void;
    fn ReadPlatformNativeTerminalSize(&maxrow: u8, &maxcol: u8) -> void;
}