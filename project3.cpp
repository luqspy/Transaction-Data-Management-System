#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <chrono>

using namespace std;

// Linked List Implementation
struct Transaction {
    string transaction_id, timestamp, sender_account, receiver_account;
    double amount;
    string transaction_type, merchant_category, location, device_used, is_fraud, fraud_type;
    double time_since_last_transaction, spending_deviation_score, geo_anomaly_score;
    int velocity_score;
    string payment_channel, ip_address, device_hash;
    Transaction* next;

    Transaction() : next(nullptr) {}
};

class TransactionList {
private:
    Transaction* head;
    int count;

    Transaction* split(Transaction* head) {
        Transaction* fast = head;
        Transaction* slow = head;
        Transaction* prev = nullptr;
        
        while (fast && fast->next) {
            prev = slow;
            slow = slow->next;
            fast = fast->next->next;
        }
        
        if (prev) {
            prev->next = nullptr;
        }
        
        return slow;
    }

    Transaction* merge(Transaction* left, Transaction* right) {
        Transaction dummy;
        Transaction* tail = &dummy;
        dummy.next = nullptr;

        while (left && right) {
            if (left->location < right->location) {
                tail->next = left;
                left = left->next;
            } else {
                tail->next = right;
                right = right->next;
            }
            tail = tail->next;
        }

        tail->next = (left) ? left : right;
        return dummy.next;
    }

    Transaction* mergeSort(Transaction* head) {
        if (!head || !head->next) {
            return head;
        }
        
        Transaction* middle = split(head);
        Transaction* left = mergeSort(head);
        Transaction* right = mergeSort(middle);
        
        return merge(left, right);
    }

public:
    TransactionList() : head(nullptr), count(0) {}

