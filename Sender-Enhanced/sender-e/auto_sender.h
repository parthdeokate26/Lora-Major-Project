// Auto-Sender Configuration for Automated Dataset Collection
// Enhanced with SF/BW/CR parameter sweeping for ML dataset generation

#ifndef AUTO_SENDER_H
#define AUTO_SENDER_H

// ============ TOGGLE AUTO-SEND FUNCTIONALITY HERE ============
#define ENABLE_AUTO_SEND 1   // Set to 1 to enable, 0 to disable
// =============================================================

#define AUTO_SEND_INTERVAL 5000  // 5 seconds between transmissions

// ================= PAYLOAD CONFIG =================

// Payload sizes to cycle through (reduced for faster generation)
const int AUTO_SEND_PAYLOAD_SIZES[] = {25, 75};
const int AUTO_SEND_NUM_SIZES = 2;

// Data types to cycle through (reduced)
const char* AUTO_SEND_DATA_TYPES[] = {"text", "random"};
const int AUTO_SEND_NUM_TYPES = 2;

// ================= LORA PARAMETER SWEEP =================

// SF Sweep (reduced: 7, 9, 11 for diverse SF values)
const int SF_VALUES[] = {7, 9, 11};
const int NUM_SF = 3;

// BW Sweep (125kHz and 500kHz for diversity, fixing typo 25000->removed)
const long BW_VALUES[] = {125000, 500000};
const int NUM_BW = 2;

// CR Sweep (reduced: 5, 6, 8)
const int CR_VALUES[] = {5, 6, 8};
const int NUM_CR = 3;

// ================= AUTO-SENDER STATE =================

struct AutoSenderState {

    bool enabled;

    unsigned long lastSendTime;

    // Payload cycling
    int currentPayloadIndex;
    int currentTypeIndex;

    // LoRa parameter sweeping
    int currentSFIndex;
    int currentBWIndex;
    int currentCRIndex;

    int transmissionCount;
};

// ================= GLOBAL STATE =================

AutoSenderState autoSender = {

    ENABLE_AUTO_SEND,

    0,

    // Payload indexes
    0,
    0,

    // Parameter indexes
    0,
    0,
    0,

    0
};

// ================= INITIALIZATION =================

void initAutoSender() {

    if (autoSender.enabled) {

        autoSender.lastSendTime = millis();

        Serial.println("========================================");
        Serial.println("AUTO-SENDER ENABLED");
        Serial.println("ML Dataset Collection Started");
        Serial.println("========================================");

        Serial.println("Payload Sizes:");
        for (int i = 0; i < AUTO_SEND_NUM_SIZES; i++) {
            Serial.println(String(AUTO_SEND_PAYLOAD_SIZES[i]) + " bytes");
        }

        Serial.println("Data Types:");
        for (int i = 0; i < AUTO_SEND_NUM_TYPES; i++) {
            Serial.println(AUTO_SEND_DATA_TYPES[i]);
        }

        Serial.println("LoRa Parameter Sweeping:");
        Serial.println("SF: 7 → 12");
        Serial.println("BW: 125k → 500k");
        Serial.println("CR: 4/5 → 4/8");

        int totalCombinations =
            AUTO_SEND_NUM_SIZES *
            AUTO_SEND_NUM_TYPES *
            NUM_SF *
            NUM_BW *
            NUM_CR;

        Serial.println("Total Sweep Combinations: " + String(totalCombinations));

        Serial.println("========================================");
    }
}

// ================= GENERATE PAYLOAD =================

String getAutoSendMessage(String& outType, int& outPayloadSize) {

    // Current payload size
    outPayloadSize =
        AUTO_SEND_PAYLOAD_SIZES[
            autoSender.currentPayloadIndex
        ];

    // Current type
    outType =
        AUTO_SEND_DATA_TYPES[
            autoSender.currentTypeIndex
        ];

    String payload = "";

    // ============ TEXT ============
    if (outType == "text") {

        while (payload.length() < outPayloadSize) {
            payload += "DataSet_";
        }

        payload =
            payload.substring(0, outPayloadSize);
    }

    // ============ RANDOM ============
    else if (outType == "random") {

        payload = "RAND:";

        while (payload.length() < outPayloadSize) {
            payload += String(random(256), HEX);
        }

        payload =
            payload.substring(0, outPayloadSize);
    }

    // ============ ONES ============
    else if (outType == "ones") {

        payload = "ONES:";

        while (payload.length() < outPayloadSize) {
            payload += "1";
        }

        payload =
            payload.substring(0, outPayloadSize);
    }

    // ============ ZEROS ============
    else if (outType == "zeros") {

        payload = "ZEROS:";

        while (payload.length() < outPayloadSize) {
            payload += "0";
        }

        payload =
            payload.substring(0, outPayloadSize);
    }

    return payload;
}

