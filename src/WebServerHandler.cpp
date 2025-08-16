#include "WebServerHandler.h"
#include "MotorControl.h"
#include "LedControl.h"
#include "WiFiManager.h"

ESP8266WebServer server(80);

unsigned long motorInterval = 5 * 60 * 1000;  // Default 5 minutes for motors
unsigned long ledInterval = 10 * 60 * 1000;   // Default 10 minutes for LEDs

void setupServer() {
  // Main page
  server.on("/", handleRoot);
  
  // Motor control endpoints
  server.on("/motor/on", handleOn);
  server.on("/motor/off", handleOff);
  
  // LED control endpoints
  server.on("/led/on", handleLedOn);
  server.on("/led/off", handleLedOff);
  
  // Configuration endpoints
  server.on("/wifi", HTTP_POST, handleWiFiSave);
  server.on("/speed", HTTP_GET, handleSpeed);
  server.on("/motor-interval", HTTP_GET, handleMotorInterval);
  server.on("/led-interval", HTTP_GET, handleLedInterval);
  
  // Enable CORS for better performance
  server.enableCORS(true);
  
  server.begin();
  Serial.println("Web server iniciado na porta 80");
}

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Hydroponic System</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body { 
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; 
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      min-height: 100vh; color: #333; padding: 20px;
    }
    .container { max-width: 800px; margin: 0 auto; }
    .header { text-align: center; margin-bottom: 30px; }
    .header h1 { color: white; font-size: 2.5em; margin-bottom: 10px; text-shadow: 2px 2px 4px rgba(0,0,0,0.3); }
    .header p { color: rgba(255,255,255,0.8); font-size: 1.1em; }
    .card { 
      background: white; border-radius: 15px; padding: 25px; margin-bottom: 20px; 
      box-shadow: 0 10px 30px rgba(0,0,0,0.1); transition: transform 0.3s ease;
    }
    .card:hover { transform: translateY(-5px); }
    .card h2 { color: #333; margin-bottom: 20px; text-align: center; }
    .control-group { display: flex; justify-content: center; gap: 15px; margin-bottom: 20px; flex-wrap: wrap; }
    .btn { 
      padding: 12px 25px; border: none; border-radius: 8px; cursor: pointer; 
      font-size: 16px; font-weight: 600; transition: all 0.3s ease; min-width: 120px;
    }
    .btn:hover { transform: translateY(-2px); box-shadow: 0 5px 15px rgba(0,0,0,0.2); }
    .btn-on { background: linear-gradient(45deg, #4CAF50, #45a049); color: white; }
    .btn-off { background: linear-gradient(45deg, #f44336, #da190b); color: white; }
    .btn-primary { background: linear-gradient(45deg, #2196F3, #0b7dda); color: white; }
    .status { 
      text-align: center; padding: 10px; margin: 15px 0; border-radius: 8px; 
      background: #e8f5e8; color: #2e7d32; font-weight: 600; min-height: 40px;
      display: flex; align-items: center; justify-content: center;
    }
    .form-group { margin-bottom: 20px; }
    .form-group label { display: block; margin-bottom: 8px; font-weight: 600; color: #555; }
    .form-input { 
      width: 100%; padding: 12px; border: 2px solid #e0e0e0; border-radius: 8px; 
      font-size: 16px; transition: border-color 0.3s ease;
    }
    .form-input:focus { outline: none; border-color: #2196F3; }
    .slider-container { text-align: center; }
    .slider { width: 100%; margin: 15px 0; }
    .slider-value { 
      font-size: 1.5em; font-weight: bold; color: #2196F3; 
      background: #f0f7ff; padding: 10px; border-radius: 8px; margin-bottom: 15px;
    }
    .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 20px; }
    @media (max-width: 768px) {
      .control-group { flex-direction: column; align-items: center; }
      .btn { width: 200px; }
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="header">
      <h1>🌱 Hydroponic System</h1>
      <p>Automated control for your cultivation</p>
    </div>

    <div class="grid">
      <div class="card">
        <h2>💧 Motor Control</h2>
        <div class="control-group">
          <button class="btn btn-on" onclick="toggleMotor('/motor/on')">🟢 On</button>
          <button class="btn btn-off" onclick="toggleMotor('/motor/off')">🔴 Off</button>
        </div>
      </div>

      <div class="card">
        <h2>💡 Light Control</h2>
        <div class="control-group">
          <button class="btn btn-on" onclick="toggleLed('/led/on')">🟢 On</button>
          <button class="btn btn-off" onclick="toggleLed('/led/off')">🔴 Off</button>
        </div>
      </div>
    </div>

    <div class="card">
      <h2>⚡ Motor Speed</h2>
      <div class="slider-container">
        <div class="slider-value" id="speedValue">100%</div>
        <input type="range" class="slider" id="speedSlider" min="0" max="100" value="100" oninput="updateSpeed(this.value)">
      </div>
    </div>

    <div class="grid">
      <div class="card">
        <h2>📶 Wi-Fi Configuration</h2>
        <form id="wifiForm" onsubmit="saveWiFi(event)">
          <div class="form-group">
            <label>SSID:</label>
            <input class="form-input" name="ssid" type="text" required>
          </div>
          <div class="form-group">
            <label>Password:</label>
            <input class="form-input" name="password" type="password" required>
          </div>
          <button class="btn btn-primary" type="submit">Save Configuration</button>
        </form>
      </div>

      <div class="card">
        <h2>⏰ Motor Interval</h2>
        <form id="motorIntervalForm" onsubmit="setMotorInterval(event)">
          <div class="form-group">
            <label>Minutes (1-1440):</label>
            <input class="form-input" name="minutes" type="number" min="1" max="1440" placeholder="5" required>
          </div>
          <button class="btn btn-primary" type="submit">Set Motor Interval</button>
        </form>
      </div>
    </div>

    <div class="card">
      <h2>💡 LED Interval</h2>
      <form id="ledIntervalForm" onsubmit="setLedInterval(event)">
        <div class="form-group">
          <label>Minutes (1-1440):</label>
          <input class="form-input" name="minutes" type="number" min="1" max="1440" placeholder="10" required>
        </div>
        <button class="btn btn-primary" type="submit">Set LED Interval</button>
      </form>
    </div>

    <div class="status" id="statusMsg">System ready ✅</div>
  </div>

  <script>
    let debounceTimer;
    
    function showStatus(msg, isError = false) {
      const statusEl = document.getElementById("statusMsg");
      statusEl.textContent = msg;
      statusEl.style.background = isError ? '#ffebee' : '#e8f5e8';
      statusEl.style.color = isError ? '#c62828' : '#2e7d32';
    }

    function updateSpeed(val) {
      document.getElementById('speedValue').textContent = val + "%";
      clearTimeout(debounceTimer);
      debounceTimer = setTimeout(() => {
        fetch(`/speed?value=${val}`)
          .then(res => res.text())
          .then(msg => showStatus(msg))
          .catch(() => showStatus("Error adjusting speed", true));
      }, 300);
    }

    function toggleMotor(endpoint) {
      fetch(endpoint)
        .then(res => res.text())
        .then(msg => showStatus(msg))
        .catch(() => showStatus("Connection error", true));
    }

    function toggleLed(endpoint) {
      fetch(endpoint)
        .then(res => res.text())
        .then(msg => showStatus(msg))
        .catch(() => showStatus("Error connecting", true));
    }

    function saveWiFi(event) {
      event.preventDefault();
      const formData = new FormData(event.target);
      fetch('/wifi', { method: 'POST', body: formData })
        .then(() => showStatus("Configuration saved! Restarting..."))
        .catch(() => showStatus("Error saving configuration", true));
    }

    function setMotorInterval(event) {
      event.preventDefault();
      const formData = new FormData(event.target);
      const minutes = formData.get('minutes');
      fetch(`/motor-interval?minutes=${minutes}`)
        .then(res => res.text())
        .then(msg => showStatus(msg))
        .catch(() => showStatus("Error setting motor interval", true));
    }

    function setLedInterval(event) {
      event.preventDefault();
      const formData = new FormData(event.target);
      const minutes = formData.get('minutes');
      fetch(`/led-interval?minutes=${minutes}`)
        .then(res => res.text())
        .then(msg => showStatus(msg))
        .catch(() => showStatus("Error setting LED interval", true));
    }
  </script>
</body>
</html>
)rawliteral";
  server.send(200, "text/html", html);
}

void handleOn() {
  Serial.println("Web request: Motor ON");
  bool ledWasOn = areLedOn();
  turnMotorsOn();
  server.sendHeader("Cache-Control", "no-cache");
  if (ledWasOn) {
    server.send(200, "text/plain", "✅ Motors on (LEDs auto-turned off for safety)");
  } else {
    server.send(200, "text/plain", "✅ Motors turned on successfully");
  }
}

void handleOff() {
  Serial.println("Web request: Motor OFF");
  turnMotorsOff();
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/plain", "⏹️ Motors turned off successfully");
}

void handleLedOn() {
  Serial.println("Web request: LED ON");
  bool motorsWereOn = areMotorsOn();
  turnLedOn();
  server.sendHeader("Cache-Control", "no-cache");
  if (motorsWereOn) {
    server.send(200, "text/plain", "💡 LEDs on (Motors auto-turned off for safety)");
  } else {
    server.send(200, "text/plain", "💡 LED turned on successfully");
  }
}

void handleLedOff() {
  Serial.println("Web request: LED OFF");
  turnLedOff();
  server.sendHeader("Cache-Control", "no-cache");
  server.send(200, "text/plain", "🌙 LED turned off successfully");
}

void handleWiFiSave() {
  String newSSID = server.arg("ssid");
  String newPassword = server.arg("password");
  saveWiFiCredentials(newSSID, newPassword);
  server.send(200, "text/html", "<p>Credentials saved. Restarting...</p>");
  delay(2000);
  ESP.restart();
}

void handleSpeed() {
  if (server.hasArg("value")) {
    int percent = server.arg("value").toInt();
    percent = constrain(percent, 0, 100);
    int speedPWM = map(percent, 0, 100, 0, 1023);
    setMotorSpeed(speedPWM);
    server.sendHeader("Cache-Control", "no-cache");
    server.send(200, "text/plain", "⚡ Speed set to " + String(percent) + "%");
  } else {
    server.send(400, "text/plain", "❌ Missing 'value' parameter");
  }
}

void handleMotorInterval() {
  if (server.hasArg("minutes")) {
    int minutes = server.arg("minutes").toInt();
    minutes = constrain(minutes, 1, 1440);
    motorInterval = (unsigned long)minutes * 60000UL;
    server.sendHeader("Cache-Control", "no-cache");
    server.send(200, "text/plain", "⏰ Motor interval set to " + String(minutes) + " minutes");
    Serial.println("Motor interval set to " + String(minutes) + " minutes");
  } else {
    server.send(400, "text/plain", "❌ Missing 'minutes' parameter");
  }
}

void handleLedInterval() {
  if (server.hasArg("minutes")) {
    int minutes = server.arg("minutes").toInt();
    minutes = constrain(minutes, 1, 1440);
    ledInterval = (unsigned long)minutes * 60000UL;
    server.sendHeader("Cache-Control", "no-cache");
    server.send(200, "text/plain", "💡 LED interval set to " + String(minutes) + " minutes");
    Serial.println("LED interval set to " + String(minutes) + " minutes");
  } else {
    server.send(400, "text/plain", "❌ Missing 'minutes' parameter");
  }
}
