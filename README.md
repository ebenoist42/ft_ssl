# 🔐 ft_ssl

*A cryptographic hashing tool written in C — 42 School Project.*

## 📖 About

**ft_ssl** is a partial reimplementation of the OpenSSL command-line utility, developed as part of the **42 School curriculum**.

The goal of this project is to understand how cryptographic hash functions work by implementing two widely known algorithms from scratch: **MD5** and **SHA-256**.

Through low-level programming in C, this project explores data processing, bitwise operations, binary manipulation, and the fundamental principles of cryptographic hashing.

## ⚙️ Features

- MD5 hashing algorithm
- SHA-256 hashing algorithm
- Hashing strings and files
- Standard input (STDIN) support
- Standard output (STDOUT) handling
- Multiple input processing
- Command-line flag management
- Error handling for invalid commands and files
- Command dispatching using function pointers
- Implementation written in C

## 🔑 Supported Algorithms

### MD5 — Message Digest 5

MD5 generates a **128-bit (32 hexadecimal characters)** hash value from an input.

Although MD5 is no longer considered cryptographically secure due to collision vulnerabilities, it remains useful for understanding the fundamentals of hashing algorithms.

### SHA-256 — Secure Hash Algorithm 256

SHA-256 belongs to the SHA-2 family and generates a **256-bit (64 hexadecimal characters)** hash value.

It offers significantly stronger collision resistance than MD5 and is widely used in security applications, integrity verification, and cryptographic systems.

## 🛠️ Compilation

Clone the repository and compile the project:

```bash
git clone https://github.com/<username>/ft_ssl.git
cd ft_ssl
make
```

This generates the executable `ft_ssl`.

## 🚀 Usage

The general command syntax is:

```bash
./ft_ssl command [flags] [file/string]
```

### MD5 Hashing

Hash a string:

```bash
./ft_ssl md5 -s "42 is nice"
```

Output:

```text
MD5 ("42 is nice") = 9cfbcb22a81b6ed3ba5b99c2e6fbf3e0
```

Hash a file:

```bash
./ft_ssl md5 file.txt
```

Hash standard input:

```bash
echo "Hello World" | ./ft_ssl md5
```

### SHA-256 Hashing

Hash a string:

```bash
./ft_ssl sha256 -s "42 is nice"
```

Output:

```text
SHA256 ("42 is nice") = b7e44c7a40c5f80139f0a50f3650fb2bd8d00b0d24667c4c2ca32c88e13b758f
```

Hash a file:

```bash
./ft_ssl sha256 file.txt
```

Hash standard input:

```bash
echo "Hello World" | ./ft_ssl sha256
```

## 🚩 Available Flags

Both MD5 and SHA-256 support the following flags:

| Flag | Description |
|------|-------------|
| `-p` | Echo STDIN to STDOUT and append the checksum |
| `-q` | Quiet mode: display only the hash |
| `-r` | Reverse the output format |
| `-s` | Hash the specified string |

### Examples

Quiet mode:

```bash
./ft_ssl md5 -q -s "Hello"
```

Reverse output format:

```bash
./ft_ssl sha256 -r file.txt
```

Process STDIN:

```bash
echo "42 School" | ./ft_ssl md5 -p
```

Combine multiple flags and inputs:

```bash
echo "Hello" | ./ft_ssl md5 -p -s "42" file.txt
```

## 🧠 How It Works

Cryptographic hash functions transform arbitrary-length input data into a fixed-length output called a **digest**.

### MD5 Algorithm

1. Pad the input message to the required block length.
2. Append the original message length.
3. Initialize four 32-bit state variables.
4. Process each 512-bit block through 64 operations.
5. Combine the resulting state to produce a 128-bit digest.

### SHA-256 Algorithm

1. Pad the input message and append its original length.
2. Divide the message into 512-bit blocks.
3. Initialize eight 32-bit state variables.
4. Expand the message schedule for each block.
5. Execute 64 rounds of compression using bitwise operations.
6. Combine the final state into a 256-bit digest.

Both algorithms use operations such as bitwise AND, OR, XOR, shifts, rotations, and modular addition.

## 📋 Project Requirements

- **Language:** C
- **Executable:** `ft_ssl`
- **Compilation:** Makefile
- **Mandatory algorithms:** MD5 and SHA-256
- **Mandatory flags:** `-p`, `-q`, `-r`, `-s`
- **Input sources:** STDIN, strings, and files
- **Output:** STDOUT
- **Architecture:** Clean and maintainable code with function-pointer-based command dispatching
- **Error handling:** Invalid commands, missing files, and incorrect arguments

## 🎓 Learning Objectives

Through this project, the main concepts explored are:

- Cryptographic hashing fundamentals
- MD5 and SHA-256 algorithm implementation
- Bitwise operations and binary data manipulation
- Memory management in C
- File descriptors and input/output streams
- Command-line argument parsing
- Function pointers and command dispatching
- Data padding and block processing
- Error handling and edge cases

## 🔒 Security Note

**MD5 is cryptographically broken and must not be used for security-sensitive applications.**

SHA-256 remains a widely accepted cryptographic hash function, but hashing alone does not provide encryption, authentication, or secure password storage.

This project is intended for **educational purposes** and is not a replacement for production-grade cryptographic libraries.
