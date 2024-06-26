#ifndef POST_H
#define POST_H

#include<iostream>
#include<string>

class Post
{
public:
    // pre: none
    // post: a default post has been created
    Post();


    // pre: Pass an int representing messageId, int representing ownerId, string of post message, and int of likes
    // post: a post with specified parameters is created
    Post(int messageId, int ownerId, std::string message, int likes);


    // pre: none
    // post: a string representation of post is returned
    std::string virtual toString();


    // pre: none
    // post: returns the message Id of the post
    int getMessageId();


    // pre: none
    // post: returns the onwer Id of the post
    int getOwnerId();


    // pre: none
    // post: returns the message of the post
    std::string getMessage();


    // pre: none
    // post: returns the number of likes the post has
    int getLikes();


    // pre: none
    // post: returns the author of the post
    std::string virtual getAuthor();


    // pre: none
    // post: returns true/false for if post is public
    bool virtual getIsPublic();


private:
    int messageId_;
    int ownerId_;
    std::string message_;
    int likes_;
};






class IncomingPost : public Post{

public:

    // pre: none
    // post: a default incoming post object created
    IncomingPost();


    // pre: none
    // post: an incoming post object with specified parameters created
    IncomingPost(int messageId, int ownerId, std::string message, int likes, bool isPublic, std::string author);


    // pre: none
    // post: a string representation of the incoming post object is returned
    std::string toString();


    // pre: none
    // post: the author of the incoming post is returned
    std::string getAuthor();


    // pre: none
    // post: a boolean is returned representing if the incoming post is public
    bool getIsPublic();


private:
    std::string author_;
    bool isPublic_;

};


#endif // POST_H