    ~TransactionList() {
        while (head) {
            Transaction* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void addTransaction(Transaction* newTrans) {
        newTrans->next = head;
        head = newTrans;
        count++;
    }

    void displayTransactions(int limit) {
        if (head == nullptr) {
            cout << "No transactions to display." << endl;
            return;
        }

        Transaction* current = head;
        int displayed = 0;
        while (current && (limit == -1 || displayed < limit)) {
            cout << "===== Transaction " << (displayed + 1) << " =====" << endl;
            cout << "ID: " << current->transaction_id << endl;
            cout << "Timestamp: " << current->timestamp << endl;
            cout << "Sender: " << current->sender_account << endl;
            cout << "Receiver: " << current->receiver_account << endl;
            cout << "Amount: " << current->amount << endl;
            cout << "Type: " << current->transaction_type << endl;
            cout << "Merchant: " << current->merchant_category << endl;
            cout << "Location: " << current->location << endl;
            cout << "Device: " << current->device_used << endl;
            cout << "Is Fraud: " << current->is_fraud << endl;
            cout << "Fraud Type: " << current->fraud_type << endl;
            cout << "Time Since Last: " << current->time_since_last_transaction << endl;
            cout << "Spending Deviation: " << current->spending_deviation_score << endl;
            cout << "Velocity: " << current->velocity_score << endl;
            cout << "Geo Anomaly: " << current->geo_anomaly_score << endl;
            cout << "Payment Channel: " << current->payment_channel << endl;
            cout << "IP: " << current->ip_address << endl;
            cout << "Device Hash: " << current->device_hash << endl << endl;

            current = current->next;
            displayed++;
        }

        if (limit != -1 && displayed < count) {
            cout << "Displaying " << displayed << " of " << count << " transactions." << endl;
        }
    }

    void sortByLocation() {
        if (head == nullptr || head->next == nullptr) {
            cout << "No transactions to sort or already sorted." << endl;
            return;
        }

        cout << "Sorting transactions by location (merge sort)..." << endl;
        auto start = chrono::high_resolution_clock::now();

        head = mergeSort(head);

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        cout << "Successfully sorted " << count << " transactions by location in " 
             << elapsed.count() << " seconds." << endl;
    }

    void searchByTransactionType(const string& type) {
        if (head == nullptr) {
            cout << "No transactions to search." << endl;
            return;
        }

        Transaction* current = head;
        int found = 0;
        while (current) {
            if (current->transaction_type == type) {
                cout << "===== Match " << (found + 1) << " =====" << endl;
                cout << "ID: " << current->transaction_id << endl;
                cout << "Date: " << current->timestamp << endl;
                cout << "Amount: " << current->amount << endl;
                cout << "Location: " << current->location << endl;
                cout << "Payment Channel: " << current->payment_channel << endl;
                found++;
            }
            current = current->next;
        }

        if (found == 0) {
            cout << "No transactions found with type: " << type << endl;
        } else {
            cout << "Found " << found << " matching transactions." << endl;
        }
    }

    void linearSearch(const string& query, const string& field) {
        if (head == nullptr) {
            cout << "No transactions to search." << endl;
            return;
        }

        auto start = chrono::high_resolution_clock::now();
        Transaction* current = head;
        int found = 0;

        while (current) {
            if (field == "type" && current->transaction_type == query) {
                found++;
            } else if (field == "location" && current->location == query) {
                found++;
            } else if (field == "fraud" && current->is_fraud == query) {
                found++;
            }
            current = current->next;
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;

        cout << "\nLinear Search Results for " << field << " = " << query << ":" << endl;
        cout << "Found " << found << " matches in " << elapsed.count() << " seconds" << endl;
    }

    bool binarySearch(const string& query, const string& field) {
        if (count == 0) {
            cout << "No transactions to search." << endl;
            return false;
        }

        // Convert linked list to array for binary search
        Transaction** arr = new Transaction*[count];
        Transaction* current = head;
        for (int i = 0; i < count; i++) {
            arr[i] = current;
            current = current->next;
        }

        // Sort the array based on the search field
        if (field == "type") {
            sort(arr, arr + count, [](Transaction* a, Transaction* b) {
                return a->transaction_type < b->transaction_type;
            });
        } else if (field == "location") {
            sort(arr, arr + count, [](Transaction* a, Transaction* b) {
                return a->location < b->location;
            });
        }

        auto start = chrono::high_resolution_clock::now();

        // Perform binary search
        int left = 0, right = count - 1;
        int found = 0;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (field == "type" && arr[mid]->transaction_type == query) {
                // Count all matching occurrences (since there may be duplicates)
                found = 1;
                // Check left side
                int left_ptr = mid - 1;
                while (left_ptr >= 0 && arr[left_ptr]->transaction_type == query) {
                    found++;
                    left_ptr--;
                }
                // Check right side
                int right_ptr = mid + 1;
                while (right_ptr < count && arr[right_ptr]->transaction_type == query) {
                    found++;
                    right_ptr++;
                }
                break;
            } else if (field == "location" && arr[mid]->location == query) {
                found = 1;
                int left_ptr = mid - 1;
                while (left_ptr >= 0 && arr[left_ptr]->location == query) {
                    found++;
                    left_ptr--;
                }
                int right_ptr = mid + 1;
                while (right_ptr < count && arr[right_ptr]->location == query) {
                    found++;
                    right_ptr++;
                }
                break;
            }
            
            string fieldValue;
            if (field == "type") {
                fieldValue = arr[mid]->transaction_type;
            } else if (field == "location") {
                fieldValue = arr[mid]->location;
            }
            
            if (fieldValue < query) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;

        cout << "\nBinary Search Results for " << field << " = " << query << ":" << endl;
        if (found > 0) {
            cout << "Found " << found << " matches in " << elapsed.count() << " seconds" << endl;
        } else {
            cout << "No matches found in " << elapsed.count() << " seconds" << endl;
        }

        delete[] arr;
        return found > 0;
    }

    void exportToJSON(const string& filename) {
        ofstream outFile(filename);
        if (!outFile.is_open()) {
            cout << "Failed to open file for writing: " << filename << endl;
            return;
        }

        outFile << "[\n";
        Transaction* current = head;
        bool first = true;

        while (current) {
            if (!first) {
                outFile << ",\n";
            }
            first = false;

            outFile << "  {\n";
            outFile << "    \"transaction_id\": \"" << current->transaction_id << "\",\n";
            outFile << "    \"timestamp\": \"" << current->timestamp << "\",\n";
            outFile << "    \"sender_account\": \"" << current->sender_account << "\",\n";
            outFile << "    \"receiver_account\": \"" << current->receiver_account << "\",\n";
            outFile << "    \"amount\": " << current->amount << ",\n";
            outFile << "    \"transaction_type\": \"" << current->transaction_type << "\",\n";
            outFile << "    \"merchant_category\": \"" << current->merchant_category << "\",\n";
            outFile << "    \"location\": \"" << current->location << "\",\n";
            outFile << "    \"device_used\": \"" << current->device_used << "\",\n";
            outFile << "    \"is_fraud\": \"" << current->is_fraud << "\",\n";
            outFile << "    \"fraud_type\": \"" << current->fraud_type << "\",\n";
            outFile << "    \"time_since_last_transaction\": " << current->time_since_last_transaction << ",\n";
            outFile << "    \"spending_deviation_score\": " << current->spending_deviation_score << ",\n";
            outFile << "    \"velocity_score\": " << current->velocity_score << ",\n";
            outFile << "    \"geo_anomaly_score\": " << current->geo_anomaly_score << ",\n";
            outFile << "    \"payment_channel\": \"" << current->payment_channel << "\",\n";
            outFile << "    \"ip_address\": \"" << current->ip_address << "\",\n";
            outFile << "    \"device_hash\": \"" << current->device_hash << "\"\n";
            outFile << "  }";

            current = current->next;
        }

        outFile << "\n]";
        outFile.close();
        cout << "Successfully exported " << count << " transactions to " << filename << endl;
    }

    int getCount() const { return count; }
};

// Array Implementation
struct ArrayTransaction {
    string transaction_id;
    string timestamp;
    string sender_account;
    string receiver_account;
    double amount;
    string transaction_type;
    string merchant_category;
    string location;
    string device_used;
    string is_fraud;
    string fraud_type;
    double time_since_last_transaction;
    double spending_deviation_score;
    int velocity_score;
    double geo_anomaly_score;
    string payment_channel;
    string ip_address;
    string device_hash;
};

class ArrayTransactionList {
private:
    ArrayTransaction** transactions;
    int count;
    int capacity;

    void resize() {
        capacity *= 2;
        ArrayTransaction** newTransactions = new ArrayTransaction*[capacity];
        for (int i = 0; i < count; ++i) {
            newTransactions[i] = transactions[i];
        }
        delete[] transactions;
        transactions = newTransactions;
    }

    void merge(ArrayTransaction** arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        ArrayTransaction** L = new ArrayTransaction*[n1];
        ArrayTransaction** R = new ArrayTransaction*[n2];

        for (int i = 0; i < n1; i++) L[i] = arr[left + i];
        for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (L[i]->location <= R[j]->location) {
                arr[k++] = L[i++];
            } else {
                arr[k++] = R[j++];
            }
        }

        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];

        delete[] L;
        delete[] R;
    }

    void mergeSort(ArrayTransaction** arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            merge(arr, left, mid, right);
        }
    }

public:
    ArrayTransactionList(int initialCapacity = 10) : count(0), capacity(initialCapacity) {
        transactions = new ArrayTransaction*[capacity];
    }

