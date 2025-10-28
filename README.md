## 🕵️‍♂️ Steganography in C

### 🧩 Project Overview

The **Steganography Project** is a C-based application that demonstrates the concept of **data hiding within images**. It allows users to **encode (hide)** secret messages or files inside image files and later **decode (extract)** them safely. This project highlights **bit-level manipulation**, **file handling**, and **data encoding techniques** in C — a practical implementation of information security and digital watermarking principles.

---

### 🚀 Features

* 🔐 **Encode** secret text data inside a `.bmp` image file
* 🧩 **Decode** and retrieve hidden data from a stego image
* 💾 Supports custom message size and flexible file handling
* 🧠 Demonstrates **bitwise operations**, **LSB (Least Significant Bit)** encoding technique
* 💬 Simple command-line interface (CLI) for user interaction

---

### ⚙️ Technologies Used

* **Language:** C
* **Concepts:** File handling, bitwise operations, data masking
* **Platform:** Linux / Windows (GCC Compiler)

---

### 📚 Learning Outcomes

* Understanding **steganography principles**
* Practical use of **bit-level programming in C**
* Handling **binary and image data structures**
* Developing a **CLI-based security tool**

---

### 🧪 Example Usage

```bash
# Encode a message
./stego -e input.bmp secret.txt output.bmp

# Decode hidden message
./stego -d output.bmp decoded.txt
```

---

### 🧰 Project Structure

```
├── encode.c
├── decode.c
├── stego.h
├── main.c
├── Makefile
└── README.md
```

---

### 📄 Future Enhancements

* Add support for **other image formats (e.g., PNG, JPEG)**
* Implement **password protection** for added security
* Include a **GUI version** for ease of use
