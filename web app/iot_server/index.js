const express = require("express");
const app = express();
const http = require("http");
const { Server } = require("socket.io");
const cors = require("cors");
const os = require('os');

app.use(cors());

const server = http.createServer(app);

const io = new Server(server, {
  cors: {
    origin: "*",
    methods: ["GET", "POST"],
  },
});

io.on("connection", (socket) => {
  console.log(`User Connected: ${socket.id}`);

  socket.on("action", (data) => {
    console.log(data);
    socket.broadcast.emit("action",data);
  });
  

  socket.on("data", (data) => {
    console.log(data);
    socket.emit("message",data);
  });

  socket.on("join_room", (data) => {
    socket.join(data);
  });

  socket.on("send_message", (data) => {
    socket.to(data.room).emit("receive_message", data);
  });
});

// Obtain the local IP address of the host machine
const networkInterfaces = os.networkInterfaces();
const ipAddress = networkInterfaces['Wi-Fi'][1].address; // Adjust this based on your network interface

server.listen(3001, ipAddress, () => {
  console.log(`SERVER IS RUNNING ${ipAddress}`);
});