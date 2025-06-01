# 🪙 Coinqueror
### Advanced Cryptocurrency Management Platform in C++

> A comprehensive C++ application that simulates a cryptocurrency management platform with enterprise-grade features, demonstrating advanced OOP concepts and real-world blockchain functionality.

## 🌟 Overview

**Coinqueror** is a sophisticated cryptocurrency simulation platform that combines the functionality of major exchanges like Binance with enhanced features for portfolio management, blockchain tracking, and real-time auditing. Built with modern C++ principles, it showcases advanced object-oriented programming concepts while providing a robust digital asset ecosystem.

## ✨ Key Features

### 🔗 **Digital Asset Management**
- **Hierarchical Architecture**: Sophisticated class hierarchy (`DigitalAsset` → `Token`/`MarketEntity` → `CryptoCurrency`/`UtilityCoin`/`StableCoin`)
- **Polymorphic Behavior**: Dynamic method dispatch via virtual functions (`displayInfo()`, `calculateMarketValue()`)
- **Diamond Inheritance Resolution**: Clean implementation of multiple inheritance patterns

### 💰 **Wallet & Blockchain Integration**
- **Dynamic Portfolio Management**: Expandable wallet system with automatic resizing
- **Transaction Validation**: Secure crypto transfers with comprehensive validation
- **Blockchain Tracking**: Immutable transaction history between wallets

### 📊 **Real-time Audit System**
- **Templated Logging**: Generic `AuditLog<T>` supports multiple data types
- **Timestamp Tracking**: Automated action logging with precise timestamps
- **Custom Formatters**: Human-readable log output with configurable formats

### 💾 **Data Persistence**
- **File I/O Operations**: Seamless load/save functionality for all data types
- **Dynamic Memory Management**: Efficient allocation and resizing for scalability
- **Multiple Data Sources**: Support for `cryptos.txt`, `wallets.txt`, `transactions.txt`

### 🖥️ **Interactive User Interface**
- **Singleton Menu System**: Clean CLI interface with 24+ operations
- **CRUD Operations**: Complete Create, Read, Update, Delete functionality
- **Comparison Tools**: Advanced asset comparison and analysis features

## 🏗️ Architecture Highlights

### Class Hierarchy
```cpp
DigitalAsset (Abstract Base)
├── Token (Symbol Management)
├── MarketEntity (Market Cap Tracking)
└── CryptoCurrency (Diamond Inheritance)
    ├── UtilityCoin (Specialized Use Cases)
    └── StableCoin (Asset-Backed Stability)
```

### Core Components

| Component | Description | Key Features |
|-----------|-------------|--------------|
| **DigitalAsset** | Abstract base class for all crypto assets | Pure virtual methods, polymorphic interface |
| **CryptoCurrency** | Main crypto implementation | Diamond inheritance resolution, market integration |
| **Wallet** | Portfolio management system | Dynamic expansion, owner metadata |
| **Blockchain** | Transaction ledger | Immutable history, validation system |
| **AuditLog\<T>** | Templated logging system | Type-safe logging, file persistence |

## 🚀 Getting Started

### Prerequisites
- C++17 compatible compiler (GCC 7.0+, Clang 5.0+, MSVC 2017+)
- Standard C++ library

### Quick Start
```bash
# Clone the repository
git clone https://github.com/yourusername/coinqueror.git
cd coinqueror

# Compile with C++17 support
g++ -std=c++17 project.cpp -o coinqueror

# Run the application
./coinqueror
```

### Optional Data Files
Place these files in your working directory to preload sample data:
```
cryptos.txt      # Sample cryptocurrency definitions
wallets.txt      # Pre-configured wallet data
transactions.txt # Historical transaction records
```

## 🎯 Advanced C++ Features Demonstrated

### Object-Oriented Programming
- ✅ **Multiple Inheritance** with diamond problem resolution
- ✅ **Virtual Functions** and pure virtual methods
- ✅ **Polymorphism** through base class pointers
- ✅ **Encapsulation** with proper access control

### Modern C++ Techniques
- ✅ **Operator Overloading** (`+`, `-`, `++`, `[]`, etc.)
- ✅ **Template Programming** with generic classes
- ✅ **Exception Handling** with custom exception types
- ✅ **RAII Principles** for resource management
- ✅ **Smart Pointers** for memory safety

### System Design Patterns
- ✅ **Singleton Pattern** for menu management
- ✅ **Factory Pattern** for object creation
- ✅ **Observer Pattern** for audit logging

## 💡 Usage Examples

### Creating and Managing Cryptocurrencies
```cpp
// Polymorphic asset creation
auto bitcoin = std::make_unique<CryptoCurrency>("Bitcoin", 45000.0, "BTC", 850000000000);
auto ethereum = std::make_unique<UtilityCoin>("Ethereum", 3200.0, "ETH", "Smart Contracts");

// Operator overloading in action
CryptoCurrency combined = *bitcoin + *ethereum;  // Combines market values
++wallet;  // Increases wallet capacity
```

### Wallet Operations
```cpp
Wallet userWallet("Alice", 10);
userWallet.addCryptocurrency(std::move(bitcoin));
userWallet.displayPortfolio();  // Polymorphic display
```

### Blockchain Transactions
```cpp
Transaction tx(wallet1, wallet2, bitcoin.get(), 0.5);
blockchain.addTransaction(tx);  // Validated and recorded
```

## 📁 Project Structure

```
coinqueror/
├── project.cpp              # Main implementation file
├── cryptos.txt              # Sample cryptocurrency data
├── wallets.txt              # Sample wallet configurations
├── transactions.txt         # Sample transaction history
├── audit_general.log        # Generated audit log
├── audit_stats.log          # Statistical audit data
└── README.md               # This file
```

## 🔍 Feature Demonstrations

### 1. **Polymorphism in Action**
Unified interface handling diverse asset types through base class pointers:
```cpp
std::vector<std::unique_ptr<DigitalAsset>> assets;
// All assets can be managed uniformly despite different implementations
```

### 2. **Diamond Inheritance Resolution**
`CryptoCurrency` cleanly inherits from both `Token` and `MarketEntity`:
```cpp
class CryptoCurrency : public Token, public MarketEntity {
    // Resolves potential ambiguity through virtual inheritance
};
```

### 3. **Template Specialization**
Generic audit logging supports multiple data types:
```cpp
AuditLog<std::string> actionLog;  // For general actions
AuditLog<int> statsLog;           // For numerical statistics
```

### 4. **Exception Safety**
Comprehensive error handling throughout the application:
```cpp
try {
    wallet.addCryptocurrency(invalidCrypto);
} catch (const std::invalid_argument& e) {
    std::cerr << "Validation failed: " << e.what() << std::endl;
}
```

## 🎮 Interactive Menu System

The application provides 24+ interactive options including:
- 🔧 **CRUD Operations**: Create, modify, and delete assets
- 📈 **Market Analysis**: Compare cryptocurrencies and market trends
- 💼 **Portfolio Management**: Manage multiple wallets and holdings
- 🔍 **Transaction History**: View and analyze blockchain transactions
- 📋 **Audit Reports**: Generate comprehensive system logs


## 🏆 Technical Achievements

- **Zero Memory Leaks**: Proper RAII and smart pointer usage
- **Exception Safe**: Comprehensive error handling throughout
- **Scalable Design**: Dynamic memory management and resizing
- **Type Safety**: Template-based generic programming
- **Clean Architecture**: SOLID principles implementation

---

<div align="center">

**Built with ❤️ and modern C++**

</div>