    ~ArrayTransactionList() {
        for (int i = 0; i < count; ++i) {
            delete transactions[i];
        }
        delete[] transactions;
    }

    void addTransaction(ArrayTransaction* t) {
        if (count == capacity) {
            resize();
        }
        transactions[count++] = t;
    }

    void displayTransactions(int limit) {
        if (count == 0) {
            cout << "No transactions to display." << endl;
            return;
        }

        int displayCount = (limit == -1) ? count : min(limit, count);
        cout << "\nDisplaying " << displayCount << " of " << count << " transactions..." << endl;
        cout << "=====================================" << endl;
        for (int i = 0; i < displayCount; ++i) {
            ArrayTransaction* t = transactions[i];
            cout << "Transaction ID: " << t->transaction_id << endl;
            cout << "Timestamp: " << t->timestamp << endl;
            cout << "Sender: " << t->sender_account << endl;
            cout << "Receiver: " << t->receiver_account << endl;
            cout << "Amount: $" << fixed << setprecision(2) << t->amount << endl;
            cout << "Type: " << t->transaction_type << endl;
            cout << "Merchant: " << t->merchant_category << endl;
            cout << "Location: " << t->location << endl;
            cout << "Is Fraud: " << t->is_fraud << endl;
            cout << "Fraud Type: " << t->fraud_type << endl;
            cout << "Payment Channel: " << t->payment_channel << endl;
            cout << "-------------------------------------" << endl;
        }
        cout << "=====================================" << endl;
    }

