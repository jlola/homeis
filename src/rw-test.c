#include <stdio.h>
#include <stdlib.h>
#include <usb.h>
#include "OneWire++/linkOW++/ow-functions.h"

#define SEARCH_ROM_CMD 0xF0
#define MAX_DEVICES 65536

int mainx() {
    unsigned char SN[MAX_DEVICES][8];
    int i;
    long dev, numDevices;

    const char commCmdName[16][32]
            = {"None",
               "SET DURATION",
               "BIT I/O",
               "PULSE ",
               "1-WIRE RESET",
               "BYTE I/O",
               "MATCH ACCESS",
               "BLOCK I/O",
               "READ STRAIGHT",
               "DO & RELEASE",
               "SET PATH",
               "WRITE SRAM PAGE",
               "WRITE EPROM",
               "READ CRC PROT PAGE",
               "READ REDIRECT PAGE W/CRC",
               "SEARCH ACCESS"};
        const char errorDescription[8][64]
            = {"No presence pulse detected",
               "Short on one-wire bus",
               "Alarming presence pulse detected",
               "Bus voltage not raised to 12V for programming pulse",
               "Byte read did not match byte written",
               "CRC read did not match CRC written",
               "Page was redirected",
               "Did not find specified number of devices"};
        const char slewRate[8][8]
            = {"15", "2.20", "1.65", "1.37", "1.10", "0.83", "0.70", "0.55"};

        unsigned char buff[32];
        int len, j;

        if (acquireAdapter() < 0) {
            return EXIT_FAILURE;
        }

        len = stateRead (buff);
        if (len < 0) {
            fprintf (stderr, "ERROR: %s\n", usb_strerror());
            releaseAdapter();
            return EXIT_FAILURE;
        }
        if (len < 16) {
            fprintf (stderr, "ERROR: Only %d bytes read from EP1\n", len);
            releaseAdapter();
            return EXIT_FAILURE;
        }

        printf ("%02X     5V strong pullup:                ", buff[0]);
        printf ("%s\n", (buff[0] & 0x01) ? "Enabled" : "Disabled");
        printf ("       12V programming pulse:           ");
        printf ("%s\n", (buff[0] & 0x02) ? "Enabled" : "Disabled");
        printf ("       Dynamic speed change:            ");
        printf ("%s\n", (buff[0] & 0x04) ? "Enabled" : "Disabled");

        printf ("%02X     1-wire bus speed:                ", buff[1]);
        switch (buff[1]) {
            case 0:
                printf ("Regular (65 microsecond time slot)");
            break;
            case 1:
                printf ("Flexible (65 - 72 microsecond time slot)");
            break;
            case 2:
                printf ("Overdrive (10 microsecond time slot)");
            break;
        }
        printf ("\n");

        printf ("%02X     5V strong pullup duration:       ", buff[2]);
        switch (buff[2]) {
            case 0:
                printf ("Infinite");
            break;
            case 0xFF:
            break;
            default:
                printf ("%d milliseconds", (int) buff[2] << 4);
            break;
        }
        printf ("\n");

        printf ("%02X     12V programming pulse duration:  ", buff[3]);
        switch (buff[3]) {
            case 0:
                printf ("Infinite");
            break;
            case 0xFF:
            break;
            default:
                printf ("%d microseconds", (int) buff[3] << 3);
            break;
        }
        printf ("\n");

        printf ("%02X     Flex. speed pulldown slew rate:  ", buff[4]);
        if (buff[4] < 8) {
            printf ("%s Volts/microsecond", slewRate[buff[4]]);
        }
        printf ("\n");

        printf ("%02X     Flex. speed write-1 low time:    ", buff[5]);
        if (buff[5] < 8) {
            printf ("%d microseconds", buff[5] + 8);
        }
        printf ("\n");

        printf ("%02X     Flex. speed data sample offset\n", buff[6]);
        printf ("           and write-0 recovery time:   ");
        if (buff[6] < 8) {
            printf ("%d microseconds", buff[6] + 3);
        }
        printf ("\n");

        printf ("%02X\n", buff[7]);

        printf ("%02X     5V strong pullup:                ", buff[8]);
        printf ("%s\n", (buff[8] & 0x01) ? "Active" : "Inactive");
        printf ("       12V programming pulse:           ");
        printf ("%s generated\n", (buff[8] & 0x02) ? "Being" : "Not being");
        printf ("       12V programming voltage:         ");
        printf ("%s\n", (buff[8] & 0x04) ? "Present" : "Not present");
        printf ("       DS2490 power source:             ");
        printf ("%s\n", (buff[8] & 0x08) ? "USB and external sources" : "USB only");
        printf ("       DS2490 halted:                   ");
        printf ("%s\n", (buff[8] & 0x10) ? "Yes" : "No");
        printf ("       DS2490 idle:                     ");
        printf ("%s\n", (buff[8] & 0x20) ? "Yes" : "No");
        printf ("       Command buffer status:           ");
        printf ("%s\n", (buff[8] & 0x80) ? "Overflow" : "OK");

        printf ("%02X %02X  Current communication command:   %s\n", buff[9], buff[10], commCmdName[buff[9] >> 4]);
        printf ("%02X     Bytes in comm. command buffer:   %d / 16\n", buff[11], buff[11]);
        printf ("%02X     Bytes in outgoing data buffer:   %d / 128\n", buff[12], buff[12]);
        printf ("%02X     Bytes in incoming data buffer:   %d / 128\n", buff[13], buff[13]);

        printf ("%02X\n", buff[14]);
        printf ("%02X\n", buff[15]);

        if (len > 16) {
            printf ("\nResult registers:\n\n");
            for (i = 16; i < len; i++) {
                printf ("%02X", buff[i]);
                if (buff[i] == 0xA5) {
                    printf ("     Presence pulse\n\n");
                    continue;
                }
                for (j = 0; j < 8; j++) {
                    if (buff[i] & (1 << j)) {
                        printf ("     %s\n  ", errorDescription[j]);
                    }
                }
                printf ("\n");
            }
        }

    //if (acquireAdapter() < 0) {
    //    return EXIT_FAILURE;
    //}

    numDevices = findDevices (SEARCH_ROM_CMD, SN, MAX_DEVICES);
    if (numDevices < 0) {
        releaseAdapter();
        return EXIT_FAILURE;
    }

    for (dev = 0; dev < numDevices; dev++) {
        for (i = 7; i >= 0; i--) {
            printf ("%02X", SN[dev][i]);
        }
        printf ("\n");
    }

    if (releaseAdapter() < 0) {
        return EXIT_FAILURE;
    }

    return 0;
}
