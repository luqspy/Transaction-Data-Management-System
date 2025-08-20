# 💳 Transaction Data Management System

A high-performance **C++ application** for managing, analyzing, and processing financial transaction data.  
This project implements and compares two core data structures (**Linked Lists** and **Dynamic Arrays**) to demonstrate their performance characteristics in real-world scenarios like sorting, searching, and data export.

---

## 📖 Table of Contents

- [Features](#-features)  
- [Data Structures](#-data-structures)  
- [Installation & Compilation](#️-installation--compilation)  
- [Usage](#-usage)  
- [CSV Format](#-csv-format)  
- [Performance Comparison](#-performance-comparison)  
- [Project Structure](#️-project-structure-code-overview)  
- [Technical Details](#-technical-details)  
- [Future Enhancements](#-future-enhancements)  
- [License](#-license)  

---

## ✨ Features

- **Dual Data Structure Implementation**: Choose between a Linked List or Dynamic Array backend.  
- **CSV Data Import**: Load and parse large transaction datasets, automatically categorized by payment channel (ACH, Card, UPI, Wire Transfer).  
- **Advanced Search Operations**:
  - Linear Search: by transaction type, location, or fraud status.  
  - Binary Search: by transaction type or location (requires sorting first).  
- **Efficient Sorting**: Implemented Merge Sort algorithm to sort transactions by location.  
- **Data Export**: Full dataset export to JSON format for interoperability with other tools.  
- **Performance Analytics**: Built-in timing and memory usage reporting to compare the efficiency of different operations and data structures.  

---

## 🧱 Data Structures

### 🔗 Linked List Implementation (`TransactionList` Class)
- **Pros**: Dynamic growth, efficient O(1) insertions, no memory wasted on unused capacity.  
- **Cons**: Slower traversal and search times due to poor cache locality.  
- **Best For**: Scenarios where data is constantly being added or removed.  

### 📦 Dynamic Array Implementation (`ArrayTransactionList` Class)
- **Pros**: Excellent cache locality for faster iteration, O(1) random access, more efficient for binary search.  
- **Cons**: Requires occasional costly resizing operations.  
- **Best For**: Scenarios where data is loaded once and frequently searched or sorted.  

---

## 🛠️ Installation & Compilation

### Prerequisites
- A modern **C++ compiler** (e.g., `g++` or `clang++`) supporting **C++11 or later**.

### Compilation
```bash
# Clone the repo (if applicable)
git clone <your-repo-url>
cd <repository-directory>

# Compile the program
g++ -std=c++11 -o transaction_manager project3.cpp

# Run the program
./transaction_manager
```
## 🚀 Usage

1. **Run the Program**  
   Execute the compiled binary.

2. **Choose Implementation**  
   Select either:
   - `1` → Linked List  
   - `2` → Array  

3. **Provide Data File**  
   Enter the full path to your CSV data file when prompted.

4. **Navigate the Menu**  
   Use the main menu to select a payment channel:  
   - ACH  
   - Card  
   - UPI  
   - Wire  

5. **Perform Operations**  
   Within each channel’s sub-menu, you can:  
   - 📄 Display the first 10 or all transactions.  
   - 🔍 Perform **linear** or **binary** searches on various fields.  
   - 📊 Sort the transactions by **location** using **merge sort**.  
   - 📤 Export the entire dataset to a **JSON file**.

## 📋 CSV Format

The application expects a CSV file with a **header row** and the following columns in order:
transaction_id, timestamp, sender_account, receiver_account, amount, transaction_type, merchant_category, location, device_used, is_fraud, fraud_type, time_since_last_transaction, spending_deviation_score, velocity_score, geo_anomaly_score, payment_channel, ip_address, device_hash


### 📝 Example Row
T1001,2023-01-01 12:30:00,ACC123,ACC456,150.75,purchase,retail,New York,Mobile,No,,3600.5,1.2,45,0.1,card,192.168.1.1,abc123def

---

## ⚡ Performance Comparison

The application provides **runtime analysis** for key operations:

- **Data Loading**: Arrays often load faster due to better memory allocation patterns.  
- **Sorting**: Merge sort performance is **O(n log n)** for both, but arrays typically execute faster due to cache efficiency.  
- **Searching**:  
  - Linear Search → **O(n)** for both, with arrays having a slight edge.  
  - Binary Search → **O(log n)**, but significantly faster on arrays since linked lists require conversion to an array first, negating the benefit.  

### ✅ Recommendation
- Use the **Array implementation** for **data analysis and exploration**.  
- Use the **Linked List implementation** if simulating a **live system** with continuous transaction ingestion.

## 🗂️ Project Structure (Code Overview)

The **`project3.cpp`** file is organized into the following logical sections:

```cpp
// 1. Linked List Implementation
struct Transaction { ... };         // Node structure
class TransactionList { ... };      // Linked list class with sort, search, export

// 2. Array Implementation
struct ArrayTransaction { ... };    // Data holder
class ArrayTransactionList { ... }; // Dynamic array class with sort, search, export

// 3. Helper Functions
bool importCSV(...);                // CSV parser for Linked Lists
bool importCSVArray(...);           // CSV parser for Arrays
void compareEfficiency();           // Performance summary

// 4. Menu Functions
void linkedListChannelMenu(...);
void arrayChannelMenu(...);

// 5. Main Function
int main() { ... }                  // Program entry point, drives the UI.

## ⚙️ Technical Details

### ⏱️ Time Complexity
- **Insertion**: O(1) (LinkedList at head, Array amortized)  
- **Sorting**: O(n log n) (Merge Sort)  
- **Linear Search**: O(n)  
- **Binary Search**: O(log n) (after sorting)  

### 💾 Space Complexity
- O(n) for both implementations.  

### 📚 Libraries Used
- `<iostream>`  
- `<fstream>`  
- `<sstream>`  
- `<algorithm>`  
- `<chrono>`  
- `<iomanip>`  

---

## 🔮 Future Enhancements

- **Database Backend**: Replace CSV import with SQL database connectivity.  
- **Multithreading**: Parallelize sorting and searching algorithms for large datasets.  
- **Graphical User Interface (GUI)**: Develop a Qt or web-based frontend.  
- **Network Server**: Run as a service that accepts data via sockets or HTTP.  
- **Advanced Analytics**: Integrate machine learning models for real-time fraud prediction.  
- **Unit Tests**: Implement a testing framework (e.g., Google Test) for reliability.  

---

## 📜 License

This project is available for **educational purposes**.


