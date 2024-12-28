[README.txt](https://github.com/user-attachments/files/18266261/README.txt)
media_data为数据储存文档,
Untitled-1.cpp为源文件
同名.exe为运行程序
# Library Media Management System

## Introduction
The "Library Media Management System" is a console application designed to manage the borrowing of books and CDs in a library. This system allows users to register, log in, add items, query items, borrow and return items, view borrowing history, delete and modify items, and more. All data is stored in a local file and loaded and saved each time the program starts and exits.

## Features
1. **User Registration**
    - Users can register new accounts by providing a user ID, name, and password.
2. **User Login/Logout**
    - Users can log in using their ID and password. They must be logged in to perform other operations.
    - Users can log out of the current session.
3. **Add Items**
    - Administrators can add new books or CDs to the system.
4. **Query Items**
    - Users can search for items by title or category.
5. **Display All Items**
    - Display all books and CDs in the library.
6. **Borrow Items**
    - Users can borrow books or CDs by providing the borrowing and return dates.
7. **View Borrowing History**
    - View a user's borrowing history, including borrowing and return dates and item status.
8. **Return Items**
    - Users can return borrowed books or CDs.
9. **Delete Items**
    - Administrators can delete books or CDs, but items that are borrowed cannot be deleted.
10. **Modify Items**
    - Administrators can modify the information of books or CDs.
11. **Save and Load Data**
    - The system loads data from a file on startup and saves data to the file on exit.

## Usage Instructions

### Running the Program
Compile and run the program, which will automatically load data on startup.

### Main Menu
After starting the program, the main menu will be displayed. Users can choose from the following operations:

1. **Register User**: Register a new user.
2. **Login**: Log in to the system.
3. **Logout**: Log out of the current session.
4. **Add Items**: Add new books or CDs.
5. **Query Items**: Search for items by title or category.
6. **Display All Items**: Display all books and CDs.
7. **Borrow Items**: Borrow books or CDs.
8. **View Borrowing History**: View a user's borrowing history.
9. **Return Items**: Return borrowed books or CDs.
10. **Delete Items**: Delete books or CDs.
11. **Modify Items**: Modify the information of books or CDs.
0. **Save and Exit**: Save data and exit the system.

### Adding New Items
After selecting the option to add items, enter the detailed information of the book or CD, such as ID, title, author, publisher, ISBN, etc. The system will check if the ID is unique and add the new item to the system.

### Borrowing and Returning Items
After logging in, users can borrow books or CDs by entering the borrowing and return dates. When returning items, the system will update the borrowing status accordingly.

### Saving and Loading Data
The system automatically loads data from the `media_data.txt` file on startup and saves data to the file on exit.

## File Structure

- `media_data.txt`: Stores information about books, CDs, users, and borrowing records.
- `main.cpp`: The main program file containing all class definitions and program logic.

## Contribution
If you have any suggestions or improvements, please submit a pull request or issue.

## License
This project is licensed under the MIT License.

---

Thank you for using the Library Media Management System!