    void sortByLocation() {
        if (count < 2) {
            cout << "Not enough transactions to sort." << endl;
            return;
        }
        cout << "Sorting transactions by location using merge sort..." << endl;
        mergeSort(transactions, 0, count - 1);
        cout << "Successfully sorted " << count << " transactions by location." << endl;
    }

    void searchByTransactionType(const string& type) {
        if (count == 0) {
            cout << "No transactions to search." << endl;
            return;
        }

        int found = 0;
        cout << "\nSearching for transaction type '" << type << "'..." << endl;
        cout << "=====================================" << endl;
        for (int i = 0; i < count; ++i) {
            if (transactions[i]->transaction_type == type) {
                ArrayTransaction* t = transactions[i];
                cout << "Match #" << ++found << endl;
                cout << "ID: " << t->transaction_id << endl;
                cout << "Amount: $" << fixed << setprecision(2) << t->amount << endl;
                cout << "Location: " << t->location << endl;
                cout << "Payment Channel: " << t->payment_channel << endl;
                cout << "-------------------------------------" << endl;
            }
        }
        cout << "=====================================" << endl;
        if (found == 0) {
            cout << "No transactions found with type: " << type << endl;
        } else {
            cout << "Found " << found << " matching transactions." << endl;
        }
    }

    void linearSearch(const string& query, const string& field) {
        if (count == 0) {
            cout << "No transactions to search." << endl;
            return;
        }

        auto start = chrono::high_resolution_clock::now();
        int found = 0;

        for (int i = 0; i < count; ++i) {
            if (field == "type" && transactions[i]->transaction_type == query) {
                found++;
            } else if (field == "location" && transactions[i]->location == query) {
                found++;
            } else if (field == "fraud" && transactions[i]->is_fraud == query) {
                found++;
            }
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;

        cout << "\nLinear Search Results for " << field << " = " << query << ":" << endl;
        cout << "Found " << found << " matches in " << elapsed.count() << " seconds" << endl;
    }

    bool binarySearch(const string& query, const string& field) {
        if (count == 0) {
            cout << "No transactions to search." << endl;
            return false;
        }

        // Sort the array if needed
        if (field == "type") {
            sort(transactions, transactions + count, [](ArrayTransaction* a, ArrayTransaction* b) {
                return a->transaction_type < b->transaction_type;
            });
        } else if (field == "location") {
            sort(transactions, transactions + count, [](ArrayTransaction* a, ArrayTransaction* b) {
                return a->location < b->location;
            });
        }

        auto start = chrono::high_resolution_clock::now();

        // Perform binary search
        int left = 0, right = count - 1;
        int found = 0;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (field == "type" && transactions[mid]->transaction_type == query) {
                found = 1;
                // Check duplicates
                int left_ptr = mid - 1;
                while (left_ptr >= 0 && transactions[left_ptr]->transaction_type == query) {
                    found++;
                    left_ptr--;
                }
                int right_ptr = mid + 1;
                while (right_ptr < count && transactions[right_ptr]->transaction_type == query) {
                    found++;
                    right_ptr++;
                }
                break;
            } else if (field == "location" && transactions[mid]->location == query) {
                found = 1;
                int left_ptr = mid - 1;
                while (left_ptr >= 0 && transactions[left_ptr]->location == query) {
                    found++;
                    left_ptr--;
                }
                int right_ptr = mid + 1;
                while (right_ptr < count && transactions[right_ptr]->location == query) {
                    found++;
                    right_ptr++;
                }
                break;
            }
            
            string fieldValue;
            if (field == "type") {
                fieldValue = transactions[mid]->transaction_type;
            } else if (field == "location") {
                fieldValue = transactions[mid]->location;
            }
            
            if (fieldValue < query) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;

        cout << "\nBinary Search Results for " << field << " = " << query << ":" << endl;
        if (found > 0) {
            cout << "Found " << found << " matches in " << elapsed.count() << " seconds" << endl;
        } else {
            cout << "No matches found in " << elapsed.count() << " seconds" << endl;
        }

        return found > 0;
    }

    void exportToJSON(const string& filename) {
        ofstream outFile(filename);
        if (!outFile.is_open()) {
            cout << "Failed to open file for writing: " << filename << endl;
            return;
        }

        outFile << "[\n";
        bool first = true;

        for (int i = 0; i < count; ++i) {
            ArrayTransaction* t = transactions[i];
            if (!first) {
                outFile << ",\n";
            }
            first = false;

            outFile << "  {\n";
            outFile << "    \"transaction_id\": \"" << t->transaction_id << "\",\n";
            outFile << "    \"timestamp\": \"" << t->timestamp << "\",\n";
            outFile << "    \"sender_account\": \"" << t->sender_account << "\",\n";
            outFile << "    \"receiver_account\": \"" << t->receiver_account << "\",\n";
            outFile << "    \"amount\": " << t->amount << ",\n";
            outFile << "    \"transaction_type\": \"" << t->transaction_type << "\",\n";
            outFile << "    \"merchant_category\": \"" << t->merchant_category << "\",\n";
            outFile << "    \"location\": \"" << t->location << "\",\n";
            outFile << "    \"device_used\": \"" << t->device_used << "\",\n";
            outFile << "    \"is_fraud\": \"" << t->is_fraud << "\",\n";
            outFile << "    \"fraud_type\": \"" << t->fraud_type << "\",\n";
            outFile << "    \"time_since_last_transaction\": " << t->time_since_last_transaction << ",\n";
            outFile << "    \"spending_deviation_score\": " << t->spending_deviation_score << ",\n";
            outFile << "    \"velocity_score\": " << t->velocity_score << ",\n";
            outFile << "    \"geo_anomaly_score\": " << t->geo_anomaly_score << ",\n";
            outFile << "    \"payment_channel\": \"" << t->payment_channel << "\",\n";
            outFile << "    \"ip_address\": \"" << t->ip_address << "\",\n";
            outFile << "    \"device_hash\": \"" << t->device_hash << "\"\n";
            outFile << "  }";
        }

        outFile << "\n]";
        outFile.close();
        cout << "Successfully exported " << count << " transactions to " << filename << endl;
    }

    int getCount() const { return count; }
};

// Helper functions
void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool importCSV(const string& filename, TransactionList& achList,
               TransactionList& cardList, TransactionList& upiList,
               TransactionList& wireList) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return false;
    }

    size_t estimatedMemoryUsage = 0;
    int processed = 0;
    string line;
    getline(file, line); // Skip header

    auto start = chrono::high_resolution_clock::now(); // Start timer

    while (getline(file, line)) {
        estimatedMemoryUsage += line.size() * sizeof(char);
        stringstream ss(line);
        string field;
        Transaction* t = new Transaction();

        try {
            getline(ss, t->transaction_id, ',');
            getline(ss, t->timestamp, ',');
            getline(ss, t->sender_account, ',');
            getline(ss, t->receiver_account, ',');

            getline(ss, field, ',');
            t->amount = field.empty() ? 0.0 : stod(field);

            getline(ss, t->transaction_type, ',');
            getline(ss, t->merchant_category, ',');
            getline(ss, t->location, ',');
            getline(ss, t->device_used, ',');
            getline(ss, t->is_fraud, ',');
            getline(ss, t->fraud_type, ',');

            getline(ss, field, ',');
            t->time_since_last_transaction = field.empty() ? 0.0 : stod(field);

            getline(ss, field, ',');
            t->spending_deviation_score = field.empty() ? 0.0 : stod(field);

            getline(ss, field, ',');
            t->velocity_score = field.empty() ? 0 : stoi(field);

            getline(ss, field, ',');
            t->geo_anomaly_score = field.empty() ? 0.0 : stod(field);

            getline(ss, t->payment_channel, ',');
            getline(ss, t->ip_address, ',');
            getline(ss, t->device_hash, ',');

            if (t->payment_channel == "ACH") {
                achList.addTransaction(t);
            } else if (t->payment_channel == "card") {
                cardList.addTransaction(t);
            } else if (t->payment_channel == "UPI") {
                upiList.addTransaction(t);
            } else if (t->payment_channel == "wire_transfer") {
                wireList.addTransaction(t);
            } else {
                delete t;
                continue;
            }

            processed++;
        } catch (const exception& e) {
            cout << "Error parsing line: " << line << endl;
            cout << "Error: " << e.what() << endl;
            delete t;
            continue;
        }
    }

    file.close();
    auto end = chrono::high_resolution_clock::now(); // End timer
    chrono::duration<double> elapsed = end - start; // Calculate elapsed time

    cout << "Successfully processed " << processed << " transactions." << endl;
    cout << "Linked List Implementation:" << endl;
    cout << "Time taken to load CSV: " << elapsed.count() << " seconds." << endl;
    cout << "Estimated memory usage: ~" << estimatedMemoryUsage / (1024 * 1024) << " MB" << endl;
    cout << "Transaction counts by channel:" << endl;
    cout << "- ACH: " << achList.getCount() << endl;
    cout << "- Card: " << cardList.getCount() << endl;
    cout << "- UPI: " << upiList.getCount() << endl;
    cout << "- Wire Transfer: " << wireList.getCount() << endl;

    return true;
}

