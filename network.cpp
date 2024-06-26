#include "network.h"

#include <string>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include <queue>
#include <algorithm>

Network::Network(){}


void Network::addUser(User* user){
    users_.push_back(user);
}


int Network::addConnection(std::string s1, std::string s2){
    if(s1 == s2){
        return -1;
    }

    int id1 = getId(s1);
    int id2 = getId(s2);

    User * person1 = getUser(id1);
    User * person2 = getUser(id2);

    if(person1 == nullptr || person2 == nullptr){
        return -1;
    }

    person1->addFriend(id2);
    person2->addFriend(id1);

    return 0;
}


int Network::deleteConnection(std::string s1, std::string s2){
    if(s1 == s2){
        return -1;
    }

    int id1 = getId(s1);
    int id2 = getId(s2);

    User * person1 = getUser(id1);
    User * person2 = getUser(id2);

    if(person1 == nullptr || person2 == nullptr){
        return -1;
    }

    person1->deleteFriend(id2);
    person2->deleteFriend(id1);

    return 0;
}


int Network::getId(std::string name){
    for(auto temp : users_){
        if(temp->getName() == name){
            return temp->getId();
        }
    }

    return -1;

}


int Network::readUsers(char * fname){
    std::ifstream in;
    in.open(fname);
    if(in.fail()){
        std::cout << "Failed to open the input file" << std::endl;
        return -1;
    }


    std::string currentLine;
    // Ignores the first line of total number of users
    std::getline(in, currentLine);


    std::string id, name, year, zip;
    std::set<int> friends;
    std::string friendString;

    while(getline(in, id) && getline(in, name) && getline(in, year) && getline(in, zip) && getline(in, friendString)){
        // Gets rid of all the \t's
        name = name.substr(1);
        year = year.substr(1);
        zip = zip.substr(1);
        friendString = friendString.substr(1);

        std::stringstream ss(friendString);
        int number;

        friends.clear();

        while(ss>>number){
            friends.insert(number);
        }

        User * u = new User(std::stoi(id), name, std::stoi(year), std::stoi(zip), friends);
        users_.push_back(u);

    }

    return 0;
}


int Network::writeUsers(char * fname){
    std::ofstream out;
    out.open(fname);
    if(out.fail()){
        std::cout << "Failed to open the output file" << std::endl;
        return -1;
    }

    out << numUsers() << std::endl;
    for(auto temp : users_){
        out << temp->getId() << std::endl;
        out << "\t" << temp->getName() << std::endl;
        out << "\t" << temp->getYear() << std::endl;
        out << "\t" << temp->getZip() << std::endl;
        out << "\t";
        for(auto f : temp->getFriends()){
            out << f  << " ";
        }
        out << std::endl;
    }

    out.close();

    return 0;
}


int Network::numUsers(){
    return users_.size();
}


User * Network::getUser(int id){
    for(auto temp : users_){
        if(temp->getId() == id){
            return temp;
        }
    }

    return nullptr;
}




std::vector<int> Network::shortestPath(int from, int to){
    std::queue<int> userQ;
    std::vector<bool> visited(numUsers(), 0);
    userQ.push(from);
    visited[from] = true;

    // Backtrack Pathing
    std::vector<int> prev(numUsers(), -1);

    if(from == to){
        return std::vector<int> {from};
    }

    while(!userQ.empty()){
        int currId = userQ.front();
        std::set<int> & friends = getUser(currId)->getFriends();
        userQ.pop();
        // std::cout << currId << " " <<std::endl;

        for(auto f : friends){
            if(!visited[f]){
                prev[f] = currId;
                visited[f] = true;
                userQ.push(f);
            }
        }
    }

    // If from and to are connected
    if(visited[to]){
        // Storing Output
        std::vector<int> output;
        int curr = to;
        while(curr != -1){
            output.push_back(curr);
            curr = prev[curr];
        }

        // Reverse Output
        std::reverse(output.begin(), output.end());

        return output;
    }

    return std::vector<int>();
}






