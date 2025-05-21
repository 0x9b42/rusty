# rusty
## learning rust in 2025

Rust is a systems programming language focused on safety, performance, and concurrency. Its unique ownership model and strict compiler checks make it powerful but also require adherence to specific best practices to write idiomatic, efficient, and maintainable code. Below is a comprehensive guide to Rust programming best practices, organized by key areas. This assumes you have a basic understanding of Rust; if you need clarification on any concept, let me know!

---

## **1. Code Organization and Structure**
- **Use Modules to Organize Code**
  - Split code into logical modules using `mod` and `pub` keywords to control visibility.
  - Place modules in separate files or directories (e.g., `src/module.rs` or `src/module/mod.rs`).
  - Use `use` statements to bring items into scope, preferring explicit imports (e.g., `use std::collections::HashMap`) over glob imports (`use std::collections::*`).
  - Example:
```rust
// src/main.rs
mod utils;

use utils::math::add;
fn main() {
    println!("{}", add(2, 3));
}

// src/utils/math.rs
pub fn add(a: i32, b: i32) -> i32 {
    a + b
}
```

- **Follow Standard Project Layout**
  - Use `Cargo.toml` for dependencies and project metadata.
  - Place main code in `src/main.rs` for binaries or `src/lib.rs` for libraries.
  - Store tests in `tests/` or within `#[cfg(test)]` modules.
  - Use `examples/` for example code and `benches/` for benchmarks.
  - Example `Cargo.toml`:
```toml
[package]
name = "my_project"
version = "0.1.0"
edition = "2021"

[dependencies]
serde = { version = "1.0", features = ["derive"] }
```

- **Leverage Cargo Workspaces for Large Projects**
  - Group related crates in a workspace to share dependencies and simplify builds.
  - Example:
```toml
[workspace]
members = ["crate1", "crate2"]
```

- **Use Descriptive Names**
  - Follow Rust naming conventions: `snake_case` for functions and variables, `CamelCase` for types and traits.
  - Name functions and variables clearly to reflect their purpose (e.g., `calculate_total_price` instead of `calc`).

---

## **2. Memory Management and Ownership**
- **Embrace Ownership and Borrowing**
  - Understand Rust’s ownership model: each value has a single owner, and it’s deallocated when the owner goes out of scope.
  - Use references (`&` for shared, `&mut` for mutable) to avoid unnecessary cloning.
  - Example:
```rust
fn print_length(s: &str) {
    println!("Length: {}", s.len());
}
```

- **Minimize Use of `Clone`**
  - Avoid `Clone` unless necessary, as it can hurt performance. Prefer references or restructuring code to avoid ownership issues.
  - If cloning is needed, use it explicitly to make costs clear.
  - Example:
```rust
// Instead of cloning
let s = String::from("hello");
let s2 = s.clone();
// Try borrowing
fn use_string(s: &String) { /* ... */ }
```

- **Use `Copy` Types for Simple Data**
  - For small, immutable types like `i32` or `bool`, leverage `Copy` to avoid borrowing complexity.
  - Implement `Copy` and `Clone` for custom types when appropriate:
```rust
#[derive(Copy, Clone)]
struct Point {
    x: i32,
    y: i32,
}
```

- **Avoid Unnecessary `unsafe` Code**
  - Use `unsafe` only when absolutely necessary (e.g., FFI, low-level optimizations).
  - Encapsulate `unsafe` code in safe abstractions with clear documentation.
  - Example:
```rust
pub fn safe_abstraction() -> i32 {
    unsafe {
        // Low-level operation
        some_unsafe_function()
    }
}
```

---

## **3. Error Handling**
- **Use `Result` and `Option` for Robustness**
  - Return `Result<T, E>` for operations that can fail and `Option<T>` for values that may be absent.
  - Avoid panics (`unwrap`, `expect`) in production code; use `match` or combinators like `map`, `and_then`, or `?`.
  - Example:
```rust
fn parse_number(s: &str) -> Result<i32, std::num::ParseIntError> {
    s.parse()
}

fn main() -> Result<(), std::num::ParseIntError> {
    let num = parse_number("42")?;
    println!("Number: {}", num);
    Ok(())
}
```

- **Create Custom Error Types**
  - Define custom error enums for complex applications, using `thiserror` for convenience.
  - Example:
```rust
use thiserror::Error;

#[derive(Error, Debug)]
enum MyError {
    #[error("IO error: {0}")]
    Io(#[from] std::io::Error),
    #[error("Invalid input")]
    InvalidInput,
}
```

