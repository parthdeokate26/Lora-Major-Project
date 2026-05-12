// Auto-Sender Configuration for Automated Dataset Collection
// Toggle this to enable/disable automatic message sending

#ifndef AUTO_SENDER_H
#define AUTO_SENDER_H

// ============ TOGGLE AUTO-SEND FUNCTIONALITY HERE ============
#define ENABLE_AUTO_SEND 0  // Set to 1 to enable, 0 to disable
// =============================================================

#define AUTO_SEND_INTERVAL 2500  // 2.5 seconds between transmissions

// Payload sizes to cycle through
const int AUTO_SEND_PAYLOAD_SIZES[] = {25, 50, 75};
const int AUTO_SEND_NUM_SIZES = 3;

// Data types to cycle through
const char* AUTO_SEND_DATA_TYPES[] = {"text", "random", "ones", "zeros"};
const int AUTO_SEND_NUM_TYPES = 4;

// LoRa parameter sweep settings
const int AUTO_SEND_SF_OPTIONS[] = {7, 8, 9, 10, 11, 12};
const int AUTO_SEND_NUM_SF = 6;

const long AUTO_SEND_BW_OPTIONS[] = {125000, 250000, 500000};
const int AUTO_SEND_NUM_BW = 3;

const int AUTO_SEND_CR_OPTIONS[] = {5, 6, 7, 8};
const int AUTO_SEND_NUM_CR = 4;

// Auto-Sender State Structure
struct AutoSenderState {
    bool enabled;
    unsigned long lastSendTime;
    int currentPayloadIndex;
    int currentTypeIndex;
    int currentSfIndex;
    int currentBwIndex;
    int currentCrIndex;
    int transmissionCount;
};

// Global auto-sender state
AutoSenderState autoSender = {
    ENABLE_AUTO_SEND,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

// Initialize auto-sender
void initAutoSender() {
    if (autoSender.enabled) {
        autoSender.lastSendTime = millis();
        Serial.println("Auto-Sender initialized. Will send messages every 2.5 seconds.");
        Serial.println("Cycling through: 3 payload sizes × 4 data types × 6 SF × 3 BW × 4 CR");
    }
}

// Get next auto-send message
String getAutoSendMessage(String& outType, int& outPayloadSize) {
    // Get current payload size
    outPayloadSize = AUTO_SEND_PAYLOAD_SIZES[autoSender.currentPayloadIndex];
    
    // Get current data type
    outType = AUTO_SEND_DATA_TYPES[autoSender.currentTypeIndex];
    
    // Generate payload based on type
    String payload = "";
    
    if (outType == "text") {
        while (payload.length() < outPayloadSize) {
            payload += "DataSet_";
        }
        payload = payload.substring(0, outPayloadSize);
    }
    else if (outType == "random") {
        payload = "RAND:";
        while (payload.length() < outPayloadSize) {
            payload += String(random(256), HEX);
        }
        payload = payload.substring(0, outPayloadSize);
    }
    else if (outType == "ones") {
        payload = "ONES:";
        while (payload.length() < outPayloadSize) {
            payload += "1";
        }
        payload = payload.substring(0, outPayloadSize);
    }
    else if (outType == "zeros") {
        payload = "ZEROS:";
        while (payload.length() < outPayloadSize) {
            payload += "0";
        }
        payload = payload.substring(0, outPayloadSize);
    }
    
    return payload;
}

// Cycle to next message configuration
void cycleAutoSendConfig() {
    autoSender.currentTypeIndex++;
    if (autoSender.currentTypeIndex >= AUTO_SEND_NUM_TYPES) {
        autoSender.currentTypeIndex = 0;
        autoSender.currentPayloadIndex++;

        if (autoSender.currentPayloadIndex >= AUTO_SEND_NUM_SIZES) {
            autoSender.currentPayloadIndex = 0;
            autoSender.currentCrIndex++;

            if (autoSender.currentCrIndex >= AUTO_SEND_NUM_CR) {
                autoSender.currentCrIndex = 0;
                autoSender.currentBwIndex++;

                if (autoSender.currentBwIndex >= AUTO_SEND_NUM_BW) {
                    autoSender.currentBwIndex = 0;
                    autoSender.currentSfIndex++;

                    if (autoSender.currentSfIndex >= AUTO_SEND_NUM_SF) {
                        autoSender.currentSfIndex = 0;
                    }
                }
            }
        }
    }

    autoSender.transmissionCount++;
    Serial.println("Auto-Send cycle: " + String(autoSender.transmissionCount) +
                   " | Payload: " + String(AUTO_SEND_PAYLOAD_SIZES[autoSender.currentPayloadIndex]) +
                   " | Type: " + AUTO_SEND_DATA_TYPES[autoSender.currentTypeIndex] +
                   " | SF: " + String(AUTO_SEND_SF_OPTIONS[autoSender.currentSfIndex]) +
                   " | BW: " + String(AUTO_SEND_BW_OPTIONS[autoSender.currentBwIndex] / 1000) + "kHz" +
                   " | CR: " + String(AUTO_SEND_CR_OPTIONS[autoSender.currentCrIndex]));
}

// Check if it's time to send the next auto-message
bool shouldSendAutoMessage() {
    if (!autoSender.enabled) return false;
    
    unsigned long currentTime = millis();
    if (currentTime - autoSender.lastSendTime >= AUTO_SEND_INTERVAL) {
        autoSender.lastSendTime = currentTime;
        return true;
    }
    
    return false;
}

// Get current auto-send configuration info
String getAutoSendInfo() {
    String info = "AutoSend: ";
    info += autoSender.enabled ? "ENABLED | " : "DISABLED | ";
    info += "Payload: " + String(AUTO_SEND_PAYLOAD_SIZES[autoSender.currentPayloadIndex]) + " bytes | ";
    info += "Type: " + String(AUTO_SEND_DATA_TYPES[autoSender.currentTypeIndex]) + " | ";
    info += "SF: " + String(AUTO_SEND_SF_OPTIONS[autoSender.currentSfIndex]) + " | ";
    info += "BW: " + String(AUTO_SEND_BW_OPTIONS[autoSender.currentBwIndex] / 1000) + " kHz | ";
    info += "CR: " + String(AUTO_SEND_CR_OPTIONS[autoSender.currentCrIndex]) + " | ";
    info += "Count: " + String(autoSender.transmissionCount);
    return info;
}

#endif // AUTO_SENDER_H
