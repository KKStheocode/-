#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <sstream>
#include <unordered_set>
using namespace std;

// 时间类定义
class Time {
private:
    int hours, minutes, seconds;

public:
    Time(int h = 0, int m = 0, int s = 0) : hours(h), minutes(m), seconds(s) { normalizeTime(); }

    void setTime(int h, int m, int s) {
        hours = h;
        minutes = m;
        seconds = s;
        normalizeTime();
    }

    void normalizeTime() {
        if (seconds >= 60) {
            minutes += seconds / 60;
            seconds %= 60;
        }
        if (minutes >= 60) {
            hours += minutes / 60;
            minutes %= 60;
        }
    }

    friend ostream& operator<<(ostream& os, const Time& t) {
        os << setw(2) << setfill('0') << t.hours << ":"
            << setw(2) << setfill('0') << t.minutes << ":"
            << setw(2) << setfill('0') << t.seconds;
        return os;
    }

    friend istream& operator>>(istream& is, Time& t) {
        char colon1, colon2;
        is >> t.hours >> colon1 >> t.minutes >> colon2 >> t.seconds;
        if (colon1 != ':' || colon2 != ':') {
            is.setstate(ios::failbit);
        }
        t.normalizeTime();
        return is;
    }
};

// 日期类定义
class Date {
private:
    int year, month, day;

public:
    Date(int y = 0, int m = 0, int d = 0) : year(y), month(m), day(d) {}

    friend ostream& operator<<(ostream& os, const Date& d) {
        os << d.year << "年" << setw(2) << setfill('0') << d.month << "月"
            << setw(2) << setfill('0') << d.day << "日";
        return os;
    }

    friend istream& operator>>(istream& is, Date& d) {
        is >> d.year >> d.month >> d.day;
        return is;
    }
};

// 图书类定义
class Book {
public:
    string id, title, author, publisher, isbn;
    int pages;
    bool isAvailable;

    Book(string i, string t, string a, string p, string is, int pg)
        : id(i), title(t), author(a), publisher(p), isbn(is), pages(pg), isAvailable(true) {}

    void display() const {
        cout << "\n-------- 图书信息 --------\n";
        cout << "编号: " << id << "\n标题: " << title << "\n作者: " << author
            << "\n出版商: " << publisher << "\nISBN号: " << isbn << "\n总页数: " << pages
            << "\n状态: " << (isAvailable ? "可借" : "已借出") << endl;
    }
};

// 光盘类定义
class CD {
public:
    string id, title, author, producer;
    int year;
    Time videoLength;
    bool isAvailable;

    CD(string i, string t, string a, string p, int y, Time vl)
        : id(i), title(t), author(a), producer(p), year(y), videoLength(vl), isAvailable(true) {}

    void display() const {
        cout << "\n-------- 光盘信息 --------\n";
        cout << "编号: " << id << "\n标题: " << title << "\n作者: " << author
            << "\n出品者: " << producer << "\n出品年份: " << year << "\n视频时长: " << videoLength
            << "\n状态: " << (isAvailable ? "可借" : "已借出") << endl;
    }
};

// 借阅记录类定义
class BorrowRecord {
public:
    string userId, itemId;
    Date borrowDate, returnDate;
    bool isReturned;

    BorrowRecord(string u, string i, Date borrow, Date returnTime)
        : userId(u), itemId(i), borrowDate(borrow), returnDate(returnTime), isReturned(false) {}

    void markAsReturned() {
        isReturned = true;
    }
};

// 用户类定义
class User {
public:
    string id, name;
    string password;  // 添加密码字段
    vector<BorrowRecord> borrowRecords;
    bool isLoggedIn;  // 添加登录状态

    User(string uid, string uname, string pwd="") : id(uid), name(uname), password(pwd), isLoggedIn(false) {}

    void addBorrowRecord(BorrowRecord record) {
        borrowRecords.push_back(record);
    }

