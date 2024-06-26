#ifndef USER_H
#define USER_H

#include<string>
#include<set>
#include<iostream>

#include "post.h"


class User{

public:

    // CONSTRUCTOR

    // default constructor
    // pre: none
    // post: creates a default User
    User();

    // pre: none
    // post: creates User
    User(int id, std::string name, int year, int zip, std::set<int> friends);

    // pre: none
    // post: Add user of corresponding ID as friend, if ID exists do nothing
    void addFriend(int id);

    // pre: none
    // post: Delete user of corresponding ID from friend, if ID doesn't exist do nothing
    void deleteFriend(int id);

    // pre: none
    // post: returns id_
    int getId();

    // pre: none
    // post: returns name_
    std::string getName();

    // pre: none
    // post: returns year_
    int getYear();

    // pre: none
    // post: returns zip_
    int getZip();

    // pre: none
    // post: returns reference to friends_
    std::set<int> & getFriends();


    // pre: none
    // post: adds a post to the user's vector of posts
    void addPost(Post *);


    // pre: none
    // post: returns the vector that holds the pointer to all posts of the given user
    std::vector<Post *> getPosts();


    // pre: none
    // post: returns a string that holds the most recent howMany posts (or all posts if less than howMany)
    std::string getPostsString(int howMany, bool showOnlyPublic);


    // pre: none
    // post: returns password_
    std::string getPassword();


    // pre: none
    // post: updates the password_ field for user object
    void setPassword(std::string password);




private:
    int id_;
    std::string name_;
    int year_;
    int zip_;
    std::set<int> friends_;
    std::vector<Post * > messages_;

    std::string password_;

    // INVARIANTS
    // id_ represents the unique ID of a user in a network
    // name_ is the user object's name, format of: firstname + " " + lastname
    // year_ represents the date of birth of the user object
    // zip_ represents the user object's zip code
    // std::set<int> friends_ stores all of the IDs of the user's friends, no repeating friends allowed

};

#endif // USER_H
