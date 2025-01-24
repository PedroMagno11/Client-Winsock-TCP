#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 512
#define MENSAGEM_PARA_SAIR "#sair"

SOCKET remote_socket; // socket para o servidor TCP (remoto)

int message_length = 0; // Tamano da mensagem a ser enviada

char message[BUFFER_SIZE]; // mensagem que o cliente deseja enviar ao servidor TCP (remoto)

size_t remote_port = 0; // Porta do servidor TCP (remoto)

char remote_ip[20]; // Endereço do servidor TCP (remoto)

struct sockaddr_in remote_address; // Estrutura que armazena o endereço do servidor TCP (remoto)

WSADATA wsa_data; // Estrutura usada pela função WSAStartup para inicializar o Winsock

void error(char *msg) {
    fprintf(stderr, "%s\n", msg);
    system("PAUSE");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {

    // -------------------- Inicializa o Winsock ----------------------------------------------------
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        error("Falha ao inicializar :(");
    }

    // ------------------ Solicita informações do servidor TCP (remoto -> onde desejo se conectar) --------------

    printf("IP do servidor: ");
    scanf("%s", remote_ip);
    fflush(stdin); // limpa o buffer

    printf("Porta do servidor: ");
    scanf("%d", &remote_port);
    fflush(stdin);

    // ------------ Cria o socket para o servidor remoto --------------------------------------------
    /*
     * AF_INET -> Define que o socket usará o protocolo IPv4
     * SOCK_STREAM -> Define que o socket usará TCP
     * 0 -> Permite que o sistema escolha o protocolo apropriado (no caso, TCP)
    */
    remote_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (remote_socket == INVALID_SOCKET) {
        WSACleanup();
        error("Não foi possível criar o socket para o servidor remoto :(");
    }

    memset(&remote_address, 0, sizeof(remote_address)); // Garante a limpeza da variável remote_address
    remote_address.sin_family = AF_INET;
    remote_address.sin_addr.s_addr = inet_addr(remote_ip);
    remote_address.sin_port = htons(remote_port);


    printf("Conectando o servidor %s na porta %llu ...\n", remote_ip, remote_port);

    // ---------------------- Inicia a conexão do cliente com o servidor remoto -------------------------------------
    if (connect(remote_socket, (struct sockaddr *)&remote_address, sizeof(remote_address)) == SOCKET_ERROR) {
        WSACleanup();
        error("Falha ao conectar :(");
    }

    printf("Conexão realizada com sucesso! :)\n");
    printf("Digite as mensagens\n");

    do {
        memset(&message, 0, BUFFER_SIZE); // Garante a limpeza da variável message

        printf("Mensagem para o servidor: ");
        fgets(message, sizeof(message), stdin);
        fflush(stdin);
        message_length = strlen(message);

        // ----------- Envia a mensagem para o servidor TCP (remoto) ------------------------
        if (send(remote_socket, message, message_length, 0) == SOCKET_ERROR)
        {
            WSACleanup();
            closesocket(remote_socket);
            error("send() failed\n");
        }
    } while(strcmp(message, MENSAGEM_PARA_SAIR) != 0); // envia "#sair" para o servidor

    printf("Encerrando\n");

    // ------------------ Fecha o socket e limpa o Winsock ---------------------------
    WSACleanup();
    closesocket(remote_socket);

    system("PAUSE");

    return 0;
}