std::vector<int> Network::distanceUser(int from, int& to, int distance){
    std::queue<int> userQ;
    std::vector<bool> visited(numUsers(), 0);
    std::vector<int> dist(numUsers(), 0);
    std::vector<int> prev(numUsers(), -1);

    userQ.push(from);
    visited[from] = true;

    if(distance < 0){
        return std::vector<int> ();
    }

    while(!userQ.empty()){

        int curr = userQ.front();
        std::set<int> & friends = getUser(curr)->getFriends();
        userQ.pop();
        for(auto friendID : friends){
            if(!visited[friendID]){
                userQ.push(friendID);
                visited[friendID] = true;

                prev[friendID] = curr;
                // The current distance is one more than the previous
                dist[friendID] = dist[curr] + 1;
            }

            if(dist[friendID] == distance){
                // Storing Output
                std::vector<int> output;
                int backTrack = friendID;
                while(backTrack != -1){
                    output.push_back(backTrack);
                    backTrack = prev[backTrack];
                }

                // Reverse Output
                std::reverse(output.begin(), output.end());
                to = friendID;
                return output;
            }

        }
    }
    return std::vector<int>();
}






std::vector<int> Network::suggestFriends(int who, int& score){
    // If who doesn't exist, return emtpy vector and set score = 0;
    if(getUser(who) == nullptr){
        score = 0;
        return std::vector<int>();
    }

    score = -1;

    std::queue<int> userQ;
    std::vector<int> distance(numUsers(),0);
    std::vector<bool> visited(numUsers(),0);
    std::vector<int> mutualFriends(numUsers(), 0);
    std::set<int> directFriends(getUser(who)->getFriends());


    userQ.push(who);
    visited[who] = true;
    distance[who] = 0;

    while(!userQ.empty()){
        int curr = userQ.front();
        userQ.pop();
        std::set<int> & friends = getUser(curr)->getFriends();

        for(auto f : friends){
            if(!visited[f]){
                userQ.push(f);
                visited[f] =  true;
                distance[f] = distance[curr] + 1;

                if(distance[curr] == 1 && distance[f] == 2){
                    for(int mutualF : getUser(f)->getFriends()){
                        if(directFriends.find(mutualF) != directFriends.end()){
                            mutualFriends[f]++;
                        }
                    }

                    if(score <= mutualFriends[f]){
                        score = mutualFriends[f];
                    }
                }
            }

        }
    }

    std::vector<int> output;

    for(int i = 0; i<numUsers(); ++i){
        if(mutualFriends[i] == score && distance[i] == 2 && directFriends.find(i) == directFriends.end()){
            output.push_back(i);
        }
    }


    return output;
}



std::vector<std::vector<int>> Network::groups(){
    std::vector<std::vector<int>> output;

    std::vector<bool> visited(numUsers(), 0);
    std::vector<int> connectedComponent;
    std::stack<int> userStack;

    // Loops through all users
    for(int i = 0; i<numUsers(); ++i){

        // For each unvisited user perform DFS
        if(!visited[i]){
            userStack.push(users_[i]->getId());
            visited[users_[i]->getId()] = true;

            while(!userStack.empty()){

                int curr = userStack.top();
                userStack.pop();
                connectedComponent.push_back(curr);

                for(auto friends : users_[curr]->getFriends()){
                    if(!visited[friends]){
                        userStack.push(friends);
                        visited[friends] = true;
                    }
                }
            }

            output.push_back(connectedComponent);
            connectedComponent.clear();
        }


    }
    return output;

}




void Network::addPost(int ownerId, std::string message, int likes, bool isIncoming, std::string authorName, bool isPublic){
    int currId = 0;
    for(auto u : users_){
        currId += u->getPosts().size();
    }

    if(isIncoming){
        IncomingPost * ip = new IncomingPost(currId, ownerId, message, likes, isPublic, authorName);
        getUser(ownerId)->addPost(ip);
    }else{
        Post * p = new Post(currId, ownerId, message, likes);
        getUser(ownerId)->addPost(p);
    }
}