    void display() const {
        cout << "用户ID: " << id << ", 姓名: " << name << endl;
    }

    void displayBorrowHistory() const {
        cout << "\n借阅历史:\n";
        for (const auto& record : borrowRecords) {
            cout << "图书/光盘ID: " << record.itemId << ", 借阅日期: " << record.borrowDate
                << ", 归还日期: " << record.returnDate << ", 是否归还: "
                << (record.isReturned ? "已归还" : "未归还") << endl;
        }
    }

    // 验证密码
    bool verifyPassword(const string& pwd) const {
        return password == pwd;
    }

    // 设置登录状态
    void login() {
        isLoggedIn = true;
    }

    // 设置注销状态
    void logout() {
        isLoggedIn = false;
    }
};

// 媒体管理系统类定义
class MediaManager {
private:
    vector<Book> books;
    vector<CD> cds;
    vector<User> users;
    vector<BorrowRecord> borrowRecords;
    const int MAX_CAPACITY = 100;

    // 存储图书和光盘的ID集合，用来避免ID冲突
    unordered_set<string> bookIds;
    unordered_set<string> cdIds;

    // 查找图书
    Book* findBookById(const string& id) {
        for (auto& book : books) {
            if (book.id == id) return &book;
        }
        return nullptr;
    }

    // 查找光盘
    CD* findCdById(const string& id) {
        for (auto& cd : cds) {
            if (cd.id == id) return &cd;
        }
        return nullptr;
    }

    // 查找用户
    User* findUserById(const string& id) {
        for (auto& user : users) {
            if (user.id == id) return &user;
        }
        return nullptr;
    }

public:
    // 添加物品
    void addItem() {
        if (books.size() + cds.size() >= MAX_CAPACITY) {
            cout << "物品库已满，不能再添加新的数据。\n";
            return;
        }

        char type;
        cout << "\n请输入物品类型（B: 图书, C: 光盘）: ";
        cin >> type;

        if (type == 'B') {
            string id, title, author, publisher, isbn;
            int pages;
            cout << "输入编号: ";
            cin >> id;

            // 检查图书ID是否已存在
            if (bookIds.find(id) != bookIds.end() || cdIds.find(id) != cdIds.end()) {
                cout << "ID已存在，请重新输入不同的图书ID。\n";
                return;
            }

            cout << "输入标题: "; cin >> title;
            cout << "输入作者: "; cin >> author;
            cout << "输入出版社: "; cin >> publisher;
            cout << "输入ISBN号: "; cin >> isbn;
            cout << "输入页数: "; cin >> pages;

            // 添加图书
            books.emplace_back(id, title, author, publisher, isbn, pages);
            bookIds.insert(id);  // 将图书ID添加到ID集合中
            cout << "图书添加成功。\n";
        }
        else if (type == 'C') {
            string id, title, author, producer;
            int year;
            Time videoLength;

            cout << "输入编号: ";
            cin >> id;

            // 检查光盘ID是否已存在
            if (cdIds.find(id) != cdIds.end() || bookIds.find(id) != bookIds.end()) {
                cout << "ID已存在，请重新输入不同的光盘ID。\n";
                return;
            }

            cout << "输入标题: "; cin >> title;
            cout << "输入作者: "; cin >> author;
            cout << "输入出品者名字: "; cin >> producer;
            cout << "输入出品年份: "; cin >> year;
            cout << "输入视频时长 (hh:mm:ss): "; cin >> videoLength;

            // 添加光盘
            cds.emplace_back(id, title, author, producer, year, videoLength);
            cdIds.insert(id);  // 将光盘ID添加到ID集合中
            cout << "光盘添加成功。\n";
        }
        else {
            cout << "无效的物品类型，请重新输入。\n";
        }
    }

