#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#ifndef WIN32
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <windows.h>
#endif
#include <time.h>
#include <stdlib.h>
#include <strings.h>
#include <stdint.h>

#define PORT (53)
#define SERVERADDRESS "1.1.1.1"
#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

typedef struct dns_packet_s {
    struct {
        uint16_t transanction_id;
        uint16_t flags;
        uint16_t questions;
        uint16_t answers;
        uint16_t authority;
        uint16_t additional;
    } header;
    uint8_t data[0];
} dns_packet;

typedef struct dns_packet_answer_s {

} dns_packet_answer;

typedef struct dns_packet_question_s {

} dns_packet_question;

uint8_t buffer[64];
uint8_t dns_packet_pack[] = {0x24, 0x1f, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x77, 0x77, 0x77, 0x06, 0x67, 0x6f, 0x6f, 0x67, 0x6c, 0x65, 0x03, 0x63, 0x6f, 0x6d, 0x00, 0x00, 0x01, 0x00, 0x01};

void print_buffer() {
    printf("0: ");
    for (size_t i = 0; i < ARRAY_SIZE(buffer); ++i) {
        printf("%x ", buffer[i]);
        if (i % 16 == 0 && i != 0) {
            printf("\n%ld: ", i);
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server;

#ifdef WIN32
    WORD version_requested = MAKEWORD(1, 1);
    WSADATA wsa_data = { 0 };

    int err = WSAStartup(version_requested, &wsa_data);

    if (err != 0) {
        return 0;
    }
#endif

    printf("Configure socket...\n");
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "Error opening socket");
        return 1;
    }

    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVERADDRESS);
    server.sin_port = htons(PORT);

    printf("Send UDP data...\n");

    if (sendto(sockfd, (char *)&dns_packet_pack, ARRAY_SIZE(dns_packet_pack), 0, (const struct sockaddr *) &server, sizeof(server)) < 0) {
        fprintf(stderr, "Error in sendto()\n");
        return 1;
    }

    if (recv(sockfd, (char *)&buffer, ARRAY_SIZE(buffer), 0) < 0) {
        fprintf(stderr, "Error in recv()\n");
        return 1;
    }

    print_buffer();

#ifdef WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif

    return 0;
}