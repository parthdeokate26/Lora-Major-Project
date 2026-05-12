// Auto-Sender Configuration for Automated Dataset Collection
// Toggle this to enable/disable automatic message sending

#ifndef AUTO_SENDER_H
#define AUTO_SENDER_H

// ============ TOGGLE AUTO-SEND FUNCTIONALITY HERE ============
#define ENABLE_AUTO_SEND 0  // Set to 1 to enable, 0 to disable
// =============================================================

#define AUTO_SEND_INTERVAL 5000  // 5 seconds between transmissions

// Payload sizes to cycle through
const int AUTO_SEND_PAYLOAD_SIZES[] = {25, 50, 75, 100};
const int AUTO_SEND_NUM_SIZES = 4;

// Data types to cycle through
const char* AUTO_SEND_DATA_TYPES[] = {"text", "random", "ones", "zeros"};
const int AUTO_SEND_NUM_TYPES = 4;

// Auto-Sender State Structure
struct AutoSenderState {
    bool enabled;
    unsigned long lastSendTime;
    int currentPayloadIndex;
    int currentTypeIndex;
    int transmissionCount;
};

// Global auto-sender state
AutoSenderState autoSender = {
    ENABLE_AUTO_SEND,
    0,
    0,
    0,
    0
};

// Initialize auto-sender
void initAutoSender() {
    if (autoSender.enabled) {
        autoSender.lastSendTime = millis();
        Serial.println("Auto-Sender initialized. Will send messages every 5 seconds.");
        Serial.println("Cycling through: 4 payload sizes × 4 data types");
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
    // Move to next type
    autoSender.currentTypeIndex++;
    
    // If we've cycled through all types, move to next payload size
    if (autoSender.currentTypeIndex >= AUTO_SEND_NUM_TYPES) {
        autoSender.currentTypeIndex = 0;
        autoSender.currentPayloadIndex++;
        
        // If we've cycled through all payload sizes, loop back
        if (autoSender.currentPayloadIndex >= AUTO_SEND_NUM_SIZES) {
            autoSender.currentPayloadIndex = 0;
        }
    }
    
    autoSender.transmissionCount++;
    Serial.println("Auto-Send cycle: " + String(autoSender.transmissionCount) + 
                   " | Payload: " + String(AUTO_SEND_PAYLOAD_SIZES[autoSender.currentPayloadIndex]) + 
                   " | Type: " + AUTO_SEND_DATA_TYPES[autoSender.currentTypeIndex]);
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
    info += "Count: " + String(autoSender.transmissionCount);
    return info;
}

#endif // AUTO_SENDER_H
