# LoRa Optimization Framework

A Smart Framework for optimizing LoRa data transmission through data compression, adaptive parameter selection, and performance benchmarking.

## Project Overview

This project implements an intelligent system for optimizing LoRa data transmission by:
- Implementing data type and pattern-based payload compression
- Dynamically adjusting transmission parameters based on performance results
- Providing comprehensive benchmarking between standard and enhanced transmission methods
- Visualizing performance metrics through a web dashboard

## Repository Structure

- **Sender-Standard**: Standard LoRa sender implementation
- **Sender-Enhanced**: Enhanced LoRa sender with Smart Framework optimization
- **Receiver**: LoRa receiver that responds to both standard and enhanced senders
- **webserver**: Flask web application for data collection and visualization

## Getting Started

1. Upload the appropriate code to your LoRa devices:
   - `Sender-Standard/sender-s.ino` to the standard sender
   - `Sender-Enhanced/sender-e.ino` to the enhanced sender
   - `Receiver/receiver.ino` to the receiver

2. Start the web server:
   ```bash
   cd webserver
   python3 app.py
   ```

3. Access the dashboard at http://localhost:8000

## Data Export

To export transmission data to CSV:
```bash
cd webserver
python3 setup_db.py --extract filename.csv
```

## Automatic Message Sending (Auto-Send)

The enhanced sender includes an auto-send feature that automatically sends different payload sizes and data types every 5 seconds.

### Enable/Disable Auto-Send

The feature is controlled by a toggle at the top of `auto_sender.h`:

**File**: `Sender-Enhanced/sender-e/auto_sender.h`
```cpp
#define ENABLE_AUTO_SEND 1  // Set to 1 to enable, 0 to disable
```

### Auto-Send Configuration

Modify these arrays in `auto_sender.h` to customize what gets sent:

```cpp
// Payload sizes to cycle through (in bytes)
const int AUTO_SEND_PAYLOAD_SIZES[] = {25, 50, 75, 100};

// Data types to cycle through
const char* AUTO_SEND_DATA_TYPES[] = {"text", "random", "ones", "zeros"};

// Interval between transmissions (milliseconds)
#define AUTO_SEND_INTERVAL 5000  // 5 seconds
```

### Serial Commands

Control auto-send via serial monitor:
```
auto_enable      # Enable auto-send
auto_disable     # Disable auto-send
auto_status      # Show current auto-send configuration
```

### How It Works

1. Sender automatically cycles through all combinations of payload sizes and data types
2. Every 5 seconds, a new message is sent (by default: 25 bytes text → 25 bytes random → ... → 100 bytes zeros)
3. Receiver acknowledges each transmission
4. Dashboard updates with received metrics (RSSI, SNR, latency, compression ratio, etc.)
5. All data is stored in database for ML training
6. Export data as CSV: `python3 setup_db.py --extract ml_dataset.csv`

### Example Flow
- **0s**: Send 25 bytes text message
- **5s**: Send 25 bytes random data
- **10s**: Send 25 bytes all ones
- **15s**: Send 25 bytes all zeros
- **20s**: Send 50 bytes text message
- ... (continues cycling through all combinations)

## Hardware Configuration

The project uses LILYGO LoRa32 T3_V1.6.1 and T3_V1.0 boards with the following pin configurations:

### LoRa (SX1276/868/915)
- MOSI: 27
- SCLK: 5
- CS: 18
- DIO: 26
- RST: 14
