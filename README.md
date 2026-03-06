# Training Management Desktop Application

<div align="center">

![Qt](https://img.shields.io/badge/Qt-6.7.3-41CD52?style=for-the-badge&logo=qt&logoColor=white)
![C++](https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![OpenCV](https://img.shields.io/badge/OpenCV-Face%20Recognition-5C3EE8?style=for-the-badge&logo=opencv&logoColor=white)
![Arduino](https://img.shields.io/badge/Arduino-Integration-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

A full-featured Qt desktop application for managing training centers — covering clients, employees, training sessions, meetings, resources, statistics, and hardware integration.

</div>

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Tech Stack](#tech-stack)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Database Setup](#database-setup)
- [Build Instructions](#build-instructions)
- [Configuration](#configuration)
- [Modules](#modules)
  - [Authentication](#authentication)
  - [Client Management](#client-management)
  - [Employee Management](#employee-management)
  - [Training Management](#training-management)
  - [Meeting Management](#meeting-management)
  - [Resource Management](#resource-management)
  - [Charts & Statistics](#charts--statistics)
  - [Arduino Integration](#arduino-integration)
  - [Email & Notifications](#email--notifications)
- [Key Classes](#key-classes)
- [Contributing](#contributing)

---

## Overview

**TrainingManagement** is a C++/Qt desktop application built for the complete operational management of a training center. It provides a unified interface to handle every aspect of day-to-day operations:

- Authenticate staff with password **or face recognition** (OpenCV)
- Manage **clients**, **employees**, **training sessions**, **meetings**, and **physical resources**
- Generate **QR codes**, export **PDF reports**, and send **email/SMS notifications**
- Display live **animated statistics charts**
- Interface with an **Arduino Uno** to drive a physical waiting room counter display
- Run an integrated **AI chatbot** and **AI image analysis** for resource classification

---

## Features

| Feature | Description |
|---|---|
| Face Recognition Login | OpenCV LBPH face recognizer + Haar cascade |
| Role-Based Access | Roles set at login, control UI visibility |
| Light / Dark Theme | Toggle in the main window |
| Full CRUD for all entities | Clients, Employees, Trainings, Meetings, Resources |
| PDF Export | Available for all major entity lists |
| QR Code Generation | Employee profile cards and meeting info |
| Email Reminders | Gmail SMTP/SSL to send client reminders |
| SMS Notifications | HTTP-based SMS for training session alerts |
| AI Chatbot | In-app chat panel connected to an external AI API |
| AI Image Analysis | Auto-classify/identify resources by image upload |
| Animated Charts | Pie and bar charts with hover tooltips |
| Arduino Waiting Room | Arduino Uno displays live training waiting room count |
| Consultation Calendar | Date-based client appointment view |
| In-App Notifications | Bell icon with live badge count |

---

## Tech Stack

| Layer | Technology |
|---|---|
| UI Framework | Qt 6.7.3 (Widgets, Charts, PrintSupport) |
| Language | C++17 |
| Build System | qmake |
| Compiler | MinGW 64-bit |
| Database | ODBC DSN (`Source_projet`) via Qt QODBC driver |
| Computer Vision | OpenCV (LBPH Face Recognizer, Haar Cascade) |
| Hardware | Qt SerialPort — Arduino Uno (VID 9025 / PID 67) |
| Email | SMTP over SSL — `smtp.gmail.com:465` |
| Networking | `QNetworkAccessManager` — AI API, SMS API |
| QR Code | Bundled `qrcodegen` C++ library |

---

## Project Structure

```
DesktopAPP/
├── client.pro                  # qmake project file (target: TrainingManagement)
├── export.sql                  # Database schema and seed data
├── sign.h / sign.cpp / sign.ui # Login screen (password + face ID)
├── img.qrc                     # Embedded image resources
│
├── core/
│   ├── main.cpp                # Application entry point
│   ├── connection.h/cpp        # ODBC database singleton (Connection class)
│   ├── emailsender.h/cpp       # SMTP/SSL Gmail email sender
│   ├── notificationmanager.h/cpp # In-app notification queue
│   └── customsqlquerymodel.h/cpp # Extended QSqlQueryModel
│
├── managers/                   # Business logic layer
│   ├── client/                 # Client entity + ClientManager
│   ├── employee/               # Employee entity + EmployeeManager
│   ├── training/               # Formation entity + TrainingManager + WaitingRoomDialog
│   ├── meeting/                # Meeting entity + MeetingManager
│   └── resources/              # Resource entity + ResourceManager
│
├── ui/
│   ├── mainwindow/             # Root window (aggregates all managers)
│   ├── chartwindow/            # Standalone animated statistics window
│   ├── search/                 # Global search dialog
│   ├── employeeArd/            # Arduino class (employee section)
│   └── waitingArd/             # Arduinoy class (training waiting room)
│
├── dialog/                     # Update/edit dialogs for each entity
│   ├── updateclientdialog/
│   ├── updateemployee/
│   ├── updatemeeting/
│   ├── updateresourcesdialog/
│   └── updatetrainingdialog/
│
└── lib/
    └── qrcodegen/              # Bundled QR code generation library
```

---

## Prerequisites

Before building, ensure the following are installed and configured:

1. **Qt 6.7.3** with the following modules:
   - `Qt Core`, `Qt GUI`, `Qt Widgets`
   - `Qt SQL` (with QODBC driver)
   - `Qt Network`
   - `Qt Charts`
   - `Qt PrintSupport`
   - `Qt SerialPort`

2. **MinGW 64-bit** compiler (bundled with Qt installer)

3. **OpenCV** (with face module):
   - Install to `C:/opencv/build/install/`
   - Required for face recognition login

4. **ODBC Data Source** configured on the machine:
   - DSN name: `Source_projet`
   - See [Database Setup](#database-setup) for details

5. **Arduino IDE / drivers** (optional):
   - Only needed if you have the physical Arduino hardware

---

## Database Setup

1. Open **ODBC Data Source Administrator** (Windows — search `ODBC` in Start menu).
2. Create a **System DSN** named exactly `Source_projet`.
3. Point it to your database server/file.
4. Import the schema using the provided `export.sql` file:

```sql
-- Run export.sql against your database to create tables and seed initial data
```

5. Ensure the following credentials are configured in `core/connection.cpp`:
   - **User:** `ahmed`
   - **Password:** `2004`

> **Note:** For production use, move credentials out of source code into environment variables or a config file.

---

## Build Instructions

### Using Qt Creator (Recommended)

1. Open **Qt Creator**.
2. Click **File → Open File or Project** and select `client.pro`.
3. Select the **Desktop Qt 6.7.3 MinGW 64-bit** kit.
4. Click **Configure Project**.
5. Press **Ctrl+B** (or click the Build button) to compile.
6. Press **Ctrl+R** to run.

### Using the Command Line

```bash
# Navigate to the project directory
cd c:\projects\DesktopAPP

# Generate Makefile
qmake client.pro -spec win32-g++

# Build
mingw32-make -j4

# The executable will be in the build output directory
```

---

## Configuration

### Email (Gmail SMTP)

The email sender is configured in `core/emailsender.cpp`. Update the following fields with a valid Gmail account and [App Password](https://support.google.com/accounts/answer/185833):

```cpp
// In emailsender.cpp
QString smtpServer = "smtp.gmail.com";
int smtpPort = 465;
QString senderEmail = "your-email@gmail.com";
QString senderPassword = "your-app-password";
```

### Arduino

The Arduino Uno is auto-detected by USB Vendor ID and Product ID in `ui/waitingArd/arduinoy.cpp`:

```cpp
// Vendor ID: 9025 (Arduino LLC), Product ID: 67 (Uno)
```

No manual COM port configuration is needed — the application discovers the device automatically.

---

## Modules

### Authentication

The login screen (`sign`) provides two authentication methods:

- **Credential Login** — Enter email and CIN (national ID number). Validated against the `employees` table. Sets the employee role for the session.
- **Face Recognition Login** — Uses OpenCV's LBPH face recognizer. A Haar cascade detects the face in the webcam feed, compares it to stored employee face data, and logs in the matching employee.
- **Remember Me** — Stores last-used credentials via `QSettings`.

---

### Client Management

Manages all clients of the training center.

- Add, edit, delete, and view clients
- Multi-criteria search with sortable table columns
- **Consultation calendar** — pick a date to view all scheduled appointments
- Send **email reminders** to clients using the integrated email sender
- Export client list to **PDF**
- View statistics by category in an animated chart

---

### Employee Management

Full HR management for training center staff.

- Full CRUD — CIN, first/last name, date of birth, phone, email, gender, salary, hire date, specialty, role, and photo
- **Photo upload** with inline thumbnail rendering in the table
- **QR code generation** for employee profile cards
- Real-time input validation (CIN format, name, phone, email, salary range)
- Search by any field; sort by any column
- Statistics: by role, specialty, gender; average salary; total headcount
- **Arduino serial integration** — read data from a connected Arduino device

---

### Training Management

Manages all training sessions offered by the center.

- Full CRUD on training sessions
- Search, filter, sort, and export to **PDF**
- **SMS notifications** — trigger SMS alerts to participants via an HTTP API
- **Waiting room counter** — a dialog displays and controls the live waiting room count
- **Arduino Uno integration** — the current waiting room count is automatically pushed to the Arduino device display over serial USB

---

### Meeting Management

Handles internal and client-facing meetings.

- Full CRUD on meetings
- Search and sort by multiple criteria
- **QR code generation** encoding meeting details
- **Resource allocation** — link physical resources to a specific meeting
- Export the full meeting list to **PDF**
- Statistics by meeting category

---

### Resource Management

Manages all physical resources owned by the center (equipment, furniture, etc.).

- Full CRUD — name, type, brand, quantity, purchase date, image
- **AI image analysis** — upload a photo of a resource to an external AI API for automatic classification
- Image load and display utilities
- **History log** — tracks resource operations; export to file or clear log
- Export to **PDF**
- Statistics charts: by resource type, by acquisition date

---

### Charts & Statistics

The `ChartWindow` provides animated, standalone statistics views:

- **Animated pie charts** (`QPieSeries`) with hover tooltips on each slice
- **Animated bar charts** (`QBarSeries`) with hover tooltips on each bar
- Refresh and reset controls
- Filter by detail/category via combo boxes
- Charts are available for Clients, Trainings, Meetings, and more — accessible from the main window sidebar

---

### Arduino Integration

Two Arduino helper classes handle serial communication:

| Class | Location | Purpose |
|---|---|---|
| `Arduino` | `ui/employeeArd/` | Generic serial connect/read/write, used in the employee section |
| `Arduinoy` | `ui/waitingArd/` | Auto-discovers Arduino Uno by USB VID/PID; drives the waiting room counter display |

The Arduino Uno is used to show the training waiting room count on a physical display. The app writes the count to the serial port whenever it changes.

---

### Email & Notifications

**Email (`EmailSender`)**
- Connects to `smtp.gmail.com` on port `465` (SSL)
- Used by `ClientManager` to send scheduled reminder emails to clients
- Exposes a `lastSendSuccessful` flag for error handling

**In-App Notifications (`NotificationManager`)**
- A lightweight in-memory queue of `Notification` structs (title, description, details, row reference)
- Emits a `notificationCountChanged(int)` signal consumed by the main window's notification bell icon to show a live badge count

---

## Key Classes

| Class | File | Role |
|---|---|---|
| `Connection` | `core/connection.h` | ODBC database singleton — open/close connection |
| `sign` | `sign.h` | Login window — credentials + face recognition |
| `MainWindow` | `ui/mainwindow/mainwindow.h` | Root window — owns all managers, AI chat, notifications |
| `ClientManager` | `managers/client/clientmanager.h` | Client CRUD, calendar, email reminders, charts |
| `EmployeeManager` | `managers/employee/employeemanager.h` | Employee CRUD, QR code, photo, stats |
| `TrainingManager` | `managers/training/trainingmanager.h` | Training CRUD, SMS, waiting room, Arduino |
| `MeetingManager` | `managers/meeting/meetingmanager.h` | Meeting CRUD, QR code, resource linking, PDF |
| `ResourceManager` | `managers/resources/resourcemanager.h` | Resource CRUD, AI image analysis, history, PDF |
| `ChartWindow` | `ui/chartwindow/chartwindow.h` | Animated pie/bar statistics charts |
| `EmailSender` | `core/emailsender.h` | Low-level SMTP/SSL Gmail sender |
| `NotificationManager` | `core/notificationmanager.h` | In-app notification queue with signal |
| `Arduino` | `ui/employeeArd/arduino.h` | Generic Arduino serial abstraction |
| `Arduinoy` | `ui/waitingArd/arduinoy.h` | Arduino Uno auto-discovery + serial R/W |
| `formations` | `managers/training/formations.h` | Training session entity model |
| `CustomSqlQueryModel` | `core/customsqlquerymodel.h` | Extended `QSqlQueryModel` with styling support |

---

## Contributing

1. Fork the repository and create a feature branch from `main`.
2. Follow the existing code style (Qt naming conventions, C++17).
3. Test all changes against the `Source_projet` ODBC database.
4. Ensure the application compiles cleanly with no warnings under Qt Creator before submitting a pull request.

---

<div align="center">
Built with Qt 6.7.3 · C++17 · OpenCV · Arduino
</div>
