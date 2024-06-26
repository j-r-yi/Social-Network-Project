#ifndef SOCIALNETWORKWINDOW_H
#define SOCIALNETWORKWINDOW_H

#include <QMainWindow>
#include "user.h"
#include "post.h"
#include "network.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class SocialNetworkWindow;
}
QT_END_NAMESPACE

class SocialNetworkWindow : public QMainWindow
{
    Q_OBJECT

public:
    SocialNetworkWindow(QWidget *parent = nullptr);
    ~SocialNetworkWindow();

    // pre: none
    // post: updates the UI by displaying the information relating to displayedUser_
    void updateUI();

    // pre: must enter a valid username in network
    // post: logs in to the username entered, sets the loggedInUser_ to the username tyepd
    void login();

    // pre: none
    // post: helper method to display all the friends of the displayedUser_
    void displayFriends(User * u);

    // pre: none
    // post: helper method to display all the posts of the displayedUser_
    void displayPosts(User * u);

    // pre: displayedUser_ must be the same as loggedInUser_ to call this method
    // post: helper method to display suggested friends of the logged in user
    void displaySuggestFriends(int u);

    // pre: clicked a valid cell on the friends table
    // post: brings the user to the page of the friend that was clicked
    void clickedFriend(int row, int col);

    // pre: clicked a valid cell on the suggested friends table
    // post: brings the user to the page of the suggested friend that was clicked
    void clickedSuggestedFriend(int row, int col);

    // pre: both users must exist on network, displayedUser_ != loggedInUser_
    // post: adds a friend connection to the displayedUser_ and the loggedInUser_, updatesUI, and rewrites users.txt
    void addFriend();

    // pre: displayedUser_ != loggedInUser_, only displayed on non logged in user page
    // post: changes displayedUser_ into loggedInUser_, updatesUI to reflect the logged in user's page
    void back();

    // pre: none
    // post: displays the sign up page
    void toSignUp();

    // pre: none
    // post: creates a new user on the network
    void signUp();

    // pre: in the sign up page
    // post: brings user back to the log in page
    void backToLogIn();


    // pre: none
    // post: log in UI is hidden
    void hideLogIn();


    // pre: user must be currently logged in
    // post: logs the user out and brings back into log in page ui
    void logout();


    // pre: none
    // post: intializes the new user page
    void initializeNewUserPage();


    // pre: none
    // post: checks if password follows correct format
    bool checkPassword();


    // pre: none
    // post: creates a list of users on network by substring to display on a label
    void searchFriend();

private:
    Ui::SocialNetworkWindow *ui;
    User * loggedInUser_;
    User * displayedUser_;
    Network network_;


};
#endif // SOCIALNETWORKWINDOW_H
