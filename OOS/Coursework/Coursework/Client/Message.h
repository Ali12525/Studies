#pragma once

struct StructMessage {
    char type[100];
    char from[100];
    char to[100];
    char message[100];
    char fileName[100];
    int delaySeconds;
    int packetNumber;
    int totalPackets;
    size_t messageSize;
};