    // 查询物品
    void queryItem() {
        char queryType;
        cout << "\n请输入查询方式（T: 按标题, C: 按类别）: ";
        cin >> queryType;

        if (queryType == 'T') {
            string title;
            cout << "输入标题: "; cin >> title;
            bool found = false;

            for (const auto& book : books) {
                if (book.title == title) {
                    book.display();
                    found = true;
                }
            }

            for (const auto& cd : cds) {
                if (cd.title == title) {
                    cd.display();
                    found = true;
                }
            }

            if (!found) {
                cout << "没有找到对应的标题。\n";
            }
        }
        else if (queryType == 'C') {
            char category;
            cout << "请输入类别（B: 图书, C: 光盘）: "; cin >> category;
            bool found = false;

            if (category == 'B') {
                for (const auto& book : books) {
                    book.display();
                    found = true;
                }
            }
            else if (category == 'C') {
                for (const auto& cd : cds) {
                    cd.display();
                    found = true;
                }
            }

            if (!found) {
                cout << "该类别没有物品。\n";
            }
        }
        else {
            cout << "无效的查询方式，请重新输入。\n";
        }
    }

    // 显示所有物品
    void displayAll() {
        cout << "\n-------- 当前物品库 --------\n";
        cout << "图书列表:\n";
        if (books.empty()) {
            cout << "暂无图书。\n";
        }
        else {
            for (const auto& book : books) {
                book.display();
            }
        }
        cout << "\n光盘列表:\n";
        if (cds.empty()) {
            cout << "暂无光盘。\n";
        }
        else {
            for (const auto& cd : cds) {
                cd.display();
            }
        }
    }

    // 用户注册
    void registerUser() {
        string id, name, password;
        cout << "请输入用户ID: "; cin >> id;
        cout << "请输入用户姓名: "; cin >> name;
        cout << "请输入用户密码: "; cin >> password;

        if (findUserById(id)) {
            cout << "该用户已存在。\n";
        }
        else {
            users.emplace_back(id, name, password);  // 使用密码来创建用户
            cout << "用户注册成功。\n";
        }
    }


    // 用户借书
    void borrowItem() {
        string userId, itemId;
        cout << "请输入用户ID: "; cin >> userId;
        cout << "请输入借阅物品ID: "; cin >> itemId;
        User* user = findUserById(userId);
        if (!user) {
            cout << "用户未找到。\n";
            return;
        }
        Book* book = findBookById(itemId);
        CD* cd = findCdById(itemId);
        if (!book && !cd) {
            cout << "该物品不存在。\n";
            return;
        }
        if (book && !book->isAvailable) {
            cout << "该图书已被借出。\n";
            return;
        }
        if (cd && !cd->isAvailable) {
            cout << "该光盘已被借出。\n";
            return;
        }
        Date borrowDate, returnDate;
        cout << "请输入借阅日期 (XX年XX月XX日): "; cin >> borrowDate;
        cout << "请输入归还日期 (XX年XX月XX日): "; 
        while (!(cin >> returnDate)) { cout << "输入错误,请重试" << endl; };

        if (book) {
            book->isAvailable = false;
        }
        if (cd) {
            cd->isAvailable = false;
        }
        BorrowRecord record(userId, itemId, borrowDate, returnDate);
        user->addBorrowRecord(record);
        borrowRecords.push_back(record);
        cout << "借阅成功。\n";
    }

    // 用户还书
    void returnItem() {
        string userId, itemId;
        cout << "请输入用户ID: "; cin >> userId;
        cout << "请输入归还物品ID: "; cin >> itemId;

        // 查找用户
        User* user = findUserById(userId);
        if (!user) {
            cout << "用户未找到。\n";
            return;
        }

        // 查找借阅记录
        BorrowRecord* record = nullptr;
        for (auto& r : user->borrowRecords) {
            if (r.itemId == itemId && !r.isReturned) {
                record = &r;
                break;
            }
        }

        if (!record) {
            cout << "未找到该物品的借阅记录或物品已归还。\n";
            return;
        }

        // 查找物品
        Book* book = findBookById(itemId);
        CD* cd = findCdById(itemId);

        if (!book && !cd) {
            cout << "该物品不存在。\n";
            return;
        }

        // 更新物品状态
        if (book) {
            book->isAvailable = true;
        }
        if (cd) {
            cd->isAvailable = true;
        }

        // 更新借阅记录状态
        record->markAsReturned();
        cout << "物品已成功归还。\n";
    }

