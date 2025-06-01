#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <memory>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>
#include <limits>
using namespace std;

template<typename T>
class AuditLog {
    private:
        struct LogEntry {
        std::string timestamp;
        std::string action;
        T details;
        
        std::string getTimestamp() const { return timestamp; }
        std::string getAction() const { return action; }
        T getDetails() const { return details; }
    };

        std::vector<LogEntry> entries;
        std::ofstream logFile;
        std::string filename;

        std::string getCurrentTime() {
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
            return ss.str();
        }

    void writeToFile(const LogEntry& entry) {
        if (!logFile.is_open()) {
            logFile.open(filename, std::ios::app);
        }
        if (logFile.is_open()) {
            logFile << "[" << entry.timestamp << "] "
                    << entry.action << " - "
                    << entry.details << std::endl;
        }
    }

    public:
        AuditLog(const std::string& logFilename) : filename(logFilename) {
        logFile.open(filename, std::ios::app); 
    }

        ~AuditLog() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

        template<typename U>
        void addEntry(const std::string& action, const U& details) {
            LogEntry entry{getCurrentTime(), action, static_cast<T>(details)};
            entries.emplace_back(entry);
             writeToFile(entry);
        }

        template<typename Formatter>
        void displayLog(Formatter format) const {
            for(const auto& entry : entries) {
                std::cout << format(
                entry.getTimestamp(),
                entry.getAction(),
                entry.getDetails()
            ) << "\n";
        }
    }
};

class DigitalAsset {
    protected:
        char* name;
        float price;
    
    public:
        DigitalAsset() : name(nullptr), price(0.0f) {}
    
        DigitalAsset(const char* name, float price) : price(price) {
            if (!name) throw invalid_argument("Name cannot be null");
             if (price < 0) throw invalid_argument("Price cannot be negative");
            delete[] this->name;
            this->name = new char[strlen(name) + 1];
            strcpy(this->name, name);
    }
    
        DigitalAsset(const DigitalAsset& other) : price(other.price) {
            this->name = new char[strlen(other.name) + 1];
            strcpy(this->name, other.name);
    }
    
        virtual ~DigitalAsset() {
            delete[] name;
    }
    
        const char* getName() const { return name; }
        float getPrice() const { return price; }
    
        void setName(const char* name) {
            delete[] this->name;
            this->name = new char[strlen(name) + 1];
            strcpy(this->name, name);
        }
    
        void setPrice(float price) { 
            if (price < 0) { throw std::invalid_argument("Price cannot be negative.");}
            this->price = price; 
        }
    
        virtual void displayInfo() const = 0;

        virtual float calculateMarketValue() const {return price;}
    
        DigitalAsset& operator=(const DigitalAsset& other) {
            if (this != &other) {
                delete[] name; 
                if (other.name) {
                    this->name = new char[strlen(other.name) + 1];
                    strcpy(this->name, other.name);
                } else {
                    this->name = nullptr;
                }
                this->price = other.price;
            }
             return *this;
        }

};

class Token : virtual public DigitalAsset {
    protected:
        char* symbol;
    
    public:
        Token() : symbol(nullptr) {}
    
        Token(const char* name, const char* symbol, float price) {
            this->symbol = new char[strlen(symbol) + 1];
            strcpy(this->symbol, symbol);
        }

        Token(const Token& other) : symbol(nullptr) {
            this->symbol = new char[strlen(other.symbol) + 1];
            strcpy(this->symbol, other.symbol);
        }
    
        ~Token() {
            delete[] symbol;
         }
    
        const char* getSymbol() const { return symbol; }
    
        void setSymbol(const char* symbol) {
            if (symbol == nullptr) {
                throw std::invalid_argument("Symbol cannot be null.");
            }
            delete[] this->symbol;
            this->symbol = new char[strlen(symbol) + 1];
            strcpy(this->symbol, symbol);
        }
    
        void displayInfo() const override { cout << "Token: " << name << " (" << symbol << "), Price: " << price << endl;}
    
        Token& operator=(const Token& other) {
            if (this != &other) {
                DigitalAsset::operator=(other);
                delete[] symbol;
                symbol = new char[strlen(other.symbol) + 1];
                strcpy(symbol, other.symbol);
            }
            return *this;
        }
};

class MarketEntity : virtual public DigitalAsset {
    protected:
        long long marketCap;
        char currencyType;
    
    public:
        MarketEntity() : DigitalAsset(), marketCap(0), currencyType('U') {}
    
        MarketEntity(long long marketCap, char currencyType = 'U'): marketCap(marketCap), currencyType(currencyType) {}
    
        MarketEntity(const MarketEntity& other) : DigitalAsset(other), marketCap(other.marketCap), currencyType(other.currencyType) {}
        
        virtual ~MarketEntity() {}
    
        long long getMarketCap() const { return marketCap; }
        char getCurrencyType() const { return currencyType; }
    
        void setMarketCap(long long marketCap) { 
            if (marketCap < 0) {
                throw std::invalid_argument("Market cap cannot be negative.");
            }
            this->marketCap = marketCap; 
        }
        
        void setCurrencyType(char type) { currencyType = type; }

        virtual void displayMarketInfo() const { cout << "Market Cap: " << marketCap << ", Currency Type: " << currencyType << endl;}
    
        MarketEntity& operator=(const MarketEntity& other) {
            if (this != &other) {
                marketCap = other.marketCap;
                currencyType = other.currencyType;
            }
            return *this;
        }
};

class CryptoCurrency : public Token, public MarketEntity {
    private:
        static int totalCryptos;
        const int MAX_NAME_LENGTH = 100;
    
    public:
        CryptoCurrency() : Token(), MarketEntity() {totalCryptos++;}
        
        CryptoCurrency(const char* name, const char* symbol, float price, long long marketCap, char currencyType = 'C'): DigitalAsset(name, price), Token(), MarketEntity() {
            setSymbol(symbol);           
            this->marketCap = marketCap;
            this->currencyType = currencyType;
            totalCryptos++;
        }

        CryptoCurrency(const CryptoCurrency& other) : DigitalAsset(other), Token(other), MarketEntity(other) {totalCryptos++;}

        ~CryptoCurrency() {totalCryptos--;}

        virtual void displayInfo() const {displayInfo(std::cout);}

        virtual void displayInfo(std::ostream& os) const { os << "Name: " << name << ", Symbol: " << symbol << ", Price: " << price << ", Market Cap: " << marketCap << ", Currency Type: " << currencyType;}

        void displayMarketInfo() const override { cout << "Cryptocurrency Market Cap: " << marketCap << ", Type: " << currencyType << endl;}
    
        float calculateMarketValue() const override { return price * 100; }

        CryptoCurrency& operator=(const CryptoCurrency& other) {
            if (this != &other) {
                DigitalAsset::operator=(other); 
                Token::operator=(other);
                MarketEntity::operator=(other);
            }
            return *this;
        }
 
        bool operator==(const CryptoCurrency& other) const { return strcmp(name, other.name) == 0 && strcmp(symbol, other.symbol) == 0;}

        bool operator<(const CryptoCurrency& other) const { return marketCap < other.marketCap;}

        CryptoCurrency operator+(const CryptoCurrency& other) const { return CryptoCurrency(name, symbol, price + other.price, marketCap + other.marketCap);}

        CryptoCurrency operator*(const CryptoCurrency& other) const {return CryptoCurrency(name, symbol, price * other.price, marketCap * other.marketCap);}

        CryptoCurrency operator-(const CryptoCurrency& other) const { return CryptoCurrency(name, symbol, price - other.price, marketCap - other.marketCap);}

