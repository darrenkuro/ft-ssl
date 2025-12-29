<h1 align="center">ft-ssl</h1>

<p align="center">
    <img src="https://img.shields.io/badge/license-MIT-blue?style=flat-square&logo=opensourceinitiative&logoColor=white" alt="License"/>
    <img src="https://img.shields.io/badge/status-development-brightgreen?style=flat-square&logo=git&logoColor=white" alt="Status">
    <!-- <img src="https://img.shields.io/badge/score-125%2F100-3CB371?style=flat-square&logo=42&logoColor=white" alt="Score"/> -->
    <!-- <img src="https://img.shields.io/badge/date-May%2026,%202023-ff6984?style=flat-square&logo=Cachet&logoColor=white" alt="Date"/> -->
</p>

> A short, clear one-liner describing what your project does.

---

## 🚀 Overview

## 🧰 Tech Stack: ![C](https://img.shields.io/badge/-C-A8B9CC?style=flat-square&logo=C&logoColor=black) ![Make](https://img.shields.io/badge/-Make-000000?style=flat-square&logo=gnu&logoColor=white)

## 📦 Features

- ***

## 🛠️ Configuration

### Prerequisites

- Compiler for C (e.g. `gcc` or `clang`) and `make`

### Installation & Usage

```bash

```

### Examples & Demo

```bash

```

### Development

```bash

```

---

## 📝 Notes & Lessons

- This combines 3 projects of cryptographic practices, each representing a different layer. A. `Integrity` ("has data changd"), incorporates hashing functions, such as MD5, SHA1, SHA256, it is one-way, not reversible. B. Confidentiality ("who can read this data"), uses symmetric encryption, such as DES, AES, shared key for encryption and decryption. C. Authentication ("who am i talking to and how do we share secrets"), utilizes asymmetric (public-key) cryptography; it uses the public key to encrypts and private key to decrypt.
- Hashing function is deterministic, going from arbitary length to fixed-length. Hash map uses something simple, for instance, `h = h * 33 + c for each c`, a map would have dynamically decided amount of buckets, for on average each bucket has 1 item. Collision isn't not a concern since generally it doesn't worry about secuirty. Cryptographic hashing, however, would be much more complex, slower, and be extremely unlikely to have collisions.
- This program has to be streaming, meaning digest and hash as you read data, and not wait for the input to finish coming in, because length is arbitary and holding them in memory is horrible design!
- Flags as bitmask (just to practice bit operations). Setting flag `|=`, checking `&`, clearing `&= ~`, toggling `^=`.
- Endianness!

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).

---

## 📫 Contact

Darren Kuro – [darren0xa@gmail.com](mailto:darren0xa@gmail.com)

GitHub: [@darrenkuro](https://github.com/darrenkuro)
