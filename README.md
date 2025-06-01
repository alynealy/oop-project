# Coinqueror
Coinqueror is a comprehensive C++ application that simulates a cryptocurrency management platform, offering functionalities similar to Binance but with enhanced features. The project demonstrates advanced object-oriented programming (OOP) concepts, including polymorphism, inheritance (diamond inheritance resolution), templates, operator overloading, and file I/O. It integrates real-time auditing, blockchain transaction tracking, and wallet management to provide a robust digital asset ecosystem.

Digital Asset Management
  Hierarchical classes (DigitalAsset, Token, MarketEntity, CryptoCurrency, UtilityCoin, StableCoin) model cryptocurrencies with distinct properties.
  Polymorphic behavior via virtual functions (displayInfo(), calculateMarketValue()).
  Diamond inheritance resolved for CryptoCurrency (combining Token and MarketEntity).

Wallet & Blockchain Integration
  Wallet class manages user portfolios and supports dynamic expansion.
  Blockchain class tracks transactions between wallets.
  Transaction class handles crypto transfers with validation.

Audit Logging
  Templated AuditLog records actions with timestamps (e.g., crypto additions, price updates).
  Supports custom formatters for human-readable logs.

Data Persistence
  Loads/saves data from/to files (cryptos.txt, wallets.txt, transactions.txt).
  Handles dynamic memory allocation and resizing for scalability.

User Interface
  Singleton Meniu class provides an interactive CLI menu.
  24+ options for CRUD operations, comparisons, and demonstrations.

Advanced OOP & C++ Features
  Operator overloading (+, -, ++, [], etc.) for intuitive object interactions.
  Exception handling for invalid inputs/memory errors.
  Smart pointers and RAII for resource management.

How to Run:
Compile: g++ -std=c++17 project.cpp -o coinqueror
Execute: ./coinqueror
Input Files (Optional):Place cryptos.txt, wallets.txt, or transactions.txt in the working directory to preload data.

Class Structure Highlights:
DigitalAsset:	   Base class for all assets (name, price). Pure virtual methods enforce polymorphism.
CryptoCurrency:	 Combines Token (symbol) and MarketEntity (market cap). Resolves diamond inheritance.
UtilityCoin:	   Specialized crypto with use cases (e.g., decentralized storage).
StableCoin:	     Asset-backed crypto (e.g., USD).
Wallet:	         Manages owned cryptocurrencies and owner metadata. Supports dynamic resizing.
Blockchain:	     Stores validated transactions between wallets.
AuditLog<T>:	   Tracks actions (string) or stats (int) with timestamps. Writes to log files.

Demonstration Scenarios:
Polymorphism:         Unified interface for diverse assets (CryptoCurrency, UtilityCoin, etc.).
Diamond Inheritance:  CryptoCurrency inherits from both Token and MarketEntity without ambiguity.
Operator Overloading: Natural syntax for operations (e.g., crypto1 + crypto2, ++walletCapacity).
Error Handling:       Validates inputs (e.g., negative prices) and throws descriptive exceptions.

File Structure:
project.cpp: Main implementation (all classes and driver code).
cryptos.txt/wallets.txt/transactions.txt: Sample data files (optional).
audit_general.log/audit_stats.log: Auto-generated audit logs.



