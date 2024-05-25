const char WEBPAGE[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Plant Monitoring System</title>
    <style>
        body {
            background-color: #4b5563;
            color: white;
            min-height: 100vh;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            margin: 0;
        }
        h1 {
            font-size: 2.25rem;
            font-weight: bold;
            margin-bottom: 2rem;
        }
        .card {
            background-color: #2d2d2d;
            box-shadow: 0 1px 3px rgba(0, 0, 0, 0.1), 0 1px 2px rgba(0, 0, 0, 0.06);
            border-radius: 0.5rem;
            padding: 1.5rem;
            margin-bottom: 2rem;
        }
        .text-xl {
            font-size: 1.25rem;
            margin-bottom: 0.5rem;
        }
        .font-semibold {
            font-weight: 600;
        }
        .button {
            background-color: #3b82f6;
            color: white;
            padding: 0.5rem 1rem;
            border-radius: 0.5rem;
            border: none;
            cursor: pointer;
            transition: background-color 0.3s;
            margin-right: 1rem;
        }
        .button:hover {
            background-color: #2563eb;
        }
        .button.red {
            background-color: #ef4444;
        }
        .button.red:hover {
            background-color: #dc2626;
        }
        .state-container {
            box-shadow: 0 1px 3px rgba(0, 0, 0, 0.1), 0 1px 2px rgba(0, 0, 0, 0.06);
            border-radius: 0.5rem;
            padding: 1rem;
            background-color: #4caf50; /* Default state color */
        }
    </style>
</head>
<body>
    <h1>Plant Monitoring System</h1>
    <div class="card">
        <p class="text-xl">Temperature: <span class="font-semibold" id="temperature">N/A</span> °C</p>
        <p class="text-xl">Humidity: <span class="font-semibold" id="humidity">N/A</span> %</p>
        <p class="text-xl">Soil Moisture: <span class="font-semibold" id="soilMoisture">N/A</span> %</p>
        <p class="text-xl">Light Amount: <span class="font-semibold" id="lightAmount">N/A</span> lx?</p>
    </div>
    <div style="display: flex; gap: 1rem; margin-bottom: 1rem;">
        <button onclick="sendCommand('water')" class="button">Water</button>
        <button onclick="sendCommand('stop')" class="button red">Stop</button>
    </div>
    <div id="currentStateContainer" class="state-container">
        <p class="text-xl">Current State: <span class="font-semibold" id="currentState">N/A</span></p>
    </div>

    <div style="margin-top: 2rem;">
        <button onclick="getSensorReadings()" class="button">Update Sensor Readings</button>
    </div>

    <script>
        async function sendCommand(command) {
            try {
                const response = await fetch(`/toggleLED?action=${command}`);
                if (!response.ok)
                    throw new Error('Error sending command');
            } catch (error) {
                console.error('Error: ', error);
            }
        }

        async function getSensorReadings() {
            try {
                const data = await fetchReading();
                updateSensorData(data);
            } catch (error) {
                console.error('Error fetching sensor readings:', error);
            }
        }

        async function fetchReading() {
            try {
                const response = await fetch("/readSensor", {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    }
                });

                if (!response.ok)
                    throw new Error('Error fetching readings');

                const data = await response.json();
                return data;
            } catch (error) {
                console.error('Error: ', error);
            }
        }

        function updateSensorData(data) {
            if (data) {
                document.getElementById('temperature').innerText = data.temperature;
                document.getElementById('humidity').innerText = data.humidity;
                document.getElementById('soilMoisture').innerText = data.moisture;
                document.getElementById('lightAmount').innerText = data.light;

                const currentState = getCurrentState(data);
                document.getElementById('currentState').innerText = currentState;
                updateStateContainer(currentState);
            } else {
                console.error('No data received to update sensor values.');
            }
        }

        function getCurrentState(data) {
            if (data.temperature > 30) {
                return 'Warning';
            } else if (data.moisture < 20) {
                return 'Critical';
            } else {
                return 'Normal';
            }
        }

        function getStateColor(state) {
            switch (state) {
                case 'Normal': return '#4caf50';
                case 'Warning': return '#ff9800';
                case 'Critical': return '#f44336';
                default: return '#4caf50';
            }
        }

        function updateStateContainer(state) {
            document.getElementById('currentStateContainer').style.backgroundColor = getStateColor(state);
        }
    </script>
</body>
</html>


)=====";