extern crate rand;

use std::io;
use std::io::Write;
use std::cmp::Ordering;

use crate::rand::Rng;

fn main() {
    let secret = rand::rng().random_range(1..101);

    println!("guess a number between 1-100");
    println!("you got 10 chances.");

    for _ in 0..10 {
        let mut guess = String::new();
        print!("pls guess a number: ");
        io::stdout().flush().unwrap();

        io::stdin().read_line(&mut guess)
            .expect("failed to read line");

        let guess:i32 = guess.trim().parse()
            .expect("please type a number!");

        match guess.cmp(&secret) {
            Ordering::Less => println!("too small"),
            Ordering::Greater => println!("too much"),
            Ordering::Equal => {
                println!("CONGRATS!!");
                break;
            }
        }
    }

    //println!("you guessed : {}", guess);
    //println!("secret num. : {}", secret);
}
