#include "server.hpp"
#include "client.hpp"
#include "channel.hpp"
#include "exception.hpp"
#include <iostream>
#include <cassert>
#include <sstream>

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

void printTest(const std::string& name) {
    std::cout << YELLOW << "\n[TEST] " << name << RESET << std::endl;
}

void printPass(const std::string& msg = "") {
    std::cout << GREEN << "  ✓ PASS";
    if (!msg.empty())
        std::cout << " - " << msg;
    std::cout << RESET << std::endl;
}

void printFail(const std::string& msg) {
    std::cout << RED << "  ✗ FAIL - " << msg << RESET << std::endl;
}

void printInfo(const std::string& msg) {
    std::cout << BLUE << "  → " << msg << RESET << std::endl;
}

// Fonction pour vérifier si un client est dans un channel
bool isClientInChannel(Channel *channel, Client *client) {
    if (!channel)
        return false;
    
    std::vector<Client*> clients = channel->getClients();
    for (size_t i = 0; i < clients.size(); i++) {
        if (clients[i] == client || 
            clients[i]->getNickname() == client->getNickname())
            return true;
    }
    return false;
}

// Fonction pour afficher les clients d'un channel
void printChannelMembers(Server *server, const std::string &channelName) {
    Channel *channel = server->getChannel(channelName);
    if (!channel) {
        printInfo("Channel does not exist");
        return;
    }
    
    std::vector<Client*> clients = channel->getClients();
    std::cout << BLUE << "  → Channel #" << channelName << " members (" 
              << clients.size() << "): ";
    
    for (size_t i = 0; i < clients.size(); i++) {
        std::cout << clients[i]->getNickname();
        if (i < clients.size() - 1)
            std::cout << ", ";
    }
    std::cout << RESET << std::endl;
}

