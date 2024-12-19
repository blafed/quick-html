#include "dev/easywsclient/easywsclient.hpp"
#include <assert.h>


#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace easywsclient;
// Global variables
WebSocket* ws = nullptr;
std::atomic<bool> running{true};
std::queue<std::string> message_queue;
std::mutex queue_mutex;
std::condition_variable queue_condition;

// Send a message through the WebSocket
void ws_send(const std::string& msg) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    message_queue.push(msg);
    queue_condition.notify_one();
}

// Handle incoming WebSocket messages
void handle_message(const std::string& message) {
    printf(">>> %s\n", message.c_str());
    if (message == "world") {
        running = false;
        ws->close();
    }
}

// WebSocket cycle running in a separate thread
void ws_scycle() {
#ifdef _WIN32
    INT rc;
    WSADATA wsaData;

    rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc) {
        printf("WSAStartup Failed.\n");
        return;
    }
#endif

    ws = WebSocket::from_url("ws://localhost:8126");
    assert(ws);

    // Initial messages
    ws_send("goodbye");
    ws_send("hello");

    // while (running && ws->getReadyState() != WebSocket::CLOSED) {
    //     // Poll and dispatch messages
    //     ws->poll();
    //     ws->dispatch(handle_message);

    //     // Process outgoing messages
    //     std::unique_lock<std::mutex> lock(queue_mutex);
    //     queue_condition.wait_for(lock, std::chrono::milliseconds(100), [] { return !message_queue.empty(); });

    //     while (!message_queue.empty()) {
    //         const std::string& msg = message_queue.front();
    //         ws->send(msg);
    //         message_queue.pop();
    //     }
    // }

    // Keep-alive mechanism
while (running) {
    if (ws == nullptr || ws->getReadyState() == WebSocket::CLOSED) {
        // Attempt to reconnect
        printf("Reconnecting...\n");
        delete ws; // Ensure old WebSocket instance is deleted
        ws = WebSocket::from_url("ws://localhost:8126");
        if (ws) {
            printf("Reconnected successfully.\n");
            ws_send("hello again"); // Send an initial message to establish the connection
        } else {
            printf("Reconnection failed. Retrying in 5 seconds...\n");
            std::this_thread::sleep_for(std::chrono::seconds(5)); // Retry after delay
            continue; // Skip further processing until reconnected
        }
    }

    try {
        // Poll and dispatch messages
        ws->poll();
        ws->dispatch(handle_message);

        // Process outgoing messages
        std::unique_lock<std::mutex> lock(queue_mutex);
        queue_condition.wait_for(lock, std::chrono::milliseconds(100), [] { return !message_queue.empty(); });

        while (!message_queue.empty()) {
            const std::string& msg = message_queue.front();
            ws->send(msg);
            message_queue.pop();
        }
        lock.unlock();

        // Keep-alive mechanism
        static auto last_heartbeat = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - last_heartbeat).count() >= 30) {
            ws->send("keep-alive"); // Send a keep-alive message
            last_heartbeat = now;
        }
    } catch (const std::exception& ex) {
        printf("Error: %s\n", ex.what());
        running = false; // Optionally handle errors more gracefully
    }
}


    delete ws;
    ws = nullptr;

#ifdef _WIN32
    WSACleanup();
#endif
}

int ws_start() {
    // Start WebSocket cycle in a separate thread
    std::thread ws_thread(ws_scycle);

    // Example usage of ws_send
    // ws_send("ping");
    // std::this_thread::sleep_for(std::chrono::seconds(2));
    // ws_send("another ping");
    // std::this_thread::sleep_for(std::chrono::seconds(2));

    // Signal to stop and join the thread
    // running = true;
    // if (ws_thread.joinable()) {
        ws_thread.join();
    // }

    return 0;
}


// void handle_message(const std::string & message)
// {
//     printf(">>> %s\n", message.c_str());
//     if (message == "world") { ws->close(); }
// }

// int main()
// {
// #ifdef _WIN32
//     INT rc;
//     WSADATA wsaData;

//     rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
//     if (rc) {
//         printf("WSAStartup Failed.\n");
//         return 1;
//     }
// #endif

//     assert(ws);

//     while (ws->getReadyState() != WebSocket::CLOSED) {
//       ws->poll();
//       ws->dispatch(handle_message);
//     }
//     delete ws;
// #ifdef _WIN32
//     WSACleanup();
// #endif
//     return 0;
// }



// void ws_init(){
//     ws = WebSocket::from_url("ws://localhost:8126");
//     assert(ws);
// }

// void ws_send(const char* msg){
//     printf("sending\n %s", msg);
//     if(ws->getReadyState() == WebSocket::OPEN)
//         ws->send(msg);
//     else
//         printf("the connection was closed \n");
// }

// void ws_close(){
//     delete ws;
// }

// void ws_listen(){
//     while (ws->getReadyState() != WebSocket::CLOSED) {
//       ws->poll();
//     }
// }

// void handle_message(const std::string & message)
// {
//     printf(">>> %s\n", message.c_str());
//     if (message == "world") { ws->close(); }
// }

// int ws_scycle()
// {
// #ifdef _WIN32
//     INT rc;
//     WSADATA wsaData;

//     rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
//     if (rc) {
//         printf("WSAStartup Failed.\n");
//         return 1;
//     }
// #endif

//     ws = WebSocket::from_url("ws://localhost:8126");
//     assert(ws);
//     ws->send("goodbye");
//     ws->send("hello");

//     while (ws->getReadyState() != WebSocket::CLOSED) {
//       ws->send("ping");
//       ws->poll();
//       ws->dispatch(handle_message);
//     }
//     delete ws;
// #ifdef _WIN32
//     WSACleanup();
// #endif
//     return 0;
// }
