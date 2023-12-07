## Brief

This is a simple IoT Platform project monitoring IoT devices through a web interface using backend server built with Express.js, React, and the ESP8266 microcontroller code for client integration.

## Features

- **Real-time Communication:** Using WebSocket communication for real-time bidirectional data exchange between the server and connected devices.

- **User Interaction:** Enables users to perform actions in real-time, visualized on the React frontend.

- **Device Monitoring:** Provides a dynamic dashboard for visualizing device status, sensor data, and user controls.

- **User Authentication:** Ensures secure access to the platform through user authentication and role-based access control.

## Components

### 1. Express Backend (index.js)

- WebSocket communication with Socket.IO.
- RESTful APIs for device management and user interaction.
- Room-based messaging for users to join specific rooms.

### 2. React Frontend (client folder)

- WebSocket integration for real-time updates.
- Dynamic dashboards for visualizing device status and sensor data.
- User authentication and role-based access control.

### 3. ESP8266 Client (nodemcu.ino)

- Wi-Fi connection to the specified network.
- Integration with the MPU6050 sensor for collecting accelerometer data.
- Socket.IO client for real-time communication with the backend server.

## Setup and Usage

1. **Backend Setup:**

   ```bash
   cd backend
   npm install
   npm start

2. **Frontend Setup:**
   ```bash
    cd frontend
    npm install
    npm start
   
3. **ESP8266 Setup:**
   Open the `nodemcu.ino` file in the Arduino IDE.

   Use the Arduino IDE to open the `nodemcu.ino` file located in the project's root directory.

   Set Wi-Fi Credentials and Adjust Configurations.

   In the `nodemcu.ino` file, locate the section where Wi-Fi credentials and other configurations are set. Modify the following lines with your Wi-Fi credentials:

   ```cpp
   /* Wi-Fi credentials */
   const char* wifiSSID = "YourWiFiSSID";
   const char* wifiPassword = "YourWiFiPassword";

# Appendices
![image](https://github.com/egyptianego17/IoT_App_ESP8266/assets/96279885/cf7e0da6-58e9-40c4-bdb4-b858359a9828)

