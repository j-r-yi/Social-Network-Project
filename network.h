#ifndef NETWORK_H
#define NETWORK_H

#include "user.h"
#include <vector>
#include <iostream>
#include <string>

class Network
{
public:

    // CONSTRUCTORS

    // pre: none
    // post: creates a default Network
    Network();


    // pre: takes in a pointer to a user object
    // post: adds User to vector of users
    void addUser(User*);


    // pre: takes in two strings that represent the full name of users
    // post: creates friend connections between corresponding users, returns 0 on success, -1 if either is invalid
    int addConnection(std::string s1, std::string s2);

    // pre: takes in two strings that represent the full name of users
    // post: deletes friend connection between corresponding users, returns 0 on success, -1 if either is invalid
    int deleteConnection(std::string s1, std::string s2);


    // pre: takes in the full name of a user
    // post: returns corresponding id of given name, -1 if not found
    int getId(std::string name);


    // pre: takes in the file name to read from
    // post: initializes network info from a file, returns 0 on success, -1 on failure
    int readUsers(char * fname);


    // pre: takes in the file name to write all users to
    // post: writes all network info to a file, returns 0 on success, -1 on failure
    int writeUsers(char * fname);


    // pre: none
    // post: returns number of user on network
    int numUsers();

    // pre: takes in an ID representing the user
    // post: returns pointer to user corresponding to id, or nullptr
    User * getUser(int id);


    // pre: takes in two integers, representing the starting user to the end user
    // post: returns the relational distance/shortest path between two users
    std::vector<int> shortestPath(int from, int to);


    // pre: takes in three integers, representing the starting user, the end user (if found) and the distance in relation to "from"
    // post: The path to a user whos relational distance from given user is returned
    std::vector<int> distanceUser(int from, int& to, int distance);


    // pre: takes in two integers, a starting user and a score
    // post: Returns a list of users who are relational distance 2 from specified user who, with a score of common friends they share with "who"
    std::vector<int> suggestFriends(int who, int& score);


    // pre: none
    // post: returns all connected components of network
    std::vector<std::vector<int>> groups();


    // pre: none
    // post: adds a new post with specified parameters to message vector of user whose id is ownerId
    void addPost(int ownerId, std::string message, int likes, bool isIncoming, std::string authorName, bool isPublic);


    // pre: none
    // post:
    std::string getPostsString(int ownerId, int howMany, bool showOnlyPublic);


    // pre: passed in a valid filename
    // post: reads the post from a file formatted in specification, returns -1 if file cannot be opened
    int readPosts(char * fname);


    // pre: passed in a valid filename
    // post: writes the post of the network to a file formatted in specification, returns -1 if file cannot be opened
    int writePosts(char * fname);


    // pre: passed in a valid filename
    // post: reads the passwords of users from a file formatted in specification, returns -1 if file cannot be opened
    int readPasswords(char * fname);


    // pre: passed in a valid filename
    // post: writes the passwords of users to a file formatted in specification, returns -1 if file cannot be opened
    int writePasswords(char * fname);


    // pre: pass in a string composing of valid typable ASCII characters 32-126
    // post: encodes the string with caesar cipher with shift of 13 and returns encoded string
    std::string encode(std::string password);


    // pre: pass in a string composing of valid typable ASCII characters 32-126
    // post: decodes the string with a caesar cipher with shift of -13 and returns the decoded string
    std::string decode(std::string password);

private:
    std::vector<User*> users_;


    // INVARIANTS
    // the vector users_ stores a series of pointers to user objects, with the size of users_ representing the number of users

};

#endif // NETWORK_H