- **Handle Errors Gracefully**
  - Use `match` for explicit control flow or combinators like `unwrap_or`, `map_err`, or `or_else` for concise error handling.
  - Provide meaningful error messages for users.

---

## **4. Concurrency**
- **Use Rust’s Concurrency Primitives**
  - Prefer `std::thread` for simple parallelism, `Arc` (Atomic Reference Counting) for shared ownership, and `Mutex` or `RwLock` for safe mutation.
  - Example:
```rust
use std::sync::{Arc, Mutex};
use std::thread;

let counter = Arc::new(Mutex::new(0));
let mut handles = vec![];

for _ in 0..10 {
    let counter = Arc::clone(&counter);
    let handle = thread::spawn(move || {
        let mut num = counter.lock().unwrap();
        *num += 1;
    });
    handles.push(handle);
}

for handle in handles {
    handle.join().unwrap();
}
```

- **Leverage Async/Await for I/O-Bound Tasks**
  - Use `async`/`await` with libraries like `tokio` or `async-std` for concurrent I/O operations.
  - Avoid blocking calls in `async` code (e.g., use `tokio::fs` instead of `std::fs`).
  - Example:
```rust
use tokio;

#[tokio::main]
async fn main() {
    let result = async_operation().await;
    println!("Result: {}", result);
}

async fn async_operation() -> String {
    "Hello, async!".to_string()
}
```

- **Avoid Shared Mutable State When Possible**
  - Use message passing (e.g., `std::sync::mpsc`) or immutable data to reduce concurrency bugs.
  - Example:
```rust
use std::sync::mpsc;
use std::thread;

let (tx, rx) = mpsc::channel();
thread::spawn(move || {
    tx.send("Hello").unwrap();
});
println!("Received: {}", rx.recv().unwrap());
```

---

## **5. Performance Optimization**
- **Use Iterators for Efficient Loops**
  - Leverage Rust’s iterator methods (`map`, `filter`, `fold`) for concise and optimized code.
  - Example:
```rust
let sum: i32 = vec![1, 2, 3, 4]
    .into_iter()
    .filter(|&x| x % 2 == 0)
    .sum();
```

- **Minimize Heap Allocations**
  - Use stack-allocated types like arrays or `&str` when possible.
  - Prefer `Vec` over `VecDeque` unless you need double-ended operations.
  - Reuse buffers with `Vec::clear` instead of creating new ones.

- **Profile and Benchmark**
  - Use `cargo bench` or tools like `criterion` to measure performance.
  - Profile with tools like `perf` or `flamegraph` to identify bottlenecks.
  - Example `Cargo.toml` for benchmarking:
```toml
[dev-dependencies]
criterion = "0.5"

[[bench]]
name = "my_benchmark"
harness = false
```

- **Use `const` and `static` Appropriately**
  - Use `const` for compile-time constants and `static` for global variables with fixed memory locations.
  - Example:
```rust
const MAX_SIZE: usize = 1024;
static VERSION: &str = "1.0.0";
```

---

## **6. Testing**
- **Write Unit and Integration Tests**
  - Place unit tests in `#[cfg(test)]` modules within the same file.
  - Write integration tests in the `tests/` directory.
  - Example:
```rust
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_add() {
        assert_eq!(add(2, 3), 5);
    }
}
```

- **Use `assert!` and `assert_eq!`**
  - Use `assert!` for boolean conditions and `assert_eq!` for equality checks.
  - Provide descriptive messages for failures:
```rust
#[test]
fn test_division() {
    let result = 10 / 2;
    assert_eq!(result, 5, "Division result incorrect");
}
```

- **Test Error Cases**
  - Test both success and failure paths, especially for `Result` and `Option`.
  - Example:
```rust
#[test]
fn test_parse_number() {
    assert!(parse_number("42").is_ok());
    assert!(parse_number("invalid").is_err());
}
```

---

## **7. Documentation and Comments**
- **Write Clear Documentation**
  - Use `///` for public API documentation and `//` for internal comments.
  - Follow Rustdoc conventions, including examples and `#[derive]` attributes.
  - Example:
```rust
/// Adds two numbers.
///
/// # Examples
///
/// let result = add(2, 3);
/// assert_eq!(result, 5);
/// 
pub fn add(a: i32, b: i32) -> i32 {
    a + b
}
```

- **Generate Documentation**
  - Run `cargo doc --open` to build and view documentation locally.
  - Include a `README.md` with project overview, installation, and usage instructions.

- **Keep Comments Up-to-Date**
  - Avoid outdated comments by updating them during code changes.
  - Use comments to explain *why* rather than *what* for complex logic.

