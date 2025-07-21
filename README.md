Gummiphone: A Console-Based Messaging System
=============================================
A foundational C project simulating a multi-user messaging platform with user management, messaging, and administrative controls.

--------------------------------------------------------------------------------

🌟 PROJECT OVERVIEW
------------------

Gummiphone is a console-based messaging system developed entirely in C. It provides a simulated environment for a multi-user platform, where data for users, messages, and system configurations are persisted to local text files. The project serves as a practical exercise in C programming, demonstrating concepts such as file I/O, structured data types, string manipulation, and menu-driven console applications.

Key features include:
  * User Management: Account creation, login, profile modification, and a "forgot password" flow handled by an administrator.
  * Messaging: Sending and receiving personal messages, group messages, and system-wide announcements from the admin.
  * Connection Management: Users can maintain a personal list of connections (friends).
  * Administrative Panel: A secure module for system oversight, including user management, message moderation, and password resets.


🎯 MOTIVATION
------------

The primary motivations behind developing Gummiphone include:

  * Mastering C Programming: To apply and solidify fundamental C concepts like pointers, structs, file handling, and memory management.
  * System Simulation: To build a basic simulation of a real-world application from scratch, understanding the logic behind authentication, message flow, and data persistence.
  * Data Management: To gain experience loading from and saving application state to external files.
  * Modular Design: To practice structuring a larger C program into distinct, readable functions.


🛠️ TECHNOLOGIES USED
-------------------

  * C Language: The entire application is written in C.
  * Standard C Libraries: 'stdio.h', 'stdlib.h', 'string.h', and 'time.h'.
  * 'conio.h': Used for non-buffered console input, making the project best suited for Windows/DOS environments.


🚀 GETTING STARTED
-----------------

To get a local copy of Gummiphone up and running, follow these steps.

### Prerequisites

  * C Compiler: A C compiler such as GCC or MinGW (for Windows).
  * Windows Operating System: Recommended due to the use of 'conio.h' and 'system("cls")'.

### Data Files

The system relies on three text files for data persistence, which will be created automatically on the first run if they don't exist:
  * 'users.txt': Stores all registered user data.
  * 'admin.txt': Stores administrator configuration and security questions.
  * 'msgs.txt': Stores all messages (personal, group, announcements).

### Compilation and Running

1.  Save the source code (e.g., as 'gummiphone.c') in a directory.
2.  Open your terminal and navigate to that directory.
3.  Compile the source file:

        gcc gummiphone.c -o gummiphone.exe

4.  Run the compiled executable:

        ./gummiphone.exe

    Note: On the first run, the system will prompt you to set an initial administrator password.


💬 USAGE
-------

Upon running the application, you will be presented with a main menu. Navigate through the system by entering the number corresponding to your desired action.

  * Main Menu: Provides options for user login, account creation, password reset requests, and admin login.
  * User Menu (after login): Access features like composing messages, viewing your inbox and sent items, managing personal connections, and modifying your account.
  * Admin Menu (after admin login): Access modules for managing all users and messages, handling password reset requests, and modifying the admin password.
