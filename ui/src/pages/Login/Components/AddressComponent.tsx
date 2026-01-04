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
    <div className="flex flex-col gap-6">
      <p className="text-center text-dobble-text-muted">
        Enter lobby server you want to connect with:
      </p>

      <div className="grid grid-cols-[auto_1fr] gap-4 items-center">
        <label htmlFor="ip" className="text-sm font-bold text-dobble-text-muted">
          IP Address
        </label>
        <input
          className="input-field"
          type="text"
          id="ip"
          value={ip}
          onChange={(e) => setIp(e.target.value)}
          placeholder="127.0.0.1"
        />

        <label htmlFor="port" className="text-sm font-bold text-dobble-text-muted">
          Port
        </label>
        <input
          className="input-field"
          type="text"
          id="port"
          value={port}
          onChange={(e) => setPort(e.target.value)}
          placeholder="1500"
        />
      </div>

      {error && (
        <div className="text-center p-3 rounded-xl bg-dobble-danger/20 text-dobble-danger">
          {error}
        </div>
      )}

      <button className="btn-primary w-full" onClick={handleSubmit}>
        Connect to Server
      </button>
    </div>
  );
}
