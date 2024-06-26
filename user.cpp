#include "user.h"
#include<string>
#include<set>
#include<iostream>


User::User(){};


User::User(int id, std::string name, int year, int zip, std::set<int> friends){
    id_ = id;
    name_ = name;
    year_ = year;
    zip_ = zip;
    friends_ = friends;
}


void User::addFriend(int id){
    friends_.insert(id);
}


void User::deleteFriend(int id){
    std::set<int>::iterator itr = friends_.find(id);

    if(itr != friends_.end()){
        friends_.erase(id);
    }
}


int User::getId(){ return id_; };


std::string User::getName(){ return name_; };


int User::getYear(){ return year_; };


int User::getZip(){ return zip_;};


std::set<int> & User::getFriends(){ return friends_;};



void User::addPost(Post * post){
    messages_.push_back(post);
}


std::vector<Post *> User::getPosts(){
    return messages_;
}


std::string User::getPostsString(int howMany, bool showOnlyPublic){
    // More specified message size than actuall messages, print out everything
    std::string output;

    int count = 0;
    for(int i = messages_.size()-1; i>=0; --i){

        if(count == howMany){
            return output;
        }

        if(showOnlyPublic && !messages_[i]->getIsPublic()){
            continue;
        }

        output += messages_[i]->toString();
        output += "\n\n";
        count++;
    }

    return output;
}


std::string User::getPassword(){ return password_; };


void User::setPassword(std::string password){
    password_ = password;
}