---

## **8. Dependency Management**
- **Minimize Dependencies**
  - Only include necessary dependencies to reduce build times and security risks.
  - Use `cargo audit` to check for vulnerabilities in dependencies.

- **Pin Dependency Versions**
  - Specify exact versions or narrow ranges in `Cargo.toml` to avoid breaking changes.
  - Example:
```toml
[dependencies]
serde = "=1.0.203"
```

- **Use Features to Reduce Bloat**
  - Enable only required features for dependencies to minimize binary size.
  - Example:
```toml
[dependencies]
serde = { version = "1.0", features = ["derive"] }
```

---

## **9. Idiomatic Rust**
- **Use Pattern Matching**
  - Leverage `match` and `if let` for control flow to handle `Option`, `Result`, and enums.
  - Example:
```rust
let opt = Some(42);
if let Some(value) = opt {
    println!("Value: {}", value);
}
```

- **Leverage Traits for Abstraction**
  - Use traits to define shared behavior and enable generic programming.
  - Example:
```rust
trait Printable {
    fn print(&self);
}

struct MyStruct(i32);
impl Printable for MyStruct {
    fn print(&self) {
        println!("Value: {}", self.0);
    }
}
```

- **Avoid Overusing `unsafe` or `panic!`**
  - Reserve `panic!` for unrecoverable errors and prefer `Result` for recoverable ones.
  - Limit `unsafe` to specific, well-documented cases.

---

## **10. Community and Tools**
- **Follow Clippy Lints**
  - Run `cargo clippy` to catch common mistakes and enforce idiomatic Rust.
  - Example command:
```bash
cargo clippy -- -W clippy::pedantic
```

- **Format Code with `rustfmt`**
  - Use `cargo fmt` to enforce consistent code style across your project.
  - Configure `rustfmt` via a `rustfmt.toml` file if needed.

- **Stay Updated with Rust Editions**
  - Use the latest Rust edition (e.g., 2021 as of now) for new features and improvements.
  - Specify the edition in `Cargo.toml`:
```toml
[package]
edition = "2021"
```

- **Engage with the Community**
  - Follow Rust’s official blog, `This Week in Rust`, and forums like `users.rust-lang.org`.
  - Contribute to open-source Rust projects to learn best practices.

---

## **11. Security Best Practices**
- **Sanitize Inputs**
  - Validate and sanitize all user inputs to prevent injection attacks or undefined behavior.
  - Use libraries like `validator` for input validation.

- **Avoid Unsafe String Operations**
  - Use safe string handling (e.g., `String` and `&str`) and avoid manual `unsafe` string manipulation.
  - Example:
```rust
let safe_string = String::from("hello");
```

- **Use Safe Libraries**
  - Prefer well-maintained libraries like `serde`, `tokio`, or `reqwest` for critical functionality.
  - Regularly update dependencies with `cargo update`.

---

## **12. Example: Putting It All Together**
Here’s a small, idiomatic Rust program demonstrating several best practices:

```rust
// src/main.rs
use std::error::Error;
use thiserror::Error;

mod calculator {
    /// Calculates the sum of two numbers.
    pub fn add(a: i32, b: i32) -> i32 {
        a + b
    }

    /// Divides two numbers, returning an error on division by zero.
    pub fn divide(a: i32, b: i32) -> Result<i32, CalculatorError> {
        if b == 0 {
            return Err(CalculatorError::DivisionByZero);
        }
        Ok(a / b)
    }
}

#[derive(Error, Debug)]
pub enum CalculatorError {
    #[error("division by zero")]
    DivisionByZero,
}

fn main() -> Result<(), Box<dyn Error>> {
    let sum = calculator::add(5, 3);
    println!("Sum: {}", sum);

    match calculator::divide(10, 2) {
        Ok(result) => println!("Division result: {}", result),
        Err(e) => eprintln!("Error: {}", e),
    }

    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_add() {
        assert_eq!(calculator::add(2, 3), 5);
    }

    #[test]
    fn test_divide() {
        assert_eq!(calculator::divide(10, 2), Ok(5));
        assert!(calculator::divide(10, 0).is_err());
    }
}
```

---

## **Additional Resources**
- **Official Rust Book**: https://doc.rust-lang.org/book/
- **Rust by Example**: https://doc.rust-lang.org/rust-by-example/
- **Rust API Guidelines**: https://rust-lang.github.io/api-guidelines/
- **Clippy Documentation**: https://doc.rust-lang.org/clippy/
- **Rust Community**: Join `r/rust` on Reddit or the Rust Discord for support.

