#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

void init_winsock();
void cleanup_winsock();
int setup_server(int port);
int setup_client(const char* ip, int port);

#endif // SOCKET_UTILS_H