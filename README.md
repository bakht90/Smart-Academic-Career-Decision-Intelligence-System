# Smart Academic & Career Decision Intelligence System

This version keeps the existing HTML/CSS interface and C++ OOP/file-handling core, but connects them through a small HTTP API implemented directly in `server.cpp`.

## Architecture

```text
Browser (HTML/CSS/JavaScript)
        |
      fetch()
        |
   server.cpp (HTTP API)
        |
 Student / Academic / Career / Dashboard
        |
      .txt files
```

There is no SQL database and no Crow dependency.

## Run the web version

1. Make sure MinGW g++ is installed and `g++` works in Command Prompt.
2. Double-click `build_server.bat`.
3. Run the generated `server.exe` from the project root.
4. Open:

   `http://localhost:18080`

The server also serves the frontend, so **do not open `index.html` by double-clicking it** for normal use.

## Run the console version

Run `build_console.bat`, then run the generated `project.exe` from the project root.

## Data files

- `data/users.txt` — student records
- `data/assignments.txt` — student assignments
- `data/skills.txt` — student skill selections
- `data/attendance.txt` — retained for the project structure; current attendance is stored with the student's record in `users.txt`

Assignments use:

```text
username|assignment name|Pending/Completed
```

Skills use:

```text
username|skill
```

## Important migration behavior

The old browser prototype used `localStorage`. This version uses C++ as the source of truth.

If an old `smartAcademicUser` / `smartAcademicAssignments` record is still present in the browser, the new frontend attempts a one-time migration after login so existing prototype assignments and the selected skill are not simply lost.

## Main API routes

- `POST /api/register`
- `POST /api/login`
- `GET /api/profile?username=...`
- `GET /api/dashboard?username=...`
- `POST /api/academic/update`
- `POST /api/assignments/add`
- `POST /api/assignments/status`
- `POST /api/career`
- `POST /api/gpa`

## Notes

Passwords remain plain text because this is a semester project using `.txt` file handling. A production system should use password hashing and stronger session/authentication handling.