        CryptoCurrency operator/(const CryptoCurrency& other) const {return CryptoCurrency(name, symbol, price / other.price, marketCap / other.marketCap);}

        CryptoCurrency& operator++() {
            setPrice(getPrice() + 1.0f); 
            return *this;
        }
        
        CryptoCurrency operator++(int) {
            CryptoCurrency temp = *this;
            price += 1.0f;
            return temp;
        }

        char operator[](int index) const {
            if (index < 0 || index >= strlen(name)) {throw out_of_range("Index out of range");}
            return name[index];
        }
    
        static int getTotalCryptos() { return totalCryptos; }
    
        friend ostream& operator<<(ostream& os, const CryptoCurrency& crypto);
        
        friend istream& operator>>(istream& is, CryptoCurrency& crypto);
};

int CryptoCurrency::totalCryptos = 0;

class UtilityCoin : virtual public CryptoCurrency {
    private:
        char* useCase;
    
    public:
        UtilityCoin() : CryptoCurrency(),useCase(nullptr) {}
    
        UtilityCoin(const char* name, const char* symbol, float price, long long marketCap, const char* useCase, char currencyType = 'U'): CryptoCurrency(name, symbol, price, marketCap, currencyType) {
            this->useCase = new char[strlen(useCase) + 1];
            this->setName(name);
            strcpy(this->useCase, useCase);
        }
    
        UtilityCoin(const UtilityCoin& other) : CryptoCurrency(other) {
            this->useCase = new char[strlen(other.useCase) + 1];
            strcpy(this->useCase, other.useCase);
        }
    
        ~UtilityCoin() { delete[] useCase; }
    
        const char* getUseCase() const { return useCase; }
    
        void setUseCase(const char* useCase) {
            delete[] this->useCase;
            this->useCase = new char[strlen(useCase) + 1];
            strcpy(this->useCase, useCase);
        }
    
        void displayInfo() const override {displayInfo(std::cout);}

        void displayInfo(std::ostream& os) const override {
            CryptoCurrency::displayInfo(os);
            if (useCase)
                os << ", Use Case: " << useCase;
            else
                os << ", Use Case: N/A";
        }
    
        float calculateMarketValue() const override {return price * 120; }
    
        UtilityCoin& operator=(const UtilityCoin& other) {
            if (this != &other) {
                CryptoCurrency::operator=(other);
                delete[] useCase;
                useCase = new char[strlen(other.useCase) + 1];
                strcpy(useCase, other.useCase);
            }
            return *this;
        }
};

class StableCoin : virtual public CryptoCurrency {
    private:
        char* backedBy;
    
    public:
        StableCoin() : CryptoCurrency(), backedBy(nullptr) {}
    
        StableCoin(const char* name, const char* symbol, float price, long long marketCap, const char* backedBy, char currencyType = 'S'): CryptoCurrency(name, symbol, price, marketCap, currencyType) {
            this->backedBy = new char[strlen(backedBy) + 1];
            this->setName(name);
            strcpy(this->backedBy, backedBy);
        }
    
        StableCoin(const StableCoin& other) : CryptoCurrency(other) {
            this->backedBy = new char[strlen(other.backedBy) + 1];
            strcpy(this->backedBy, other.backedBy);
        }
    
        ~StableCoin() { delete[] backedBy;}
    
        const char* getBackedBy() const { return backedBy; }
    
        void setBackedBy(const char* backedBy) {
            delete[] this->backedBy;
            this->backedBy = new char[strlen(backedBy) + 1];
            strcpy(this->backedBy, backedBy);
        }
    
        void displayInfo() const override { displayInfo(std::cout);}

        void displayInfo(std::ostream& os) const override {
            CryptoCurrency::displayInfo(os);
            os << ", Backed By: " << backedBy;
        }
        
        float calculateMarketValue() const override { return price * 110; }
    
        StableCoin& operator=(const StableCoin& other) {
            if (this != &other) {
                CryptoCurrency::operator=(other);
                delete[] backedBy;
                backedBy = new char[strlen(other.backedBy) + 1];
                strcpy(backedBy, other.backedBy);
            }
            return *this;
        }
};

ostream& operator<<(ostream& os, const CryptoCurrency& crypto) {
        crypto.displayInfo(os);
        return os;
    }

istream& operator>>(istream& is, CryptoCurrency& crypto) {
    char buffer[100];
    float price;
    long long marketCap;
    char currencyType;

    cout << "Enter name: ";
    is >> buffer;
    crypto.setName(buffer);

    cout << "Enter symbol: ";
    is >> buffer;
    crypto.setSymbol(buffer);

    cout << "Enter price: ";
    is >> price;
    crypto.setPrice(price); 

    cout << "Enter market cap: ";
    is >> marketCap;
    crypto.setMarketCap(marketCap); 

    cout << "Enter currency type (C for Crypto): ";
    is >> currencyType;
    crypto.setCurrencyType(currencyType);

    return is;
}

class Wallet {
    private:
        char* owner;
        CryptoCurrency* cryptos;
        int numCryptos;
        int capacity;
        bool isActive; 

    public:
        Wallet() : owner(nullptr), cryptos(nullptr), numCryptos(0), capacity(10), isActive(true) {cryptos = new CryptoCurrency[capacity];}

        Wallet(const char* owner) : numCryptos(0), capacity(10), isActive(true) {
            this->owner = new char[strlen(owner) + 1];
            strcpy(this->owner, owner);
            cryptos = new CryptoCurrency[capacity];
        }

        Wallet(const char* owner, int capacity) : numCryptos(0), capacity(capacity), isActive(true) {
            this->owner = new char[strlen(owner) + 1];
            strcpy(this->owner, owner);
            cryptos = new CryptoCurrency[capacity];
        }

        Wallet(const Wallet& other) {
            owner = new char[strlen(other.owner) + 1];
            strcpy(owner, other.owner);
            capacity = other.capacity;
            numCryptos = other.numCryptos;
            cryptos = new CryptoCurrency[capacity];
            for (int i = 0; i < numCryptos; i++) {
                cryptos[i] = other.cryptos[i];
            }
            isActive = other.isActive; 
        }
    
         ~Wallet() {
            delete[] owner;
            delete[] cryptos;
        }

        const char* getOwner() const { return owner; }
        
        int getNumCryptos() const { return numCryptos; }
        
        int getCapacity() const { return capacity; }
        
        bool getIsActive() const { return isActive; } 

        void setOwner(const char* owner) {
            delete[] this->owner;
            this->owner = new char[strlen(owner) + 1];
            strcpy(this->owner, owner);
        }

        void setIsActive(bool isActive) { this->isActive = isActive;}
   
        void setNumCryptos(int numCryptos) {this->numCryptos = numCryptos;}
   
        void addCrypto(const CryptoCurrency& crypto) {
            if (numCryptos >= capacity) {
                capacity *= 2;
                CryptoCurrency* newCryptos = new CryptoCurrency[capacity];
                for (int i = 0; i < numCryptos; i++) {
                    newCryptos[i] = cryptos[i];
                }
                delete[] cryptos;
                cryptos = newCryptos;
            }
            cryptos[numCryptos++] = crypto;
        }

        Wallet& operator=(const Wallet& other) {
            if (this != &other) {
                delete[] owner;
                delete[] cryptos;

                owner = new char[strlen(other.owner) + 1];
                strcpy(owner, other.owner);

                capacity = other.capacity;
                numCryptos = other.numCryptos;
                cryptos = new CryptoCurrency[capacity];
                for (int i = 0; i < numCryptos; i++) {
                    cryptos[i] = other.cryptos[i];
                }
                isActive = other.isActive;
            }
            return *this;
        }

