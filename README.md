# 🏦 Bank Account Management System in C


## Project Overview

This **Bank Account Management System** is designed to simulate bank transactions such as handling account balances and processing transfers. Built in **C**, the project involves reading and updating binary and text files to reflect bank account activities efficiently. The project demonstrates key concepts like file handling, data structures, and optimization for handling large datasets.

## Features

- 🏛 Manage CCP accounts and balances stored in binary files
- 🔄 Process transactions from a text file, applying the changes to account balances
- 🧹 Remove duplicate CCP account entries during initialization
- 🧾 Track accounts with negative balances and those exceeding 1,000,000 DZD
- 💾 Supports two file structures:
  - **TOF (Table Ordonnée Fixe)** for fixed-size records
  - **LOVC (Liste Ordonnée à Variables Chevauchantes)** for variable-size records

## Tech Stack

- **Programming Language:** C
- **File Types:**
  - Binary (`CCP_SOLDE.bin`, `CCP_SOLDE_2.bin`)
  - Text (`VIREMENTS.txt`)
- **Key Concepts:**
  - Random number generation
  - Binary and text file manipulation
  - Dynamic memory management

## Installation

To compile and run the project locally:

1. Clone the repository:

    ```bash
    git clone https://github.com/sofianchelihi/C-Project.git
    ```

2. Navigate to the project directory:

    ```bash
    cd C-Project
    ```

3. Compile the C source files using a C compiler (e.g., GCC):

    ```bash
    gcc -o bank_management bank_management.c
    ```

4. Run the compiled program:

    ```bash
    ./bank_management
    ```

## Usage

- **CCP_SOLDE.bin:** Contains CCP account numbers and their balances.
- **VIREMENTS.txt:** Contains bank transfer records to be applied to CCP accounts.
- **CCP_SOLDE_2.bin:** An advanced file format with variable record size for handling transactions more efficiently.
  
### Key Operations:
- **Create** a binary file with random CCP accounts.
- **Process** bank transfers from the `VIREMENTS.txt` file.
- **Insert**, **delete**, or **update** records in the CCP account files.
- **View** all accounts with negative balances or those exceeding 1,000,000 DZD.

## Contributing

Contributions are welcome! If you'd like to improve or extend the project, follow these steps:

1. Fork the repository.
2. Create a feature branch (`git checkout -b feature-branch`).
3. Commit your changes (`git commit -m 'Add new feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Open a Pull Request.

## License

This project is open-source and available under the [MIT License](LICENSE).

---