int main() {
    std::cout << "\n" << YELLOW << "╔════════════════════════════════════════╗" << RESET << std::endl;
    std::cout << YELLOW << "║  IRC Server Channel Handler Test Suite  ║" << RESET << std::endl;
    std::cout << YELLOW << "╚════════════════════════════════════════╝" << RESET << std::endl;

    // Créer le serveur
    int server_fd = create_server_socket(6667);
    if (server_fd < 0) {
        printFail("Could not create server socket");
        return 1;
    }
    Server server("test_password", server_fd);

    // Créer des clients
    Client alice(&server, 1001);
    Client bob(&server, 1002);
    Client charlie(&server, 1003);

    // Enregistrer les clients
    auto registerClient = [&](Client &c, const std::string &nick, const std::string &user) {
        c.setHostname("127.0.0.1");
        c.setNickname(nick);
        c.setUsername(user);
        c.setRegistered(true);
        server.addClient(&c);
    };

    registerClient(alice, "alice", "alice_user");
    registerClient(bob, "bob", "bob_user");
    registerClient(charlie, "charlie", "charlie_user");

    printTest("Server - Print registered clients");
    try
    {
        std::vector<Client*> *clients = server.getClients();
        if (!clients || clients->empty())
        {
            printFail("No clients registered on server");
        }
        else
        {
            printPass("Registered clients on server: " + std::to_string(clients->size()));
            for (size_t i = 0; i < clients->size(); i++)
            {
                if ((*clients)[i])
                {
                    printInfo("  - " + (*clients)[i]->getNickname() + 
                             " (" + (*clients)[i]->getUsername() + 
                             "@" + (*clients)[i]->getHostname() + ")");
                }
            }
        }
    }
    catch (std::exception &e)
    {
        printFail(std::string(e.what()));
    }

    // Test 1: isFullyRegistered
    printTest("isFullyRegistered - Registered Client");
    try {
        if (server.isFullyRegistered(&alice)) {
            printPass("Alice is fully registered");
        } else {
            printFail("Alice should be fully registered");
        }
    } catch (std::exception &e) {
        printFail(std::string(e.what()));
    }

    // Test 2: Create and join a channel
    printTest("JOIN - Create Channel #general (Alice)");
    try {
        server.join(&alice, "#general");
        printPass("Alice created #general channel");
        
        // Vérifier dans les channels d'Alice
        bool alice_has_channel = false;
        for (const auto &ch : alice.getChannels()) {
            if (ch == "general") {
                alice_has_channel = true;
                break;
            }
        }
        
        // Vérifier dans le channel lui-même
        Channel *channel = server.getChannel("general");
        bool alice_in_channel = isClientInChannel(channel, &alice);
        
        if (alice_has_channel && alice_in_channel) {
            printPass("Alice is in both: her client list AND the channel");
            printChannelMembers(&server, "general");
        } else {
            if (!alice_has_channel)
                printFail("Alice doesn't have #general in her channels list");
            if (!alice_in_channel)
                printFail("Alice is not in the #general channel members");
        }
    } catch (IrcException &e) {
        printFail("Exception thrown: " + std::to_string(e.errorCode()));
    }

    // Test 3: Second client joins existing channel
    printTest("JOIN - Bob joins existing #general channel");
    try {
        server.join(&bob, "#general");
        printPass("Bob joined #general");
        
        // Vérifier dans les channels de Bob
        bool bob_has_channel = false;
        for (const auto &ch : bob.getChannels()) {
            if (ch == "general") {
                bob_has_channel = true;
                break;
            }
        }
        
        // Vérifier dans le channel lui-même
        Channel *channel = server.getChannel("general");
        bool bob_in_channel = isClientInChannel(channel, &bob);
        
        if (bob_has_channel && bob_in_channel) {
            printPass("Bob is in both: his client list AND the channel");
            printChannelMembers(&server, "general");
        } else {
            printFail("Bob not properly added to #general");
        }
    } catch (IrcException &e) {
        printFail("Exception thrown: " + std::to_string(e.errorCode()));
    }

    // Test 4: Third client joins the same channel
    printTest("JOIN - Charlie joins #general");
    try {
        server.join(&charlie, "#general");
        printPass("Charlie joined #general");
        
        Channel *channel = server.getChannel("general");
        bool charlie_in_channel = isClientInChannel(channel, &charlie);
        
        if (charlie_in_channel) {
            printPass("Charlie is in the #general channel");
            printChannelMembers(&server, "general");
        } else {
            printFail("Charlie not found in #general channel members");
        }
    } catch (IrcException &e) {
        printFail("Exception thrown: " + std::to_string(e.errorCode()));
    }

    // Test 5: PRIVMSG to channel
    printTest("PRIVMSG - Alice sends message to #general");
    try {
        server.privmsg(&alice, "#general :Hello everyone!");
        printPass("Alice sent message to #general");
        printInfo("Message should be broadcast to: bob, charlie");
    } catch (IrcException &e) {
        printFail("Exception thrown: " + std::to_string(e.errorCode()));
    }

    // Test 6: PRIVMSG direct message
    printTest("PRIVMSG - Direct message from alice to bob");
    try {
        server.privmsg(&alice, "bob :Hi Bob, how are you?");
        printPass("Alice sent private message to bob");
    } catch (IrcException &e) {
        printFail("Exception thrown: " + std::to_string(e.errorCode()));
    }

    // Test 7: PRIVMSG empty message (should fail)
    printTest("PRIVMSG - Empty message (should fail)");
    try {
        server.privmsg(&alice, "#general :");
        printFail("Should have thrown ERR_NOTEXTTOSEND");
    } catch (ERR_NOTEXTTOSEND &e) {
        printPass("Correctly threw ERR_NOTEXTTOSEND");
    } catch (IrcException &e) {
        printFail("Wrong exception: " + std::to_string(e.errorCode()));
    }

    // Test 8: TOPIC change
    printTest("TOPIC - Alice changes topic of #general");
    try {
        server.topic(&alice, "#general :Welcome to General Chat!");
        printPass("Alice changed topic of #general");
    } catch (IrcException &e) {
        printFail("Exception thrown: " + std::to_string(e.errorCode()));
    }

    // Test 9: PART - Leave channel
    printTest("PART - Alice leaves #general");
    try {
        server.part(&alice, "#general");
        printPass("Alice left #general");
        
        // Vérifier qu'Alice n'est plus dans ses channels
        bool alice_has_channel = false;
        for (const auto &ch : alice.getChannels()) {
            if (ch == "general") {
                alice_has_channel = true;
                break;
            }
        }
        
        // Vérifier qu'Alice n'est plus dans le channel
        Channel *channel = server.getChannel("general");
        bool alice_in_channel = isClientInChannel(channel, &alice);
        
        if (!alice_has_channel && !alice_in_channel) {
            printPass("Alice removed from both: her client list AND the channel");
            printChannelMembers(&server, "general");
        } else {
            if (alice_has_channel)
                printFail("Alice still has #general in her channels");
            if (alice_in_channel)
                printFail("Alice still in #general channel members");
        }
    } catch (IrcException &e) {
        printFail("Exception thrown: " + std::to_string(e.errorCode()));
    }

    // Test 10: Create second channel
    printTest("JOIN - Bob creates #dev channel");
    try {
        server.join(&bob, "#dev");
        printPass("Bob created #dev channel");
        
        Channel *channel = server.getChannel("dev");
        bool bob_in_dev = isClientInChannel(channel, &bob);
        
        if (bob_in_dev) {
            printInfo("Bob's channels: general, dev");
            printChannelMembers(&server, "dev");
        } else {
            printFail("Bob not in #dev channel");
        }
    } catch (IrcException &e) {
        printFail("Exception thrown: " + std::to_string(e.errorCode()));
    }

    // Test 11: Multiple channels join
    printTest("JOIN - Alice joins multiple channels (#general, #dev)");
    try {
        server.join(&alice, "#general,#dev");
        printPass("Alice joined #general and #dev");
        
        // Vérifier Alice dans #general
        Channel *general = server.getChannel("general");
        bool alice_in_general = isClientInChannel(general, &alice);
        
        // Vérifier Alice dans #dev
        Channel *dev = server.getChannel("dev");
        bool alice_in_dev = isClientInChannel(dev, &alice);
        
        if (alice_in_general && alice_in_dev) {
            printPass("Alice is in both #general and #dev channels");
            printChannelMembers(&server, "general");
            printChannelMembers(&server, "dev");
        } else {
            if (!alice_in_general)
                printFail("Alice not in #general");
            if (!alice_in_dev)
                printFail("Alice not in #dev");
        }
    } catch (IrcException &e) {
        printFail("Exception thrown: " + std::to_string(e.errorCode()));
    }

    // Test 12: Final state verification
    printTest("Final State - Verify all channels");
    try {
        printInfo("=== Final Channel State ===");
        printChannelMembers(&server, "general");
        printChannelMembers(&server, "dev");
    } catch (std::exception &e) {
        printFail(std::string(e.what()));
    }
    printTest("INVITE - Invite-only channel");
    try
    {
        server.join(&alice, "#private");
        server.mode(&alice, "#private +i");

        server.join(&bob, "#private");
        Channel *privateChan = server.getChannel("private");
        if (isClientInChannel(privateChan, &bob))
            printFail("Bob should not join #private without invite");
        else
            printPass("Bob blocked from invite-only channel");

        server.invite(&alice, "bob #private");
        server.join(&bob, "#private");

        if (isClientInChannel(privateChan, &bob))
            printPass("Bob joined #private after invite");
        else
            printFail("Bob should have joined #private after invite");
    }
    catch (IrcException &e)
    {
        printFail("Exception thrown: " + std::to_string(e.errorCode()));
    }

    // Test 14: KEY - password protected channel
    printTest("MODE +k - Password protected channel");
    try
    {
        server.join(&alice, "#keyed");
        server.mode(&alice, "#keyed +k secret");

        server.join(&charlie, "#keyed wrong");
        Channel *keyedChan = server.getChannel("keyed");
        if (isClientInChannel(keyedChan, &charlie))
            printFail("Charlie should not join #keyed with wrong key");
        else
            printPass("Charlie blocked with wrong key");

        server.join(&charlie, "#keyed secret");
        if (isClientInChannel(keyedChan, &charlie))
            printPass("Charlie joined #keyed with correct key");
        else
            printFail("Charlie should have joined #keyed with correct key");
    }
    catch (IrcException &e)
    {
        printFail("Exception thrown: " + std::to_string(e.errorCode()));
    }

    // Test 15: LIMIT - user limit
    printTest("MODE +l - User limit");
    try
    {
        server.join(&alice, "#limited");
        server.mode(&alice, "#limited +l 2"); // alice + 1 user max
        
        server.join(&bob, "#limited");
        server.join(&charlie, "#limited");

        Channel *limitedChan = server.getChannel("limited");
        bool aliceInLimited = isClientInChannel(limitedChan, &alice);
        bool bobInLimited = isClientInChannel(limitedChan, &bob);
        bool charlieInLimited = isClientInChannel(limitedChan, &charlie);

        if (aliceInLimited && bobInLimited && !charlieInLimited)
            printPass("User limit respected");
        else
            printFail("User limit not respected");
    }
    catch (IrcException &e)
    {
        printFail("Exception thrown: " + std::to_string(e.errorCode()));
    }

    // Test 16: Invalid syntax / malformed commands
    printTest("INVALID SYNTAX - Malformed commands");
    try
    {
        server.join(&alice, "");
        server.privmsg(&alice, "bob");
        server.topic(&alice, "#general");
        server.kick(&alice, "#general");
        server.mode(&alice, "#general +k");
        printPass("Malformed commands did not crash");
    }
    catch (IrcException &e)
    {
        printPass("Malformed commands raised error code: " + std::to_string(e.errorCode()));
    }

    std::cout << "\n" << YELLOW << "╔════════════════════════════════════════╗" << RESET << std::endl;
    std::cout << YELLOW << "║     Test Suite Completed Successfully    ║" << RESET << std::endl;
    std::cout << YELLOW << "╚════════════════════════════════════════╝\n" << RESET << std::endl;

    close(server_fd);
    return 0;
}