        bool operator==(const Wallet& other) const {return strcmp(owner, other.owner) == 0 && numCryptos == other.numCryptos;}

        bool operator<(const Wallet& other) const {return numCryptos < other.numCryptos;}

        Wallet operator+(const Wallet& other) const {
            Wallet newWallet(owner, capacity + other.capacity);
            for (int i = 0; i < numCryptos; i++) {
                newWallet.addCrypto(cryptos[i]);
            }
            for (int i = 0; i < other.numCryptos; i++) {
                newWallet.addCrypto(other.cryptos[i]);
            }
            return newWallet;
        }

        Wallet operator*(const Wallet& other) const {
            Wallet newWallet(owner, capacity * other.capacity);
            for (int i = 0; i < numCryptos; i++) {
                newWallet.addCrypto(cryptos[i]);
            }
            for (int i = 0; i < other.numCryptos; i++) {
                newWallet.addCrypto(other.cryptos[i]);
            }
            return newWallet;
        }

        Wallet& operator++() {
            capacity += 10;
            CryptoCurrency* newCryptos = new CryptoCurrency[capacity];
            for (int i = 0; i < numCryptos; i++) {
                newCryptos[i] = cryptos[i];
            }
            delete[] cryptos;
            cryptos = newCryptos;
            return *this;
        }

        Wallet operator++(int) {
            Wallet temp = *this;
            ++(*this);
            return temp;
        }
    
        CryptoCurrency& operator[](int index) {
            if (index < 0 || index >= numCryptos) {
                throw out_of_range("Index out of range");
            }
            return cryptos[index];
        }

        friend ostream& operator<<(ostream& os, const Wallet& wallet);
        friend istream& operator>>(istream& is, Wallet& wallet);
};

ostream& operator<<(ostream& os, const Wallet& wallet) {
    os << "Owner: " << wallet.owner << "\n";
    os << "Active: " << (wallet.isActive ? "Yes" : "No") << "\n";
    os << "Cryptos:\n";
    for (int i = 0; i < wallet.numCryptos; i++) {
        os << wallet.cryptos[i] << "\n";
    }
    return os;
}

istream& operator>>(istream& is, Wallet& wallet) {
    char buffer[100];
    cout << "Enter wallet owner name: ";
    is >> buffer;
    wallet.setOwner(buffer);

    char activeStatus;
    cout << "Is the wallet active? (y/n): ";
    is >> activeStatus;
    wallet.setIsActive(activeStatus == 'y' || activeStatus == 'Y');

    return is;
}

class Transaction {
    private:
        Wallet* from;
        Wallet* to;
        CryptoCurrency crypto;
        float amount;

    public:
        Transaction() : from(nullptr), to(nullptr), amount(0.0f) {}

        Transaction(Wallet* from, Wallet* to, const CryptoCurrency& crypto, float amount): from(from), to(to), crypto(crypto), amount(amount) {}

        Transaction(const Transaction& other) : from(other.from), to(other.to), crypto(other.crypto), amount(other.amount) {}

        ~Transaction() {}

        Wallet* getFrom() const { return from; }
        
        Wallet* getTo() const { return to; }
        
        CryptoCurrency getCrypto() const { return crypto; }
        
        float getAmount() const { return amount; }

        void setFrom(Wallet* from) { this->from = from; } 
        
        void setTo(Wallet* to) { this->to = to; }
        
        void setCrypto(const CryptoCurrency& crypto) { this->crypto = crypto; }
        
        void setAmount(float amount) { this->amount = amount; }

        Transaction& operator=(const Transaction& other) {
            if (this != &other) {
                from = other.from;
                to = other.to;
                crypto = other.crypto;
                amount = other.amount;
            }
            return *this;
        }

        bool operator==(const Transaction& other) const {return from == other.from && to == other.to && crypto == other.crypto && amount == other.amount;}

        bool operator<(const Transaction& other) const {return amount < other.amount;}

        Transaction operator+(const Transaction& other) const {
            if (from == other.from && to == other.to && crypto == other.crypto) {
                return Transaction(from, to, crypto, amount + other.amount);
            } else {
                throw invalid_argument("Cannot add transactions with different participants or cryptos.");
            }
        }

        Transaction operator-(const Transaction& other) const {
            if (from == other.from && to == other.to && crypto == other.crypto) {
                return Transaction(from, to, crypto, amount - other.amount);
            } else {
                throw invalid_argument("Cannot subtract transactions with different participants or cryptos.");
            }
        }

        Transaction& operator++() {
            amount += 1.0f;
            return *this;
        }

        Transaction operator++(int) {
            Transaction temp = *this;
            amount += 1.0f;
            return temp;
        }

        float operator[](int index) const {
            if (index != 0) {
                throw out_of_range("Index out of range");
            }
            return amount;
        }

        friend ostream& operator<<(ostream& os, const Transaction& transaction);
};

ostream& operator<<(ostream& os, const Transaction& transaction) {
    os << "Transaction: " << transaction.amount << " of " << transaction.crypto.getSymbol() << " from " << transaction.from->getOwner() << " to " << transaction.to->getOwner();
    return os;
}

class Blockchain {
    private:
        Transaction** transactions;
        int numTransactions;
        int capacity;

    public:
        Blockchain() : numTransactions(0), capacity(10) {transactions = new Transaction*[capacity];}

        Blockchain(int capacity) : numTransactions(0), capacity(capacity) { transactions = new Transaction*[capacity];}
    
        Blockchain(const Blockchain& other) {
            capacity = other.capacity;
            numTransactions = other.numTransactions;
            transactions = new Transaction*[capacity];
            for (int i = 0; i < numTransactions; i++) {
                transactions[i] = new Transaction(*other.transactions[i]);
            }
        }

        ~Blockchain() {
            for (int i = 0; i < numTransactions; i++) {
                delete transactions[i];
            }
            delete[] transactions;
        }

        int getNumTransactions() const { return numTransactions; }
        
        int getCapacity() const { return capacity; }
        
        Transaction** getTransactions() {return transactions;}
   
        void addTransaction(Transaction* transaction) {
            if (numTransactions >= capacity) {
                capacity *= 2;
                Transaction** newTransactions = new Transaction*[capacity];
                for (int i = 0; i < numTransactions; i++) {
                    newTransactions[i] = transactions[i];
                }
                delete[] transactions;
                transactions = newTransactions;
            }
            transactions[numTransactions++] = transaction;
        }
    
        void setNumTransactions(int numTransactions) {this->numTransactions = numTransactions; }

        Blockchain& operator=(const Blockchain& other) {
            if (this != &other) {
                for (int i = 0; i < numTransactions; i++) {
                    delete transactions[i];
                }
                delete[] transactions;

                capacity = other.capacity;
                numTransactions = other.numTransactions;
                transactions = new Transaction*[capacity];
                for (int i = 0; i < numTransactions; i++) {
                    transactions[i] = new Transaction(*other.transactions[i]);
                }
            }
            return *this;
        }

        bool operator==(const Blockchain& other) const {return numTransactions == other.numTransactions;}

        bool operator<(const Blockchain& other) const {return numTransactions < other.numTransactions;}