    // 删除物品
    void deleteItem() {
        char type;
        cout << "\n请输入物品类型（B: 图书, C: 光盘）: ";
        cin >> type;

        string itemId;
        cout << "请输入物品ID: ";
        cin >> itemId;

        if (type == 'B') {
            Book* book = findBookById(itemId);
            if (book) {
                if (!book->isAvailable) {
                    cout << "该图书已被借出，不能删除。\n";
                    return;
                }
                // 删除图书
                books.erase(remove_if(books.begin(), books.end(),
                    [&itemId](const Book& b) { return b.id == itemId; }), books.end());
                bookIds.erase(itemId);  // 从ID集合中删除
                cout << "图书删除成功。\n";
            }
            else {
                cout << "未找到该图书。\n";
            }
        }
        else if (type == 'C') {
            CD* cd = findCdById(itemId);
            if (cd) {
                if (!cd->isAvailable) {
                    cout << "该光盘已被借出，不能删除。\n";
                    return;
                }
                // 删除光盘
                cds.erase(remove_if(cds.begin(), cds.end(),
                    [&itemId](const CD& c) { return c.id == itemId; }), cds.end());
                cdIds.erase(itemId);  // 从ID集合中删除
                cout << "光盘删除成功。\n";
            }
            else {
                cout << "未找到该光盘。\n";
            }
        }
        else {
            cout << "无效的物品类型，请重新输入。\n";
        }
    }


    // 显示借阅历史
    void displayBorrowHistory() {
        string userId;
        cout << "请输入用户ID查看借阅历史: "; cin >> userId;
        User* user = findUserById(userId);
        if (!user) {
            cout << "用户未找到。\n";
            return;
        }
        user->displayBorrowHistory();
    }

    // 修改物品
    void modifyItem() {
        char type;
        cout << "\n请输入物品类型（B: 图书, C: 光盘）: ";
        cin >> type;

        string itemId;
        cout << "请输入物品ID: ";
        cin >> itemId;

        if (type == 'B') {
            Book* book = findBookById(itemId);
            if (!book) {
                cout << "未找到该图书。\n";
                return;
            }
            // 修改图书信息
            cout << "请输入新的标题（留空不修改）: ";
            string newTitle;
            cin.ignore();
            getline(cin, newTitle);
            if (!newTitle.empty()) book->title = newTitle;

            cout << "请输入新的作者（留空不修改）: ";
            string newAuthor;
            getline(cin, newAuthor);
            if (!newAuthor.empty()) book->author = newAuthor;

            cout << "请输入新的出版社（留空不修改）: ";
            string newPublisher;
            getline(cin, newPublisher);
            if (!newPublisher.empty()) book->publisher = newPublisher;

            cout << "请输入新的ISBN号（留空不修改）: ";
            string newIsbn;
            getline(cin, newIsbn);
            if (!newIsbn.empty()) book->isbn = newIsbn;

            cout << "请输入新的页数（留空不修改）: ";
            int newPages;
            if (cin >> newPages) {
                book->pages = newPages;
            }
            cout << "图书信息已修改。\n";
        }
        else if (type == 'C') {
            CD* cd = findCdById(itemId);
            if (!cd) {
                cout << "未找到该光盘。\n";
                return;
            }
            // 修改光盘信息
            cout << "请输入新的标题（留空不修改）: ";
            string newTitle;
            cin.ignore();
            getline(cin, newTitle);
            if (!newTitle.empty()) cd->title = newTitle;

            cout << "请输入新的作者（留空不修改）: ";
            string newAuthor;
            getline(cin, newAuthor);
            if (!newAuthor.empty()) cd->author = newAuthor;

            cout << "请输入新的出品者（留空不修改）: ";
            string newProducer;
            getline(cin, newProducer);
            if (!newProducer.empty()) cd->producer = newProducer;

            cout << "请输入新的出品年份（留空不修改）: ";
            int newYear;
            if (cin >> newYear) {
                cd->year = newYear;
            }

            cout << "请输入新的视频时长 (hh:mm:ss)（留空不修改）: ";
            Time newVideoLength;
            cin >> newVideoLength;
                cd->videoLength = newVideoLength;

            cout << "光盘信息已修改。\n";
        }
        else {
            cout << "无效的物品类型，请重新输入。\n";
        }
    }
    // 保存数据到文件
    void saveData() {
        ofstream outFile("media_data.txt");

        // 保存图书信息
        outFile << "Books\n";
        for (const auto& book : books) {
            outFile << book.id << "," << book.title << "," << book.author << "," << book.publisher
                << "," << book.isbn << "," << book.pages << "," << (book.isAvailable ? 1 : 0) << "\n";
        }

        // 保存光盘信息
        outFile << "CDs\n";
        for (const auto& cd : cds) {
            outFile << cd.id << "," << cd.title << "," << cd.author << "," << cd.producer
                << "," << cd.year << "," << cd.videoLength << "," << (cd.isAvailable ? 1 : 0) << "\n";
        }

        // 保存用户信息
        outFile << "Users\n";
        for (const auto& user : users) {
            outFile << user.id << "," << user.name << "\n";
            for (const auto& record : user.borrowRecords) {
                outFile << record.userId << "," << record.itemId << "," << record.borrowDate
                    << "," << record.returnDate << "," << (record.isReturned ? 1 : 0) << "\n";
            }
        }

        outFile.close();
    }

