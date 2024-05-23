"use client"; // This directive ensures the component is treated as a client component

import { useState, useEffect } from 'react';
import axios from 'axios';

export default function Home() {
  const [data, setData] = useState({
    temperature: 0,
    humidity: 0,
    soilMoisture: 0,
    lightAmount: 0
  });
  const [currentState, setCurrentState] = useState('manual');

  useEffect(() => {
    const fetchData = async () => {
      const result = await axios.get('/data');
      setData(result.data);
    };
    fetchData();
    const interval = setInterval(fetchData, 5000);
    return () => clearInterval(interval);
  }, []);

  const sendCommand = async (command) => {
    await axios.post('/control', null, { params: { action: command } });
    setCurrentState(command);
  };

  return (
    <div className="bg-zinc-500 min-h-screen flex flex-col items-center justify-center">
      <h2 className="text-4xl font-bold mb-8 ">Plant Monitoring System</h2>
      <div className="bg-neutral-800 shadow-md rounded-lg p-6 mb-8">
        <p className="text-xl mb-2">Temperature: <span className="font-semibold">{data.temperature}</span> °C</p>
        <p className="text-xl mb-2">Humidity: <span className="font-semibold">{data.humidity}</span> %</p>
        <p className="text-xl mb-2">Soil Moisture: <span className="font-semibold">{data.soilMoisture}</span> %</p>
        <p className="text-xl">Light Amount: <span className="font-semibold">{data.lightAmount}</span> lx</p>
      </div>
      <div className="flex space-x-4 mb-4">
        <button onClick={() => sendCommand('water')} className="bg-blue-500 text-white px-4 py-2 rounded-lg hover:bg-blue-700">Water</button>
        <button onClick={() => sendCommand('stop')} className="bg-red-500 text-white px-4 py-2 rounded-lg hover:bg-red-700">Stop</button>
        <button onClick={() => sendCommand('auto')} className="bg-green-500 text-white px-4 py-2 rounded-lg hover:bg-green-700">Auto</button>
        <button onClick={() => sendCommand('manual')} className="bg-yellow-500 text-white px-4 py-2 rounded-lg hover:bg-yellow-700">Manual</button>
      </div>
      <div className="bg-white shadow-md rounded-lg p-4">
        <p className="text-xl">Current State: <span className="font-semibold">{currentState}</span></p>
      </div>
    </div>
  );
}
