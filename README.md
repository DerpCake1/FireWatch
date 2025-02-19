# limitd

A GitHub repo for MakerHacks 25' with Jordan Tran, Samit Rath, and Sebastian Sandoval.

## Wildfire Risk Assessment System

This project uses a **temperature/humidity sensor** and a **rotary encoder (acting as an anemometer)** to collect environmental data. The data is sent to **ThingSpeak** for live monitoring and analyzed using a **decision tree algorithm** to assess the risk of wildfires in a specific area. A **Flask web server** hosts a live **HTML dashboard** displaying the real-time data and wildfire risk analysis.

---

## Table of Contents
1. [Project Overview](#project-overview)
2. [Hardware Requirements](#hardware-requirements)
3. [Software Requirements](#software-requirements)
4. [Setup Instructions](#setup-instructions)
5. [How It Works](#how-it-works)
6. [ThingSpeak Integration](#thingspeak-integration)
7. [Decision Tree Algorithm](#decision-tree-algorithm)
8. [Web Dashboard (Flask & HTML)](#web-dashboard-flask--html)
9. [Results and Output](#results-and-output)
10. [Future Improvements](#future-improvements)
11. [License](#license)

---

## Project Overview
This system collects real-time environmental data (temperature, humidity, and wind speed) using a **DHT22 sensor** and a **rotary encoder** configured as an anemometer. The data is transmitted to **ThingSpeak** for live visualization. A **decision tree algorithm** processes the data to predict the risk of wildfires in the monitored area. Additionally, a **Flask web server** displays a real-time HTML dashboard for easy data monitoring.

---

## Hardware Requirements
- **Microcontroller**: ESP32 or Arduino
- **Temperature/Humidity Sensor**: DHT22
- **Rotary Encoder**: Used as an anemometer to measure wind speed
- **LCD Display** (optional): For local data visualization
- **Breadboard and Jumper Wires**
- **Power Supply**: 5V or 3.3V, depending on the microcontroller

---

## Software Requirements
- **Arduino IDE** or **PlatformIO**
- **Python** (for Flask web server and decision tree model)
- **ThingSpeak API** for data logging and visualization
- **Flask** (for hosting the live dashboard)
- Libraries:
  - `DHT.h` for the temperature/humidity sensor
  - `Encoder.h` for the rotary encoder
  - `WiFi.h` or `ESP8266WiFi.h` for Wi-Fi connectivity
  - `ThingSpeak.h` for data upload
  - `Flask` (for web server)
  - `pandas`, `numpy`, `scikit-learn` (for decision tree analysis)

---

## Setup Instructions
1. **Hardware Setup**:
   - Connect the DHT22 sensor to the microcontroller (e.g., GPIO pin for data).
   - Connect the rotary encoder to measure wind speed (e.g., GPIO pins for pulse counting).
   - (Optional) Connect an LCD display for local data visualization.

2. **Microcontroller Software Setup**:
   - Install the required libraries in the Arduino IDE or PlatformIO.
   - Configure the Wi-Fi credentials in the code.
   - Set up a ThingSpeak channel and update the API key in the code.
   - Upload the Arduino sketch to the microcontroller.

3. **Flask Web Server Setup**:
   - Install dependencies:
     ```sh
     pip install -r requirements.txt
     ```
   - Run the Flask server:
     ```sh
     python app.py
     ```
   - Open `http://127.0.0.1:5000` in your browser to view the dashboard.

4. **Run the Decision Tree Algorithm**:
   - If the decision tree is implemented in Python, run the script:
     ```sh
     python wildfire_model.py
     ```

---

## How It Works
1. **Data Collection**:
   - The DHT22 sensor measures temperature and humidity.
   - The rotary encoder (anemometer) calculates wind speed based on pulse counts.

2. **Data Upload**:
   - The collected data is sent to ThingSpeak via Wi-Fi for live monitoring.

3. **Risk Assessment**:
   - A decision tree algorithm processes the data to determine the wildfire risk level (Low, Medium, High).

4. **Web Dashboard**:
   - A Flask web server retrieves real-time data and serves an HTML dashboard with live metrics and risk assessment.

---

## ThingSpeak Integration
- Create a ThingSpeak channel with fields for:
  - Temperature
  - Humidity
  - Wind Speed
  - Wildfire Risk Level
- Use the ThingSpeak API to send data from the microcontroller.

---

## Decision Tree Algorithm
The decision tree uses the following features to predict wildfire risk:
- **Temperature**: Higher temperatures increase risk.
- **Humidity**: Lower humidity increases risk.
- **Wind Speed**: Higher wind speeds increase risk.

The algorithm is trained on historical wildfire data and deployed to classify real-time data into risk categories.

---

## Web Dashboard (Flask & HTML)
### Flask Backend:
- Retrieves live data from ThingSpeak.
- Runs the decision tree model to determine risk level.
- Serves the real-time dashboard via HTML.

### HTML Frontend:
- Displays:
  - **Live temperature, humidity, and wind speed**
  - **Current wildfire risk level**
  - **Graphs for historical trends**
- Refreshes dynamically to update with the latest data.

**Example Flask API Endpoint:**
```python
@app.route('/')
def index():
    data = get_latest_data_from_thingspeak()
    risk = calculate_risk(data)
    return render_template('index.html', data=data, risk=risk)
```
---

## Results and Output
- **ThingSpeak Dashboard**: Live graphs for temperature, humidity, wind speed, and wildfire risk.
- **Serial Monitor**: Debugging and logging output.
- **Flask Web Dashboard**: Real-time display of sensor data and risk level.

---

## Future Improvements
- Add more sensors (e.g., soil moisture, air quality) for better accuracy.
- Implement a mobile app for real-time alerts.
- Use machine learning models (e.g., Random Forest) for improved predictions.
- Integrate with IoT platforms like AWS IoT or Google Cloud IoT.

---

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
