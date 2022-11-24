
use std::io::{stdout, Write, Read};

use crossterm::{
    execute,
    style::{Color, Print, ResetColor, SetBackgroundColor, SetForegroundColor},
    ExecutableCommand, Result,
    event,
};
use std::collections::VecDeque;

use std::io;
use std::sync::mpsc;
use std::sync::mpsc::Receiver;
use std::sync::mpsc::TryRecvError;
use std::{thread, time};


fn sleep(millis: u64) {
    let duration = time::Duration::from_millis(millis);
    thread::sleep(duration);
}

fn main() {
    let processed_input_queue: VecDeque<char> = VecDeque::new();
    println!("Hello, world!");
    let the_thread = thread::spawn(move || {
        let reader = io::stdin();
        let mut buffer = [0; 10];
        loop {

            let byte = reader.read(&mut buffer);
            if byte.is_ok()
            {
                processed_input_queue.push_back(buffer);
            }
            else {
                sleep(50);
            }
        }
    });

    loop {
        let n = processed_input_queue.pop_front();
        if (n == None)
        {
            sleep(2000);
        }
        println!(n);
    }
    


    
}