        Blockchain operator+(const Blockchain& other) const {
            Blockchain newBlockchain(capacity + other.capacity);
            for (int i = 0; i < numTransactions; i++) {
                newBlockchain.addTransaction(new Transaction(*transactions[i]));
            }
            for (int i = 0; i < other.numTransactions; i++) {
                newBlockchain.addTransaction(new Transaction(*other.transactions[i]));
            }
            return newBlockchain;
        }

        Blockchain operator-(const Blockchain& other) const {
            Blockchain newBlockchain(capacity);
            for (int i = 0; i < numTransactions; i++) {
                bool found = false;
                for (int j = 0; j < other.numTransactions; j++) {
                    if (*transactions[i] == *other.transactions[j]) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    newBlockchain.addTransaction(new Transaction(*transactions[i]));
                }
            }
            return newBlockchain;
        }

        Blockchain& operator++() {
            capacity += 10;
            Transaction** newTransactions = new Transaction*[capacity];
            for (int i = 0; i < numTransactions; i++) {
                newTransactions[i] = transactions[i];
            }
            delete[] transactions;
            transactions = newTransactions;
            return *this;
        }

        Blockchain operator++(int) {
            Blockchain temp = *this;
            ++(*this);
            return temp;
        }

        Transaction* operator[](int index) {
            if (index < 0 || index >= numTransactions) {
                throw out_of_range("Index out of range");
            }
            return transactions[index];
        }

        friend ostream& operator<<(ostream& os, const Blockchain& blockchain);
};

ostream& operator<<(ostream& os, const Blockchain& blockchain) {
    os << "Blockchain Transactions:\n";
    for (int i = 0; i < blockchain.numTransactions; i++) {
        os << *blockchain.transactions[i] << "\n";
    }
    return os;
}

void demonstratePolymorphism(DigitalAsset** assets, int count) {
    cout << "\n=== Polymorphism Demonstration ===\n";
    for (int i = 0; i <count; i++) {
        cout << "Asset " << i + 1 << ":\n";
        assets[i]->displayInfo();
        cout << "Market value calculation: " << assets[i]->calculateMarketValue() << endl;
        cout << endl;
    }
} 

template<typename T>
void resizeArray(T**& array, int& capacity, int required) {
    try {
        if (required <= capacity) return;
        int newCapacity = capacity * 2;
        if (newCapacity < required) newCapacity = required + 10;
        T** newArray = new T*[newCapacity];
        for (int i = 0; i < capacity; i++) {
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    } 
    catch (const bad_alloc& e) {
        throw runtime_error("Failed to resize array: memory allocation failed");
    }
}

bool loadCryptosFromFile(const string& filename, CryptoCurrency**& cryptos, int& numCryptos, int& cryptosCapacity,UtilityCoin**& utilityCoins, int& numUtilityCoins, int& utilityCoinsCapacity,StableCoin**& stableCoins, int& numStableCoins, int& stableCoinsCapacity, DigitalAsset**& assets, int& numAssets, int& assetsCapacity, AuditLog<std::string>& generalLog, AuditLog<int>& statsLog) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string type;
        ss >> type;

        if (type == "CRYPTO") {
            string name, symbol;
            float price;
            long long marketCap;
            char currencyType;

            if (ss >> name >> symbol >> price >> marketCap >> currencyType) {
                resizeArray(cryptos, cryptosCapacity, numCryptos + 1);
                resizeArray(assets, assetsCapacity, numAssets + 1);
                
                CryptoCurrency* crypto = new CryptoCurrency(
                    name.c_str(), symbol.c_str(), price, marketCap, currencyType
                );
                cryptos[numCryptos++] = crypto;
                assets[numAssets++] = crypto;
                
                generalLog.addEntry("Crypto loaded", name);
                statsLog.addEntry("Total cryptos", numCryptos);
            } else {
                cerr << "Skipping invalid CRYPTO line: " << line << endl;
            }
        } else if (type == "UTILITY") {
            string name, symbol, useCase;
            float price;
            long long marketCap;
            char currencyType;

            if (ss >> name >> symbol >> price >> marketCap >> useCase >> currencyType) {
                resizeArray(utilityCoins, utilityCoinsCapacity, numUtilityCoins + 1);
                resizeArray(assets, assetsCapacity, numAssets + 1);
                
                UtilityCoin* utility = new UtilityCoin(
                    name.c_str(), symbol.c_str(), price, marketCap, useCase.c_str(), currencyType
                );
                utilityCoins[numUtilityCoins++] = utility;
                assets[numAssets++] = utility;
                
                generalLog.addEntry("Utility loaded", name);
                statsLog.addEntry("Total utilities", numUtilityCoins);
            } else {
                cerr << "Skipping invalid UTILITY line: " << line << endl;
            }
        } else if (type == "STABLE") {
            string name, symbol, backedBy;
            float price;
            long long marketCap;
            char currencyType;

            if (ss >> name >> symbol >> price >> marketCap >> backedBy >> currencyType) {
                resizeArray(stableCoins, stableCoinsCapacity, numStableCoins + 1);
                resizeArray(assets, assetsCapacity, numAssets + 1);
                
                StableCoin* stable = new StableCoin(
                    name.c_str(), symbol.c_str(), price, marketCap, backedBy.c_str(), currencyType
                );
                stableCoins[numStableCoins++] = stable;
                assets[numAssets++] = stable;
                
                generalLog.addEntry("Stable loaded", name);
                statsLog.addEntry("Total stables", numStableCoins);
            } else {
                cerr << "Skipping invalid STABLE line: " << line << endl;
            }
        } else {
            cerr << "Unknown type: " << type << endl;
        }
    }
    
    file.close();
    return true;
}

bool loadWalletsFromFile(const string& filename, Wallet**& wallets, int& numWallets, int& walletsCapacity, CryptoCurrency** cryptos, int numCryptos, UtilityCoin** utilityCoins, int numUtilityCoins, StableCoin** stableCoins, int numStableCoins){
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return false;
    }

    string line, owner;
    int capacity, numCrypto;
    char isActiveChar;
    bool isActive;

    while (file >> owner >> capacity >> isActiveChar) {
        resizeArray(wallets, walletsCapacity, numWallets + 1);
        
        isActive = (isActiveChar == 'Y' || isActiveChar == 'y');
        
        Wallet* wallet = new Wallet(owner.c_str(), capacity);
        wallet->setIsActive(isActive);
        
        file >> numCrypto;
        for (int i = 0; i < numCrypto; i++) {
            string cryptoName;
            file >> cryptoName;

            bool found = false;
            for (int j = 0; j < numCryptos && !found; j++) {
                if (cryptoName == cryptos[j]->getName()) {
                    wallet->addCrypto(*cryptos[j]);
                    found = true;
                }
            }
            
            for (int j = 0; j < numUtilityCoins && !found; j++) {
                if (cryptoName == utilityCoins[j]->getName()) {
                    wallet->addCrypto(*utilityCoins[j]);
                    found = true;
                }
            }
            
            for (int j = 0; j < numStableCoins && !found; j++) {
                if (cryptoName == stableCoins[j]->getName()) {
                    wallet->addCrypto(*stableCoins[j]);
                    found = true;
                }
            }
        }
        
        wallets[numWallets++] = wallet;
    }
    
    file.close();
    return true;
}

