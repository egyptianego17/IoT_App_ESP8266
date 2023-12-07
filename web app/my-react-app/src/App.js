import { useEffect, useState } from "react";
import "./App.css";
import io from "socket.io-client";
import Home from "./Compos/Home/Home";
import Register from "./Compos/Register/Register";
const socket = io.connect("http://192.168.8.136:3001");

function App() {

    // Define your labels dynamically
    const labelsData = [
      { title: "Label 1", content: "Content 1" },
      { title: "Label 2", content: "Content 2" },
      { title: "Label 3", content: "Content 3" },
      { title: "Label 4", content: "Content 4" },
    ];
  
  // Set up socket connection
  useEffect(() => {
    socket.on("recieve_message", (data) => {
      alert(data.message);
      
    });
    socket.on("message", (data) =>{
      const parsedSensorData = parseSensorData(data);
      alert(data);
    })
    
    
  }, [socket]);

  const parseSensorData = (data) => {
    const { accelerationX, accelerationY, accelerationZ } = data;

    // You can modify this part to suit your specific needs
    return [
      { title: "Acceleration X", content: accelerationX.toFixed(2) },
      { title: "Acceleration Y", content: accelerationY.toFixed(2) },
      { title: "Acceleration Z", content: accelerationZ.toFixed(2) },
      { title: "Custom Label", content: "Custom Value" },
    ];
  };



  // State for user information
  const [user, setUser] = useState("12");

  // Function to handle user registration
  const registerHandler = (user) => {
    setUser(user);
  };

  // Function to handle slider changes in Home.js
  const handleSliderChange = (value) => {
    // Do something with the slider value in App.js
    console.log("Slider value changed in App.js:", value);
    socket.emit("action",{source :"slider",value: value})
  };

  // Function to handle button clicks in Home.js
  const handleButtonClick = () => {
    // Do something when the button in Home.js is clicked
    console.log("Button clicked in App.js");
    socket.emit("action",{source :"button",value: "pressed"})
  };

  // Functions to handle switch changes in Home.js
  const handleSwitch1Change = (checked) => {
    // Do something with the state of the first switch
    console.log("Switch 1 state changed in App.js:", checked);
    socket.emit("action",{source :"switch1",value: checked})
  };

  const handleSwitch2Change = (checked) => {
    // Do something with the state of the second switch
    console.log("Switch 2 state changed in App.js:", checked);
    socket.emit("action",{source :"switch2",value: checked})
  };

  return (
    <>
      {user ? (
        <Home
          user={user}
          onSliderChange={handleSliderChange}
          onButtonClick={handleButtonClick}
          onSwitch1Change={handleSwitch1Change}
          onSwitch2Change={handleSwitch2Change}
          labelsData={labelsData} // Pass labelsData as a prop
        />
      ) : (
        <Register registerHandler={registerHandler} />
      )}
    </>
  );
}

export default App;
