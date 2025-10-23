# Steganography

## 🕵️‍♂️ Steganography in C

### 🧩 Project Overview

The **Steganography Project** is a **C-based image data hiding tool** that allows users to **encode (hide)** and **decode (extract)** secret messages within image files using the **Least Significant Bit (LSB)** technique. It demonstrates the fundamentals of **data security**, **bitwise operations**, and **file manipulation** in C programming.

### 🚀 Features

* 🔐 **Encode Message** – Hide secret text inside an image file.
* 🔎 **Decode Message** – Extract hidden information from an encoded image.
* ⚙️ **Bit-Level Manipulation** – Uses the LSB technique for secure and invisible data embedding.
* 💾 **File Handling** – Reads and writes binary image data efficiently.
* 🧠 **Error Checking** – Ensures message integrity and safe data recovery.

### 🧠 Concepts Used

* Bitwise Operations (`&`, `|`, `<<`, `>>`)
* File Handling (`fopen`, `fread`, `fwrite`)
* Binary Data Processing
* Modular Programming in C
* Data Encoding and Decoding Logic

### ⚙️ How to Run

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/steganography.git
   cd steganography
   ```
2. Compile the program:

   ```bash
   gcc steganography.c -o steganography
   ```
3. Run the executable:

   ```bash
   ./steganography
   ```

### 🧑‍💻 Example Use

```
1. Encode Message
2. Decode Message
3. Exit
Enter your choice: 1
Enter Image File: input.bmp
Enter Output File: secret.bmp
Enter Message: Hello World!
Message encoded successfully!
```

### 📂 File Structure

```
├── steganography.c
├── encode.c
├── decode.c
├── stego.h
├── input.bmp
├── secret.bmp
└── README.md
```

### 🏁 Future Enhancements

* Support for PNG and JPEG formats
* Password protection for decoding
* GUI-based tool for better user interaction
* Improved data compression for larger messages