bool loadTransactionsFromFile(const string& filename, Blockchain& blockchain, Wallet** wallets, int numWallets, CryptoCurrency** cryptos, int numCryptos, UtilityCoin** utilityCoins, int numUtilityCoins, StableCoin** stableCoins, int numStableCoins) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return false;
    }

    string fromOwner, toOwner, cryptoName;
    float amount;

    while (file >> fromOwner >> toOwner >> cryptoName >> amount) {
        Wallet* fromWallet = nullptr;
        Wallet* toWallet = nullptr;
        
        for (int i = 0; i < numWallets; i++) {
            if (fromOwner == wallets[i]->getOwner()) {
                fromWallet = wallets[i];
            }
            if (toOwner == wallets[i]->getOwner()) {
                toWallet = wallets[i];
            }
        }
        
        if (!fromWallet || !toWallet) {
            cout << "Warning: Couldn't find wallets for transaction between " 
                << fromOwner << " and " << toOwner << endl;
            continue;
        }

        CryptoCurrency* crypto = nullptr;
        
        for (int i = 0; i < numCryptos; i++) {
            if (cryptoName == cryptos[i]->getName()) {
                crypto = cryptos[i];
                break;
            }
        }
        
        if (!crypto) {
            for (int i = 0; i < numUtilityCoins; i++) {
                if (cryptoName == utilityCoins[i]->getName()) {
                    crypto = utilityCoins[i];
                    break;
                }
            }
        }
        
        if (!crypto) {
            for (int i = 0; i < numStableCoins; i++) {
                if (cryptoName == stableCoins[i]->getName()) {
                    crypto = stableCoins[i];
                    break;
                }
            }
        }
        
        if (!crypto) {
            cout << "Warning: Couldn't find cryptocurrency " << cryptoName << " for transaction" << endl;
            continue;
        }
        Transaction* transaction = new Transaction(fromWallet, toWallet, *crypto, amount);
        blockchain.addTransaction(transaction);
        
        toWallet->addCrypto(*crypto);
    }
    
    file.close();
    return true;
} 

class Meniu {
    private:
        static Meniu* instance;
        AuditLog<std::string> generalLog; 
        AuditLog<int> statsLog; 
        Meniu() : generalLog("audit_general.log"), statsLog("audit_stats.log") {} 
    public:
        static Meniu* getInstance() {
            if (instance == nullptr) { instance = new Meniu();}
            return instance;
        }

        void afiseazaMeniu() {
            cout << "\n=== Coinqueror (the better version of Binance) ===\n";
            cout << "\nA market where you buy the ownership of a cryptocurrency, not just some coins. \n";
            cout << "1. Add CryptoCurrency\n";
            cout << "2. Add Utility Coin\n";
            cout << "3. Add Stable Coin\n";
            cout << "4. Add Wallet\n";
            cout << "5. Add Transaction\n";  
            cout << "6. View All Cryptos\n";
            cout << "7. View All Wallets\n";
            cout << "8. View Blockchain Transactions\n";
            cout << "9. Update CryptoCurrency\n";
            cout << "10. Delete CryptoCurrency\n";
            cout << "11. Update Wallet\n";
            cout << "12. Delete Wallet\n";
            cout << "13. View Wallet Details\n";
            cout << "14. View Transaction Details\n";
            cout << "15. Delete Transaction\n";
            cout << "16. Compare Two Cryptos\n"; 
            cout << "17. Increment Crypto Price\n";
            cout << "18. Subtract Two Cryptos\n";
            cout << "19. Access Crypto in Wallet\n";
            cout << "20. Add Crypto to Wallet\n";
            cout << "21. Show Polymorphism Demo\n";
            cout << "22. Demonstrate Diamond Inheritance\n";
            cout << "23.Show audit log\n";
            cout << "24. Exit\n";
            cout << "Enter your choice: ";
        }

