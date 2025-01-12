async function fetchData() {
    try {
        const response = await fetch('http://your-esp32-ip/data'); // Replace with your ESP32 IP address
        const data = await response.json();
        
        document.getElementById('servo1').innerText = data.servo1;
        document.getElementById('servo2').innerText = data.servo2;
        document.getElementById('servo3').innerText = data.servo3;
        document.getElementById('servo4').innerText = data.servo4;
        document.getElementById('distance').innerText = data.distance;
        document.getElementById('battery').innerText = data.battery;
    } catch (error) {
        console.error('Error fetching data:', error);
    }
}

async function sendCommand(command) {
    try {
        const response = await fetch(`http://your-esp32-ip/command?cmd=${command}`, {
            method: 'GET'
        });
        const result = await response.json();
        console.log(`Command ${command} executed:`, result);
    } catch (error) {
        console.error(`Error sending command ${command}:`, error);
    }
}
