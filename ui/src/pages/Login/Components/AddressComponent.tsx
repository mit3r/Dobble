import { mainStore } from "@/store";
import { useState } from "react";

export default function AddressComponent() {
  const [ip, setIp] = useState<string>("127.0.0.1");
  const [port, setPort] = useState<string>("1500");

  const [error, setError] = useState<string | null>(null);

  const handleSubmit = () => {
    const portNumber = parseInt(port, 10);
    if (isNaN(portNumber)) return setError("Port must be a number");
    if (portNumber < 0 || portNumber > 65536) return setError("Port must be between 0 and 65536");

    // Regex to validate IPv4 address
    // Znalazłem tutaj: 04.01.2026, 14:28 GMT+1, https://stackoverflow.com/questions/5284147/validating-ipv4-addresses-with-regexp
    const ipv4Regex = /^(((?!25?[6-9])[12]\d|[1-9])?\d\.?\b){4}$/;
    if (!ipv4Regex.test(ip)) return setError("Invalid IP address format");

    setError(null);
    mainStore.getState().browser.setLobbyAddress(ip, portNumber);
    mainStore.getState().browser.connectToLobbyServer();
  };

  return (
    <div className="p-4 px-8 border-2 rounded-xl grid-cols-2 gap-2 grid">
      <p className="col-span-2">Enter lobby server you want to connect with:</p>

      <label htmlFor="ip">IP Address:</label>
      <input
        className="border-2 p-1"
        type="text"
        id="ip"
        value={ip}
        onChange={(e) => setIp(e.target.value)}
      />

      <label htmlFor="port">Port:</label>
      <input
        className="border-2 p-1"
        type="text"
        id="port"
        value={port}
        onChange={(e) => setPort(e.target.value)}
      />

      <div className="h-min col-span-2">{error && <p className="text-red-500">{error}</p>}</div>

      <div className="col-span-2 flex">
        <button
          className="p-2 px-4 border-2 active:bg-black active:text-white"
          onClick={handleSubmit}
        >
          Connect
        </button>
      </div>
    </div>
  );
}