bool importCSVArray(const string& filename, ArrayTransactionList& achList,
                    ArrayTransactionList& cardList, ArrayTransactionList& upiList,
                    ArrayTransactionList& wireList) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return false;
    }

    size_t estimatedMemoryUsage = 0;
    int processed = 0;
    string line;
    getline(file, line); // Skip header

    auto start = chrono::high_resolution_clock::now(); // Start timer

    while (getline(file, line)) {
        estimatedMemoryUsage += line.size() * sizeof(char);
        stringstream ss(line);
        string field;
        ArrayTransaction* t = new ArrayTransaction();

        try {
            getline(ss, t->transaction_id, ',');
            getline(ss, t->timestamp, ',');
            getline(ss, t->sender_account, ',');
            getline(ss, t->receiver_account, ',');

            getline(ss, field, ',');
            t->amount = field.empty() ? 0.0 : stod(field);

            getline(ss, t->transaction_type, ',');
            getline(ss, t->merchant_category, ',');
            getline(ss, t->location, ',');
            getline(ss, t->device_used, ',');
            getline(ss, t->is_fraud, ',');
            getline(ss, t->fraud_type, ',');

            getline(ss, field, ',');
            t->time_since_last_transaction = field.empty() ? 0.0 : stod(field);

            getline(ss, field, ',');
            t->spending_deviation_score = field.empty() ? 0.0 : stod(field);

            getline(ss, field, ',');
            t->velocity_score = field.empty() ? 0 : stoi(field);

            getline(ss, field, ',');
            t->geo_anomaly_score = field.empty() ? 0.0 : stod(field);

            getline(ss, t->payment_channel, ',');
            getline(ss, t->ip_address, ',');
            getline(ss, t->device_hash, ',');

            if (t->payment_channel == "ACH") {
                achList.addTransaction(t);
            } else if (t->payment_channel == "card") {
                cardList.addTransaction(t);
            } else if (t->payment_channel == "UPI") {
                upiList.addTransaction(t);
            } else if (t->payment_channel == "wire_transfer") {
                wireList.addTransaction(t);
            } else {
                delete t;
                continue;
            }

            processed++;
        } catch (const exception& e) {
            cout << "Error parsing line: " << line << endl;
            cout << "Error: " << e.what() << endl;
            delete t;
            continue;
        }
    }

    file.close();
    auto end = chrono::high_resolution_clock::now(); // End timer
    chrono::duration<double> elapsed = end - start; // Calculate elapsed time

    cout << "Successfully processed " << processed << " transactions." << endl;
    cout << "Array Implementation:" << endl;
    cout << "Time taken to load CSV: " << elapsed.count() << " seconds." << endl;
    cout << "Estimated memory usage: ~" << estimatedMemoryUsage / (1024 * 1024) << " MB" << endl;
    cout << "Transaction counts by channel:" << endl;
    cout << "- ACH: " << achList.getCount() << endl;
    cout << "- Card: " << cardList.getCount() << endl;
    cout << "- UPI: " << upiList.getCount() << endl;
    cout << "- Wire Transfer: " << wireList.getCount() << endl;

    return true;
}