// ================= APPLY CURRENT SWEEP PARAMETERS =================

void applySweepParameters(
    int &sf,
    int &bw,
    int &cr
) {

    sf =
        SF_VALUES[
            autoSender.currentSFIndex
        ];

    bw =
        BW_VALUES[
            autoSender.currentBWIndex
        ];

    cr =
        CR_VALUES[
            autoSender.currentCRIndex
        ];

    Serial.println(
        "Using Sweep Params -> "
        "SF:" + String(sf) +
        " BW:" + String(bw) +
        " CR:4/" + String(cr)
    );
}

// ================= CYCLE CONFIG =================

void cycleAutoSendConfig() {

    // ============ CR ============
    autoSender.currentCRIndex++;

    if (autoSender.currentCRIndex >= NUM_CR) {

        autoSender.currentCRIndex = 0;

        // ============ BW ============
        autoSender.currentBWIndex++;
    }

    if (autoSender.currentBWIndex >= NUM_BW) {

        autoSender.currentBWIndex = 0;

        // ============ SF ============
        autoSender.currentSFIndex++;
    }

    if (autoSender.currentSFIndex >= NUM_SF) {

        autoSender.currentSFIndex = 0;

        // ============ TYPE ============
        autoSender.currentTypeIndex++;
    }

    if (autoSender.currentTypeIndex >= AUTO_SEND_NUM_TYPES) {

        autoSender.currentTypeIndex = 0;

        // ============ PAYLOAD ============
        autoSender.currentPayloadIndex++;
    }

    if (autoSender.currentPayloadIndex >= AUTO_SEND_NUM_SIZES) {

        autoSender.currentPayloadIndex = 0;
    }

    autoSender.transmissionCount++;

    Serial.println("========================================");

    Serial.println(
        "Auto Cycle #" +
        String(autoSender.transmissionCount)
    );

    Serial.println(
        "Payload Size: " +
        String(
            AUTO_SEND_PAYLOAD_SIZES[
                autoSender.currentPayloadIndex
            ]
        )
    );

    Serial.println(
        "Type: " +
        String(
            AUTO_SEND_DATA_TYPES[
                autoSender.currentTypeIndex
            ]
        )
    );

    Serial.println(
        "SF: " +
        String(
            SF_VALUES[
                autoSender.currentSFIndex
            ]
        )
    );

    Serial.println(
        "BW: " +
        String(
            BW_VALUES[
                autoSender.currentBWIndex
            ]
        )
    );

    Serial.println(
        "CR: 4/" +
        String(
            CR_VALUES[
                autoSender.currentCRIndex
            ]
        )
    );

    Serial.println("========================================");
}

// ================= TIMER CHECK =================

bool shouldSendAutoMessage() {

    if (!autoSender.enabled) {
        return false;
    }

    unsigned long currentTime = millis();

    if (
        currentTime - autoSender.lastSendTime
        >= AUTO_SEND_INTERVAL
    ) {

        autoSender.lastSendTime = currentTime;

        return true;
    }

    return false;
}

// ================= INFO =================

String getAutoSendInfo() {

    String info = "";

    info += autoSender.enabled ?
        "AutoSend ENABLED | " :
        "AutoSend DISABLED | ";

    info +=
        "Payload: " +
        String(
            AUTO_SEND_PAYLOAD_SIZES[
                autoSender.currentPayloadIndex
            ]
        ) +
        " bytes | ";

    info +=
        "Type: " +
        String(
            AUTO_SEND_DATA_TYPES[
                autoSender.currentTypeIndex
            ]
        ) +
        " | ";

    info +=
        "SF:" +
        String(
            SF_VALUES[
                autoSender.currentSFIndex
            ]
        ) +
        " | ";

    info +=
        "BW:" +
        String(
            BW_VALUES[
                autoSender.currentBWIndex
            ]
        ) +
        " | ";

    info +=
        "CR:4/" +
        String(
            CR_VALUES[
                autoSender.currentCRIndex
            ]
        ) +
        " | ";

    info +=
        "Count: " +
        String(autoSender.transmissionCount);

    return info;
}

#endif // AUTO_SENDER_H