        void ruleaza() {
            const int INITIAL_CAPACITY = 10;
            CryptoCurrency** cryptos = new CryptoCurrency*[INITIAL_CAPACITY];
            int numCryptos = 0;
            int cryptosCapacity = INITIAL_CAPACITY;
            
            UtilityCoin** utilityCoins = new UtilityCoin*[INITIAL_CAPACITY];
            int numUtilityCoins = 0;
            int utilityCoinsCapacity = INITIAL_CAPACITY;
            
            StableCoin** stableCoins = new StableCoin*[INITIAL_CAPACITY];
            int numStableCoins = 0;
            int stableCoinsCapacity = INITIAL_CAPACITY;
            
            DigitalAsset** assets = new DigitalAsset*[INITIAL_CAPACITY];
            int numAssets = 0;
            int assetsCapacity = INITIAL_CAPACITY;
            
            Blockchain blockchain;
            
            Wallet** wallets = new Wallet*[INITIAL_CAPACITY];
            int numWallets = 0;
            int walletsCapacity = INITIAL_CAPACITY;
            int choice;
            cout << "Loading data from files...\n";
        
            if (loadCryptosFromFile("cryptos.txt", cryptos, numCryptos, cryptosCapacity,  utilityCoins, numUtilityCoins, utilityCoinsCapacity, stableCoins, numStableCoins, stableCoinsCapacity, assets, numAssets, assetsCapacity, generalLog, statsLog)) {
                cout << "Successfully loaded cryptocurrencies from file.\n";
            } else {
                cout << "Using default cryptocurrency data.\n";
            
                CryptoCurrency* bitcoin = new CryptoCurrency("Bitcoin", "BTC", 45000.0f, 850000000000LL, 'C');
                cryptos[numCryptos++] = bitcoin;
                assets[numAssets++] = bitcoin;
                generalLog.addEntry("Crypto added", bitcoin->getName());
                statsLog.addEntry("Total cryptos", numCryptos);
    
                CryptoCurrency* ethereum = new CryptoCurrency("Ethereum", "ETH", 3000.0f, 350000000000LL, 'C');
                cryptos[numCryptos++] = ethereum;
                assets[numAssets++] = ethereum;
                generalLog.addEntry("Crypto added", ethereum->getName());
                statsLog.addEntry("Total cryptos", numCryptos);
    
                UtilityCoin* filecoin = new UtilityCoin("Filecoin", "FIL", 40.0f, 7000000000LL, "Decentralized Storage", 'U');
                utilityCoins[numUtilityCoins++] = filecoin;
                assets[numAssets++] = filecoin;
                generalLog.addEntry("Utility added", filecoin->getName());
                statsLog.addEntry("Total Utility", numUtilityCoins);
    
                StableCoin* tether = new StableCoin("Tether", "USDT", 1.0f, 70000000000LL, "USD", 'S');
                stableCoins[numStableCoins++] = tether;
                assets[numAssets++] = tether;
                generalLog.addEntry("Stable added", tether->getName());
                statsLog.addEntry("Total stables", numStableCoins);
            }
        
            if (loadWalletsFromFile("wallets.txt", wallets, numWallets, walletsCapacity, cryptos, numCryptos, utilityCoins, numUtilityCoins, stableCoins, numStableCoins)) {
                cout << "Successfully loaded wallets from file.\n";
            } else {
                cout << "Using default wallet data.\n";

                Wallet* aliceWallet = new Wallet("Alice");
                if (numCryptos > 0) aliceWallet->addCrypto(*cryptos[0]);
                if (numCryptos > 1) aliceWallet->addCrypto(*cryptos[1]);
                wallets[numWallets++] = aliceWallet;
    
                Wallet* bobWallet = new Wallet("Bob");
                if (numStableCoins > 0) bobWallet->addCrypto(*stableCoins[0]);
                wallets[numWallets++] = bobWallet;
            }
        
            if (loadTransactionsFromFile("transactions.txt", blockchain,wallets, numWallets, cryptos, numCryptos, utilityCoins, numUtilityCoins, stableCoins, numStableCoins)) {
                cout << "Successfully loaded transactions from file.\n";
            } else {
                 cout << "Using default transaction data.\n";

                if (numWallets >= 2 && numCryptos >= 1) {
                    Transaction* transaction = new Transaction(wallets[0], wallets[1], *cryptos[0], 0.5);
                     blockchain.addTransaction(transaction);
                }
            }
            do {
                afiseazaMeniu();
                if (!(cin >> choice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number between 1-24.\n";
                    continue;
                }
                switch (choice) {
                   case 1: {
                        CryptoCurrency* crypto = new CryptoCurrency();
                        cin >> *crypto;
                        cryptos[numCryptos++] = crypto;
                        assets[numAssets++] = crypto;
                        generalLog.addEntry("Crypto added", crypto->getName());
                        statsLog.addEntry("Total cryptos", numCryptos);
                        cout << "CryptoCurrency added successfully!\n";
                        break;
                    }
                   case 2: {
                        try {
                            char name[100], symbol[100], useCase[100];
                            float price;
                            long long marketCap;
                            char currencyType;
                            
                            cout << "Enter name: ";
                            cin >> name;
                            cout << "Enter symbol: ";
                            cin >> symbol;
                            
                            cout << "Enter price: ";
                            if (!(cin >> price) || price < 0) {
                                if (!cin) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    throw invalid_argument("Price must be a positive number");
                                }
                                throw invalid_argument("Price cannot be negative");
                            }
                            
                            cout << "Enter market cap: ";
                            if (!(cin >> marketCap) || marketCap < 0) {
                                if (!cin) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    throw invalid_argument("Market cap must be a positive number");
                                }
                                throw invalid_argument("Market cap cannot be negative");
                            }
                            
                            cout << "Enter use case: ";
                            if (!(cin >> useCase) || strlen(useCase) == 0) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                throw invalid_argument("Use case cannot be empty");
                            }
                            cout << "Enter currency type (U for Utility): ";
                            if (!(cin >> currencyType)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                throw invalid_argument("Invalid currency type input");
                            }
                            if (toupper(currencyType) != 'U') {
                                throw invalid_argument("Utility coins must have currency type 'U'");
                            }
                            
                            UtilityCoin* utilityCoin = nullptr;
                            try {
                                utilityCoin = new UtilityCoin(name, symbol, price, marketCap, useCase, currencyType);
                            } catch (const bad_alloc& e) {
                                throw runtime_error("Memory allocation failed for new UtilityCoin");
                            }
                            
                            if (numUtilityCoins >= utilityCoinsCapacity) {
                                resizeArray(utilityCoins, utilityCoinsCapacity, numUtilityCoins + 1);
                            }
                            if (numAssets >= assetsCapacity) {
                                resizeArray(assets, assetsCapacity, numAssets + 1);
                            }
                            
                            utilityCoins[numUtilityCoins++] = utilityCoin;
                            assets[numAssets++] = utilityCoin;
                            
                            generalLog.addEntry("Utility added", utilityCoin->getName());
                            statsLog.addEntry("Total utilities", numUtilityCoins);
                            cout << "Utility Coin added successfully!\n";
                        } 
                        catch (const invalid_argument& e) {
                            cout << "Input Error: " << e.what() << endl;
                        }
                        catch (const runtime_error& e) {
                            cout << "System Error: " << e.what() << endl;
                        }
                        catch (...) {
                            cout << "Unknown error occurred while adding Utility Coin" << endl;
                        }
                        break;
                    }
                   case 3: { // Add Stable Coin
                        try {
                            char name[100], symbol[100], backedBy[100];
                            float price;
                            long long marketCap;
                            char currencyType;

                            cout << "Enter name: ";
                            if (!(cin >> name) || strlen(name) == 0) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                throw invalid_argument("Name cannot be empty");
                            }

                            cout << "Enter symbol: ";
                            if (!(cin >> symbol) || strlen(symbol) == 0) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                throw invalid_argument("Symbol cannot be empty");
                            }

                            cout << "Enter price: ";
                            if (!(cin >> price)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                throw invalid_argument("Invalid price format");
                            }
                            if (price < 0) throw invalid_argument("Price cannot be negative");

                            cout << "Enter market cap: ";
                            if (!(cin >> marketCap)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                throw invalid_argument("Invalid market cap format");
                            }
                            if (marketCap < 0) throw invalid_argument("Market cap cannot be negative");

                            cout << "Enter backed by: ";
                            if (!(cin >> backedBy) || strlen(backedBy) == 0) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                throw invalid_argument("Backing asset cannot be empty");
                            }
                    