    // 从文件加载数据
    void loadData() {
        ifstream inFile("media_data.txt");
        string line;
        string section;

        while (getline(inFile, line)) {
            if (line == "Books") {
                section = "Books";
                continue;
            }
            else if (line == "CDs") {
                section = "CDs";
                continue;
            }
            else if (line == "Users") {
                section = "Users";
                continue;
            }

            istringstream ss(line);
            if (section == "Books") {
                string id, title, author, publisher, isbn;
                int pages, isAvailable;

                // Using getline to handle comma-separated values
                getline(ss, id, ',');
                getline(ss, title, ',');
                getline(ss, author, ',');
                getline(ss, publisher, ',');
                getline(ss, isbn, ',');
                ss >> pages;
                ss.ignore(1, ','); // Ignore the comma before 'isAvailable'
                ss >> isAvailable;

                books.emplace_back(id, title, author, publisher, isbn, pages);
                books.back().isAvailable = isAvailable;
            }
            else if (section == "CDs") {
                string id, title, author, producer;
                int year, isAvailable;
                Time videoLength;

                // Using getline to handle comma-separated values
                getline(ss, id, ',');
                getline(ss, title, ',');
                getline(ss, author, ',');
                getline(ss, producer, ',');
                ss >> year;
                ss.ignore(1, ','); // Ignore the comma before 'videoLength'
                ss >> videoLength;
                ss.ignore(1, ','); // Ignore the comma before 'isAvailable'
                ss >> isAvailable;

                cds.emplace_back(id, title, author, producer, year, videoLength);
                cds.back().isAvailable = isAvailable;
            }
            else if (section == "Users") {
                string id, name;
                getline(ss, id, ',');
                getline(ss, name, ',');

                users.emplace_back(id, name);

                // Handle borrowing records
                while (getline(inFile, line) && !line.empty()) {
                    istringstream recordStream(line);
                    string userId, itemId;
                    Date borrowDate, returnDate;
                    bool isReturned;

                    // Parsing the borrowing record
                    getline(recordStream, userId, ',');
                    getline(recordStream, itemId, ',');
                    recordStream >> borrowDate;
                    recordStream.ignore(1, ','); // Ignore the comma before returnDate
                    recordStream >> returnDate;
                    recordStream.ignore(1, ','); // Ignore the comma before isReturned
                    recordStream >> isReturned;

                    BorrowRecord record(userId, itemId, borrowDate, returnDate);
                    record.isReturned = isReturned;
                    users.back().addBorrowRecord(record);
                }
            }
        }

        inFile.close();
    }

