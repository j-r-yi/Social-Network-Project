# Social Network Project

## Overview

This project is a simulation of a social network. It provides classes and functionalities for managing users, their relationships, and interactions through posts. The system allows for user creation, friend connections, posting, and network analysis, including shortest paths, friend suggestions, and groups.

## Features

### Network Management (`Network.h`)
- **User Management**
  - Add users to the network.
  - Retrieve user information by name or ID.
  - Count the number of users in the network.

- **Friend Connections**
  - Create and delete friend connections between users.
  - Suggest friends based on shared connections.

- **Posts**
  - Add posts with options for likes, privacy, and author details.
  - Read and write posts to/from files.

- **File Operations**
  - Import/export users, posts, and passwords from files.
  - Password encoding/decoding using Caesar cipher.

- **Network Analysis**
  - Calculate shortest paths between users.
  - Retrieve groups (connected components) in the network.

### User Management (`User.h`)
- **Profile Management**
  - Manage user profiles with attributes such as:
    - Name
    - ID
    - Birth year
    - Zip code
    - Friends list
  - Password management with encoding and decoding.

- **Post Management**
  - Add and retrieve user posts.
  - Display recent posts, filtering by privacy.

### Post System (`Post.h`)
- **Post Base Class**
  - Manage posts with attributes such as:
    - Message ID
    - Owner ID
    - Content
    - Likes
    - Privacy settings.

- **IncomingPost Class**
  - Specialized post type for messages authored by other users.
  - Additional fields for author name and public visibility.

## How to Use

1. **Initialize Network**: Create a `Network` object to manage users and connections.
2. **Add Users**: Use `addUser` to add new users to the network.
3. **Create Connections**: Use `addConnection` and `deleteConnection` to manage friendships.
4. **Posts**: Add posts for users with `addPost` and retrieve them using `getPostsString`.
5. **Network Analysis**:
   - Use `shortestPath` to find relational distances between users.
   - Use `groups` to analyze connected components in the network.
6. **File Operations**:
   - Import/export network data using `readUsers` and `writeUsers`.
   - Securely manage passwords with `readPasswords` and `writePasswords`.

## Project Structure

- **Header Files**
  - `Network.h`: Manages the overall network and its functionalities.
  - `User.h`: Defines user profiles and their interactions.
  - `Post.h`: Implements posts and derived classes for extended functionality.

- **Key Methods**
  - **User Management**: `addUser`, `getId`, `numUsers`
  - **Connections**: `addConnection`, `deleteConnection`
  - **Posts**: `addPost`, `getPostsString`, `readPosts`, `writePosts`
  - **File I/O**: `readUsers`, `writeUsers`, `readPasswords`, `writePasswords`

## Dependencies

- **Standard Libraries**: `<vector>`, `<iostream>`, `<string>`, `<set>`
- **Project-Specific Headers**: `user.h`, `post.h`

## Notes

- Ensure file paths are valid before calling read/write methods.
- Passwords are encoded using a Caesar cipher with a shift of 13 for basic obfuscation.

## Future Enhancements

- Implement advanced encryption for password security.
- Add a GUI or API for interaction with the network.
- Expand post capabilities with multimedia support.