                            cout << "Enter currency type (S for Stable): ";
                            if (!(cin >> currencyType)) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                throw invalid_argument("Invalid currency type input");
                            }
                            if (toupper(currencyType) != 'S') {
                                throw invalid_argument("Stable coins must have currency type 'S'");
                            }

                            StableCoin* stableCoin = nullptr;
                            try {
                                stableCoin = new StableCoin(name, symbol, price, marketCap, backedBy, currencyType);
                            } catch (const bad_alloc& e) {
                                throw runtime_error("Failed to allocate memory for StableCoin");
                            } catch (const exception& e) {
                                throw runtime_error(string("Object creation failed: ") + e.what());
                            }

                            try {
                                if (numStableCoins >= stableCoinsCapacity) {
                                    resizeArray(stableCoins, stableCoinsCapacity, numStableCoins + 1);
                                }
                                if (numAssets >= assetsCapacity) {
                                    resizeArray(assets, assetsCapacity, numAssets + 1);
                                }
                            } catch (const exception& e) {
                                delete stableCoin;
                                throw runtime_error(string("Array resize failed: ") + e.what());
                            }
                    
                            stableCoins[numStableCoins++] = stableCoin;
                            assets[numAssets++] = stableCoin;

                            generalLog.addEntry("Stable added", stableCoin->getName());
                            statsLog.addEntry("Total stables", numStableCoins);
                            
                            cout << "Stable Coin added successfully!\n";
                        }
                        catch (const invalid_argument& e) {
                            cout << "Input Validation Error: " << e.what() << endl;
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        catch (const runtime_error& e) {
                            cout << "System Error: " << e.what() << endl;
                        }
                        catch (const exception& e) {
                            cout << "Unexpected Error: " << e.what() << endl;
                        }
                        catch (...) {
                            cout << "Unknown catastrophic error occurred while adding Stable Coin" << endl;
                        }
                        break;
                    }
                   case 4: {
                        Wallet* wallet = new Wallet();
                        cin >> *wallet;
                        wallets[numWallets++] = wallet;
                        cout << "Wallet added successfully!\n";
                        break;
                    }
                   case 5: {
                    try {
                        int fromIndex, toIndex, cryptoIndex;
                        float amount;
                
                        // Input validation for indices
                        auto read_index = [](const string& prompt, int max) -> int {
                            cout << prompt;
                            int index;
                            if (!(cin >> index) || index < 0 || index >= max) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                throw out_of_range("Invalid index");
                            }
                            return index;
                        };
                
                        fromIndex = read_index("Enter sender wallet index (0-" + to_string(numWallets-1) + "): ", numWallets);
                        toIndex = read_index("Enter receiver wallet index (0-" + to_string(numWallets-1) + "): ", numWallets);
                        cryptoIndex = read_index("Enter cryptocurrency index (0-" + to_string(numCryptos-1) + "): ", numCryptos);
                
                        cout << "Enter amount: ";
                        if (!(cin >> amount) || amount < 0) {
                            if (!cin) {
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                throw invalid_argument("Amount must be a number");
                            }
                            throw invalid_argument("Amount cannot be negative");
                        }
                
                        Transaction* t = nullptr;
                        try {
                            t = new Transaction(wallets[fromIndex], wallets[toIndex], *cryptos[cryptoIndex], amount);
                        } catch (const bad_alloc& e) {
                            throw runtime_error("Failed to allocate memory for transaction");
                        }
                        
                        blockchain.addTransaction(t);
                        wallets[toIndex]->addCrypto(*cryptos[cryptoIndex]);
                        cout << "Transaction added!\n";
                    }
                    catch (const exception& e) {
                        cout << "Transaction Error: " << e.what() << endl;
                    }
                    break;
                }
                   case 6: {
                        cout << "\n=== All Cryptocurrencies ===\n";
                        cout << "Regular Cryptocurrencies:\n";
                        for (int i = 0; i < numCryptos; i++) {
                            cout << i << ". " << *cryptos[i] << endl;
                        }
                        cout << "\nUtility Coins:\n";   \
                        int nr = sizeof(utilityCoins);
                        for (int i = 0; i < numUtilityCoins; i++) {
                            utilityCoins[i]->displayInfo();
                            cout << endl;
                        }
                    
                        cout << "\nStable Coins:\n";
                        for (int i = 0; i < numStableCoins; i++) {
                            stableCoins[i]->displayInfo();
                            cout << endl;
                        }
                        break;
                    }
                   case 7: {
                        cout << "\n=== All Wallets ===\n";
                        for (int i = 0; i < numWallets; i++) {
                            cout << "Wallet " << i << ":\n" << *wallets[i] << endl;
                        }
                        break;
                    }
                   case 8: { // View Blockchain Transactions
                        cout << blockchain;
                        break;
                    }
                   case 9: {
                        int index;
                        char name[100], symbol[100];
                        float price;
                        long long marketCap;
                        char currencyType;
                    
                        cout << "Enter cryptocurrency index to update (0-" << numCryptos - 1 << "): ";
                        cin >> index;
                    
                        if (index >= 0 && index < numCryptos) {
                            cout << "Enter new name (current: " << cryptos[index]->getName() << "): ";
                            cin >> name;
                            cout << "Enter new symbol (current: " << cryptos[index]->getSymbol() << "): ";
                            cin >> symbol;
                            cout << "Enter new price (current: " << cryptos[index]->getPrice() << "): ";
                            cin >> price;
                            cout << "Enter new market cap: ";
                            cin >> marketCap;
                            cout << "Enter new currency type: ";
                            cin >> currencyType;
                    
                            cryptos[index]->setName(name);
                            cryptos[index]->setSymbol(symbol);
                            cryptos[index]->setPrice(price);
                            cryptos[index]->setMarketCap(marketCap);
                            cryptos[index]->setCurrencyType(currencyType);
                            generalLog.addEntry("Crypto Updated",cryptos[index]->getName());
                            cout << "Cryptocurrency updated successfully!\n";
                        } else {
                            cout << "Invalid index!\n";
                        }
                        break;
                    }
                   case 10: {
                        int index;
                        cout << "Enter cryptocurrency index to delete (0-" << numCryptos - 1 << "): ";
                        cin >> index;
                    
                        if (index >= 0 && index < numCryptos) {
                           // generalLog.addEntry("Crypto Deleted",cryptos[index]->getName());
                            delete cryptos[index]; 
                            for (int i = index; i < numCryptos - 1; i++) {
                                cryptos[i] = cryptos[i + 1];
                            }
                            numCryptos--;
                            cout << "Cryptocurrency deleted successfully!\n";
                        } else {
                            cout << "Invalid index!\n";
                        }
                        break;
                    }
                   case 11: {
                        int index;
                        char owner[100];
                        char activeStatus;
                    
                        cout << "Enter wallet index to update (0-" << numWallets - 1 << "): ";
                        cin >> index;
                    
                        if (index >= 0 && index < numWallets) {
                            cout << "Enter new owner name (current: " << wallets[index]->getOwner() << "): ";
                            cin >> owner;
                            cout << "Is the wallet active? (y/n): ";
                            cin >> activeStatus;
                    
                            wallets[index]->setOwner(owner);
                            wallets[index]->setIsActive(activeStatus == 'y' || activeStatus == 'Y');
                            cout << "Wallet updated successfully!\n";
                        } else {
                            cout << "Invalid index!\n";
                        }
                        break;
                    }
                   case 12: {
                        int index;
                        cout << "Enter wallet index to delete (0-" << numWallets - 1 << "): ";
                        cin >> index;
                    
                        if (index >= 0 && index < numWallets) {
                            delete wallets[index]; 
                            for (int i = index; i < numWallets - 1; i++) {
                                wallets[i] = wallets[i + 1];
                            }
                            numWallets--;
                            cout << "Wallet deleted successfully!\n";
                        } else {
                            cout << "Invalid index!\n";
                        }
                        break;
                    }
                   case 13: { // View Wallet Details
                        int index;
                        cout << "Enter wallet index to view (0-" << numWallets - 1 << "): ";
                        cin >> index;
                        
                        if (index >= 0 && index < numWallets) {
                            cout << "\n=== Wallet Details ===\n";
                            cout << wallets[index];
                        } else {
                            cout << "Invalid index!\n";
                        }
                        break;
                    }
                   case 14: { // View Transaction Details
                        int index;
                        cout << "Enter transaction index to view (0-" << blockchain.getNumTransactions() - 1 << "): ";
                        cin >> index;
                        
                        if (index >= 0 && index < blockchain.getNumTransactions()) {
                            cout << "\n=== Transaction Details ===\n";
                            cout << *blockchain[index] << endl;
                        } else {
                            cout << "Invalid index!\n";
                        }
                        break;
                    }
                   case 15: { // Delete Transaction
                        int index;
                        cout << "Enter transaction index to delete (0-" << blockchain.getNumTransactions() - 1 << "): ";
                        cin >> index;
                        
                        if (index >= 0 && index < blockchain.getNumTransactions()) {
                            Transaction** transactions = blockchain.getTransactions();
                            delete transactions[index];
                            
                            for (int i = index; i < blockchain.getNumTransactions() - 1; i++) {
                                transactions[i] = transactions[i + 1];
                            }
                            
                            blockchain.setNumTransactions(blockchain.getNumTransactions() - 1);
                            cout << "Transaction deleted successfully!\n";
                        } else {
                            cout << "Invalid index!\n";
                        }
                        break;
                    }
                   case 16: { // Compare Two Cryptos
                        int index1, index2;
                        cout << "\nAvailable cryptocurrencies:\n";
                        for (int i = 0; i < numCryptos; i++) {
                            cout << i << ". " << cryptos[i]->getName() << " (" << cryptos[i]->getSymbol() << ")\n";
                        }
                        
                        cout << "Enter first cryptocurrency index (0-" << numCryptos - 1 << "): ";
                        cin >> index1;
                        cout << "Enter second cryptocurrency index (0-" << numCryptos - 1 << "): ";
                        cin >> index2;
                        
                        if (index1 >= 0 && index1 < numCryptos && index2 >= 0 && index2 < numCryptos) {
                    bool isEqual = *cryptos[index1] == *cryptos[index2];
                    bool isLess = *cryptos[index1] < *cryptos[index2];
                            
                            cout << "\nComparison Results:\n";
                            if (isEqual) {
                                cout << "The cryptocurrencies are equal.\n";
                            } else {
                                cout << "The cryptocurrencies are not equal.\n";
                                if (isLess) {
                                    cout << cryptos[index1]->getName() << " has a smaller market cap than " 
                                         << cryptos[index2]->getName() << endl;
                                } else {
                                    cout << cryptos[index1]->getName() << " has a larger market cap than " 
                                         << cryptos[index2]->getName() << endl;
                                }
                            }
                        } else {
                            cout << "Invalid indices!\n";
                        }
                        break;
                    }
                   case 17: { // Increment Crypto Price
                        cout << "\nAvailable cryptocurrencies:\n";
                        for (int i = 0; i < numCryptos; i++) {
                            cout << i << ". " << cryptos[i]->getName() << " (" << cryptos[i]->getSymbol() << ")\n";
                        }
                        
                        int index;
                        cout << "Enter cryptocurrency index to increment price (0-" << numCryptos - 1 << "): ";
                        cin >> index;
                        
                        if (index >= 0 && index < numCryptos) {
                            cout << "Current price: " << cryptos[index]->getPrice() << endl;
                            ++(*cryptos[index]); 
                            cout << "New price after pre-increment: " << cryptos[index]->getPrice() << endl;
                            
                            (*cryptos[index])++; 
                            cout << "New price after post-increment: " << cryptos[index]->getPrice() << endl;
                            generalLog.addEntry("Crypto price updated", cryptos[index]->getName());
                        } else {
                            cout << "Invalid index!\n";
                        }
                        break;
                    }
                   case 18: { // Subtract Two Cryptos
                        cout << "\nAvailable cryptocurrencies:\n";
                        for (int i = 0; i < numCryptos; i++) {
                            cout << i << ". " << cryptos[i]->getName() << " (" << cryptos[i]->getSymbol() << ")\n";
                        }
                    
                        int index1, index2;
                        cout << "Enter first cryptocurrency index (0-" << numCryptos - 1 << "): ";
                        cin >> index1;
                        cout << "Enter second cryptocurrency index (0-" << numCryptos - 1 << "): ";
                        cin >> index2;
                    
                        if (index1 >= 0 && index1 < numCryptos && index2 >= 0 && index2 < numCryptos) {
                            CryptoCurrency result = *cryptos[index1] - *cryptos[index2];
                            cout << "\nSubtraction Result:\n";
                            cout << result << endl;
                            cout << "Price of result: " << result.getPrice() << endl;
                            cout << "Market Cap of result: " << result.getMarketCap() << endl;
                        } else {
                            cout << "Invalid indices!\n";
                        }
                        break;
                    }
                   case 19: {
                        int walletIndex, cryptoIndex;
                        cout << "Enter wallet index: ";
                        cin >> walletIndex;
                        try {
                            if (walletIndex < 0 || walletIndex >= numWallets) throw std::out_of_range("Invalid wallet index.");
                            Wallet* wallet = wallets[walletIndex];
                            cout << "Enter crypto index: ";
                            cin >> cryptoIndex;
                            try {
                                CryptoCurrency& crypto = (*wallet)[cryptoIndex];
                                cout << "Crypto: " << crypto << endl;
                            } catch (const std::out_of_range& e) {
                                cout << "Crypto error: " << e.what() << endl;
                            }
                        } catch (const std::out_of_range& e) {
                            cout << "Wallet error: " << e.what() << endl;
                        }
                        break;
                    }
                   case 20: { // Add Crypto to Wallet
                        int walletIndex, cryptoIndex;
                        cout << "Enter wallet index (0-" << numWallets - 1 << "): ";
                        cin >> walletIndex;
                        cout << "Enter cryptocurrency index (0-" << numCryptos - 1 << "): ";
                        cin >> cryptoIndex;
                        
                        if (walletIndex >= 0 && walletIndex < numWallets && cryptoIndex >= 0 && cryptoIndex < numCryptos) {
                            wallets[walletIndex]->addCrypto(*cryptos[cryptoIndex]);
                            cout << "Cryptocurrency added to wallet successfully!\n";
                            
                        } else {
                            cout << "Invalid indices!\n";
                        }
                        break;
                    }
                   case 21: { // Show Polymorphism Demo
                        if (numAssets > 0) {
                            demonstratePolymorphism(assets, numAssets);
                        } else {
                            cout << "No assets available for demonstration. Please add some cryptocurrencies first.\n";
                        }
                        break;
                    }
                   case 22: { // Demonstrate Diamond Inheritance
                        char name[100], symbol[100];
                        float price;
                        long long marketCap;
                        char currencyType;
                        
                        cout << "\n=== Diamond Inheritance Demonstration ===\n";
                        cout << "This will create a CryptoCurrency that inherits from both Token and MarketEntity\n\n";
                        
                        cout << "Enter cryptocurrency name: ";
                        cin >> name;
                        cout << "Enter symbol: ";
                        cin >> symbol;
                        cout << "Enter price: ";
                        cin >> price;
                        cout << "Enter market cap: ";
                        cin >> marketCap;
                        cout << "Enter currency type: ";
                        cin >> currencyType;
                        
                        CryptoCurrency crypto(name, symbol, price, marketCap, currencyType);
                        
                        cout << "\nCryptoCurrency object created with diamond inheritance!\n";
                        
                        cout << "\n--- Accessing methods from Token class (first parent) ---\n";
                        cout << "Name (from Token): " << crypto.getName() << endl;
                        cout << "Symbol (from Token): " << crypto.getSymbol() << endl;
                        cout << "Price (from Token): " << crypto.getPrice() << endl;
                        
                        cout << "\n--- Accessing methods from MarketEntity class (second parent) ---\n";
                        cout << "Market Cap (from MarketEntity): " << crypto.getMarketCap() << endl;
                        cout << "Currency Type (from MarketEntity): " << crypto.getCurrencyType() << endl;
                        
                        cout << "\n--- Methods that show resolution of the diamond inheritance ---\n";
                        cout << "Calling displayInfo() method:\n";
                        crypto.displayInfo();
                        
                        cout << "\nCalling displayMarketInfo() method:\n";
                        crypto.displayMarketInfo();
                        
                        cout << "\nDiamond inheritance demonstration complete.\n";
                        break;
                    }
                   case 23: { // Exit
                        cout << "\n=== General Audit Log ===\n";
                         generalLog.displayLog([](const std::string& timestamp, 
                                              const std::string& action, 
                                              const std::string& details) {
                            return "[" + timestamp + "] " + action + ": " + details;
                        });
                        
                        cout << "\n=== Statistics Log ===\n";
                       statsLog.displayLog([](const std::string& timestamp, 
                                             const std::string& action, 
                                             int details) {
                            std::ostringstream oss;
                            oss << details;
                            return "[" + timestamp + "] " + action + " count: " + oss.str();
                        });
                    
                        break;
                    }
                   case 24: { 
                    cout << "Thank you for using Coinqueror! Goodbye!\n";
                    break;
                }
                   default: {
    cout << "Invalid choice! Please try again.\n";
    break;
}    
                }
            }while(choice!=24);
            for (int i = 0; i < numCryptos; i++) delete cryptos[i];
            for (int i = 0; i < numUtilityCoins; i++) delete utilityCoins[i];
            for (int i = 0; i < numStableCoins; i++) delete stableCoins[i];
            for (int i = 0; i < numWallets; i++) delete wallets[i];
            for (int i = 0; i < numAssets; i++) delete assets[i];
            delete[] cryptos;
            delete[] utilityCoins;
            delete[] stableCoins;
            delete[] assets;
            delete[] wallets;
        }
};

Meniu* Meniu::instance = nullptr;

int main() {
    Meniu::getInstance()->ruleaza();
return 0;}