void compareEfficiency() {
    cout << "\nPerformance Comparison Summary:\n";
    cout << "1. Linked List Pros:\n";
    cout << "   - Dynamically grows without reallocation\n";
    cout << "   - Efficient for frequent insertions/deletions\n";
    cout << "   - No memory wastage for unused elements\n";
    cout << "2. Array Pros:\n";
    cout << "   - Better memory locality (faster iteration)\n";
    cout << "   - Constant time random access\n";
    cout << "   - More efficient for binary search\n";
    cout << "3. Recommendation:\n";
    cout << "   - Use arrays when frequent searching is needed\n";
    cout << "   - Use linked lists when frequent modifications are needed\n";
}

void linkedListChannelMenu(TransactionList& list, const string& name) {
    int subChoice;
    do {
        cout << "\n=== " << name << " Transactions (Linked List) (" << list.getCount() << ") ===" << endl;
        cout << "1. Display First 10 Transactions" << endl;
        cout << "2. Display All Transactions" << endl;
        cout << "3. Search by Transaction Type (Linear)" << endl;
        cout << "4. Search by Location (Linear)" << endl;
        cout << "5. Search by Fraud Status (Linear)" << endl;
        cout << "6. Search by Transaction Type (Binary)" << endl;
        cout << "7. Search by Location (Binary)" << endl;
        cout << "8. Sort by Location" << endl;
        cout << "9. Export to JSON" << endl;
        cout << "10. Back to Main Menu" << endl;
        cout << "Enter choice: ";
        cin >> subChoice;
        clearInputBuffer();

        switch (subChoice) {
            case 1:
                list.displayTransactions(10);
                break;
            case 2:
                list.displayTransactions(-1);
                break;
            case 3: {
                string searchType;
                cout << "Enter transaction type to search (Linear): ";
                getline(cin, searchType);
                list.linearSearch(searchType, "type");
                break;
            }
            case 4: {
                string searchLocation;
                cout << "Enter location to search (Linear): ";
                getline(cin, searchLocation);
                list.linearSearch(searchLocation, "location");
                break;
            }
            case 5: {
                string searchFraud;
                cout << "Enter fraud status to search (Linear): ";
                getline(cin, searchFraud);
                list.linearSearch(searchFraud, "fraud");
                break;
            }
            case 6: {
                string searchType;
                cout << "Enter transaction type to search (Binary): ";
                getline(cin, searchType);
                list.binarySearch(searchType, "type");
                break;
            }
            case 7: {
                string searchLocation;
                cout << "Enter location to search (Binary): ";
                getline(cin, searchLocation);
                list.binarySearch(searchLocation, "location");
                break;
            }
            case 8:
                list.sortByLocation();
                break;
            case 9: {
                string filename;
                cout << "Enter filename to export (e.g., ach_transactions.json): ";
                getline(cin, filename);
                list.exportToJSON(filename);
                break;
            }
            case 10:
                break;
            default:
                cout << "Invalid choice." << endl;
                break;
        }
    } while (subChoice != 10);
}