    User* loggedInUser = nullptr;  // 当前登录的用户

    void loginUser() {
        if (loggedInUser) {
            cout << "已经登录的用户: " << loggedInUser->name << endl;
            return;
        }

        string id, password;
        cout << "请输入用户ID: "; cin >> id;
        cout << "请输入密码: "; cin >> password;

        User* user = findUserById(id);
        if (user && user->verifyPassword(password)) {
            loggedInUser = user;
            loggedInUser->login();
            cout << "登录成功！\n";
        }
        else {
            cout << "登录失败：用户名或密码错误。\n";
        }
    }

    void logoutUser() {
        if (loggedInUser) {
            loggedInUser->logout();
            cout << "注销成功！\n";
            loggedInUser = nullptr;
        }
        else {
            cout << "当前没有用户登录。\n";
        }
    }
};

int main() {
    MediaManager manager;
    manager.loadData(); // 启动时加载数据
    int choice;
    do {
        cout << "\n-------- 图书馆媒体管理系统 --------\n";
        cout << "1. 注册用户\n";
        cout << "2. 登录\n";
        cout << "3. 注销\n";
        cout << "4. 添加物品\n";
        cout << "5. 查询物品\n";
        cout << "6. 显示物品库\n";
        cout << "7. 借阅物品\n";
        cout << "8. 显示借阅历史\n";
        cout << "9. 归还物品\n";
        cout << "10. 删除物品\n";
        cout << "11. 修改物品\n";  // 新增修改物品选项
        cout << "0. 保存并退出\n";
        cout << "选择操作: ";
        while (true) {
            cin >> choice;
            if (cin.fail()) {
                cin.clear(); // 清除错误标志
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 丢弃无效输入
                cout << "无效输入，请输入一个数字。\n";
            }
            else if (choice < 0 || choice > 11) {
                cout << "选择无效，请重新输入。\n";
            }
            else {
                break; // 有效输入，退出循环
            }
        }

        switch (choice) {
        case 1: manager.registerUser(); break;
        case 2: manager.loginUser(); break;
        case 3: manager.logoutUser(); break;
        case 4:
            if (manager.loggedInUser) {
                manager.addItem();
            }
            else {
                cout << "请先登录。\n";
            }
            break;
        case 5:
            if (manager.loggedInUser) {
                manager.queryItem();
            }
            else {
                cout << "请先登录。\n";
            }
            break;
        case 6:
            if (manager.loggedInUser) {
                manager.displayAll();
            }
            else {
                cout << "请先登录。\n";
            }
            break;
        case 7:
            if (manager.loggedInUser) {
                manager.borrowItem();
            }
            else {
                cout << "请先登录。\n";
            }
            break;
        case 8:
            if (manager.loggedInUser) {
                manager.displayBorrowHistory();
            }
            else {
                cout << "请先登录。\n";
            }
            break;
        case 9:
            if (manager.loggedInUser) {
                manager.returnItem();
            }
            else {
                cout << "请先登录。\n";
            }
            break;
        case 10:
            if (manager.loggedInUser) {
                manager.deleteItem();
            }
            else {
                cout << "请先登录。\n";
            }
            break;
        case 11:
            if (manager.loggedInUser) {
                manager.modifyItem();
            }
            else {
                cout << "请先登录。\n";
            }
            break;
        case 0:
            cout << "保存数据并退出系统。\n";
            manager.saveData(); // 退出时保存数据
            break;
        default:
            cout << "无效选择，请重新输入！\n";
        }
        system("pause");
        system("cls");
    } while (choice != 0);
    return 0;
}