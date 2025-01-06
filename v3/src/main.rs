
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
use std::sync::{Arc, Mutex};


fn sleep(millis: u64) {
    let duration = time::Duration::from_millis(millis);
    thread::sleep(duration);
}

fn main() {
    let workload = Arc::new(Mutex::new(VecDeque::new()));
    workload.lock().unwrap().push_back(0);

    let thread_1_queue = workload.clone();
    println!("Hello, world!");
    let the_thread = thread::spawn(move || {
        let mut reader = io::stdin();

        loop {
            let mut buffer = [0; 10];
            let byte = reader.read(&mut buffer);
            if byte.is_ok()
            {
                buffer.iter().for_each(|&c| {
                    if c == 0 {
                        //thread_1_queue.lock().unwrap().push_back(c);
                    }
                    else {
                        thread_1_queue.lock().unwrap().push_back(c);
                    }

                });
            }
            else {
                sleep(50);
            }
        }
    });
    let mut newline = false;
    loop {
        let n = workload.lock().unwrap().pop_front();
        if (n == None)
        {
            if (newline)
            {
                println!("");
                newline = false;
            }
            sleep(20);
        }
        else
        {
            print!("{}", n.unwrap() as char);
            newline = true;
        }
    }
    


    
}