void arrayChannelMenu(ArrayTransactionList& list, const string& name) {
    int subChoice;
    do {
        cout << "\n=== " << name << " Transactions (Array) (" << list.getCount() << ") ===" << endl;
        cout << "1. Display First 10 Transactions" << endl;
        cout << "2. Display All Transactions" << endl;
        cout << "3. Search by Transaction Type (Linear)" << endl;
        cout << "4. Search by Location (Linear)" << endl;
        cout << "5. Search by Fraud Status (Linear)" << endl;
        cout << "6. Search by Transaction Type (Binary)" << endl;
        cout << "7. Search by Location (Binary)" << endl;
        cout << "8. Sort by Location" << endl;
        cout << "9. Export to JSON" << endl;
        cout << "10. Back to Main Menu" << endl;
        cout << "Enter choice: ";
        cin >> subChoice;
        clearInputBuffer();

        switch (subChoice) {
            case 1:
                list.displayTransactions(10);
                break;
            case 2:
                list.displayTransactions(-1);
                break;
            case 3: {
                string searchType;
                cout << "Enter transaction type to search (Linear): ";
                getline(cin, searchType);
                list.linearSearch(searchType, "type");
                break;
            }
            case 4: {
                string searchLocation;
                cout << "Enter location to search (Linear): ";
                getline(cin, searchLocation);
                list.linearSearch(searchLocation, "location");
                break;
            }
            case 5: {
                string searchFraud;
                cout << "Enter fraud status to search (Linear): ";
                getline(cin, searchFraud);
                list.linearSearch(searchFraud, "fraud");
                break;
            }
            case 6: {
                string searchType;
                cout << "Enter transaction type to search (Binary): ";
                getline(cin, searchType);
                list.binarySearch(searchType, "type");
                break;
            }
            case 7: {
                string searchLocation;
                cout << "Enter location to search (Binary): ";
                getline(cin, searchLocation);
                list.binarySearch(searchLocation, "location");
                break;
            }
            case 8:
                list.sortByLocation();
                break;
            case 9: {
                string filename;
                cout << "Enter filename to export (e.g., ach_transactions.json): ";
                getline(cin, filename);
                list.exportToJSON(filename);
                break;
            }
            case 10:
                break;
            default:
                cout << "Invalid choice." << endl;
                break;
        }
    } while (subChoice != 10);
}