std::string Network::getPostsString(int ownerId, int howMany, bool showOnlyPublic){
    return getUser(ownerId)->getPostsString(howMany, showOnlyPublic);
}



int Network::readPosts(char * fname){
    std::ifstream in;
    in.open(fname);
    if(in.fail()){
        std::cout << "Failed to open the input file" << std::endl;
        return -1;
    }

    std::string firstLine;
    // Ignores the first line of total number of users
    std::getline(in, firstLine);


    std::string messageId, messageText, ownerId, likes, publicity, author;

    while(getline(in, messageId) && getline(in, messageText) && getline(in, ownerId) && getline(in, likes) && getline(in, publicity) && getline(in, author)){
        // Gets rid of all the \t's
        messageText = messageText.substr(1);
        ownerId = ownerId.substr(1);
        likes = likes.substr(1);

        if(publicity.size() != 0){
            publicity = publicity.substr(1);
            author = author.substr(1);

            IncomingPost * ip = new IncomingPost(std::stoi(messageId), std::stoi(ownerId), messageText, std::stoi(likes), (publicity == "public" ? true : false), author);
            getUser(std::stoi(ownerId))->addPost(ip);
        }else{
            Post * p = new Post(std::stoi(messageId), std::stoi(ownerId), messageText, std::stoi(likes));
            getUser(std::stoi(ownerId))->addPost(p);
        }
    }


    return 0;
}


// Helper method to compare two posts
bool comparePosts(Post * p1, Post * p2){
    return p1->getMessageId() < p2->getMessageId();
}


int Network::writePosts(char * fname){
    std::ofstream out;
    out.open(fname);
    if(out.fail()){
        std::cout << "Failed to open the output file" << std::endl;
        return -1;
    }


    std::vector<Post *> allPosts;
    int totalPosts = 0;
    for(auto u : users_){
        totalPosts += u->getPosts().size();
        for(auto p : u->getPosts()){
            allPosts.push_back(p);
        }
    }
    sort(allPosts.begin(), allPosts.end(), comparePosts);
    out << totalPosts << std::endl;

    for(auto posts : allPosts){
        out << posts->getMessageId() << std::endl;
        out << "\t" << posts->getMessage() << std::endl;
        out << "\t" << posts->getOwnerId() << std::endl;
        out << "\t" << posts->getLikes() << std::endl;


        // IF owner post
        if(posts->getAuthor() != ""){
            out << "\t" << (posts->getIsPublic() ? "public" : "private") << std::endl;
            out << "\t" << posts->getAuthor() << std::endl;
        }else{
            out << "\n" << "\n";
        }
    }

    out.close();

    return 0;
}


int Network::readPasswords(char * fname){
    std::ifstream in;
    in.open(fname);
    if(in.fail()){
        std::cout << "Failed to open the input file" << std::endl;
        return -1;
    }

    std::string firstLine;
    // Ignores the first line of total number of users
    std::getline(in, firstLine);

    std::string password;

    for(int i = 0; i<numUsers(); ++i){
        getline(in, password);
        getUser(i)->setPassword(decode(password));
    }

    return 0;
}



int Network::writePasswords(char * fname){
    std::ofstream out;
    out.open(fname);
    if(out.fail()){
        std::cout << "Failed to open the output file" << std::endl;
        return -1;
    }

    out << numUsers() << std::endl;
    for(auto temp : users_){
        out << encode(temp->getPassword()) << std::endl;
    }

    out.close();

    return 0;
}



std::string Network::encode(std::string password){
    std::string result;
    int start = 32;
    int end = 126;
    int shift = 13;
    for(char c : password){
        if(c >= 32 && c<=126){
            c = (c - start + shift) % (end - start) + start;
            result += c;
        }
    }
    return result;
}

std::string Network::decode(std::string password){
    std::string result;
    int start = 32;
    int end = 126;
    int shift = 13;
    for(char c : password){
        if(c >= 32 && c<=126){
            c = (c - start - shift + (end - start)) % (end - start) + start;
            result += c;
        }
    }
    return result;
}



