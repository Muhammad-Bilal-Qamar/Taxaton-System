This is my 3rd-semester project—a Tax Management System built with C++. I designed this tool to bridge the gap between simple data entry and a functional management system by combining Doubly Linked Lists for smooth memory management and File Handling to ensure data persists after the program closes. It’s built with two distinct user experiences: a secure Admin panel for database oversight and a User module where individuals can register, secure their profiles, and get instant tax breakdowns based on their filer status.

What’s Under the Hood?

Smart Data Management: I used a Doubly Linked List structure, allowing the system to traverse taxpayer records in both directions for faster searching and updates.

Data That Sticks: By integrating File I/O, all taxpayer information is automatically synced to a local file, so your data is right where you left it every time you run the app.

Built-in Tax Logic: The system doesn't just store data—it calculates it. It features dynamic logic that applies different tax brackets for filers versus non-filers across both monthly income and yearly assets.

Security First: I implemented a custom validation engine. It requires complex passwords (including uppercase letters, special characters, etc.) and uses input masking (asterisks) for sensitive fields, such as passwords.

Clean Input Validation: To prevent crashes, I added strict checks for CNIC (13 digits) and phone numbers (11 digits), ensuring the database stays clean and error-free.

Full CRUD Functionality: Admins have total control to Create, Read, Update, and Delete records through an intuitive console menu.

Tech Used
Language: C++