int main() {
    TransactionList ll_achList, ll_cardList, ll_upiList, ll_wireList;
    ArrayTransactionList arr_achList, arr_cardList, arr_upiList, arr_wireList;
    
    int implementationChoice;
    string filename;
    
    cout << "Choose implementation:" << endl;
    cout << "1. Linked List" << endl;
    cout << "2. Array" << endl;
    cout << "Enter choice: ";
    cin >> implementationChoice;
    clearInputBuffer();
    
    if (implementationChoice != 1 && implementationChoice != 2) {
        cout << "Invalid choice. Exiting program." << endl;
        return 1;
    }
    
    cout << "Enter CSV filename: ";
    getline(cin, filename);
    
    bool importSuccess = false;
    if (implementationChoice == 1) {
        importSuccess = importCSV(filename, ll_achList, ll_cardList, ll_upiList, ll_wireList);
    } else {
        importSuccess = importCSVArray(filename, arr_achList, arr_cardList, arr_upiList, arr_wireList);
    }
    
    if (!importSuccess) {
        return 1;
    }
    
    int choice;
    do {
        cout << "\n=== MAIN MENU ===" << endl;
        cout << "1. ACH Transactions" << endl;
        cout << "2. Card Transactions" << endl;
        cout << "3. UPI Transactions" << endl;
        cout << "4. Wire Transfer Transactions" << endl;
        cout << "5. Performance Comparison" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
            case 1:
                if (implementationChoice == 1) {
                    linkedListChannelMenu(ll_achList, "ACH");
                } else {
                    arrayChannelMenu(arr_achList, "ACH");
                }
                break;
            case 2:
                if (implementationChoice == 1) {
                    linkedListChannelMenu(ll_cardList, "Card");
                } else {
                    arrayChannelMenu(arr_cardList, "Card");
                }
                break;
            case 3:
                if (implementationChoice == 1) {
                    linkedListChannelMenu(ll_upiList, "UPI");
                } else {
                    arrayChannelMenu(arr_upiList, "UPI");
                }
                break;
            case 4:
                if (implementationChoice == 1) {
                    linkedListChannelMenu(ll_wireList, "Wire Transfer");
                } else {
                    arrayChannelMenu(arr_wireList, "Wire Transfer");
                }
                break;
            case 5:
                compareEfficiency();
                break;
            case 6:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
                break;
        }
    } while (choice != 6);

    return 0;
}