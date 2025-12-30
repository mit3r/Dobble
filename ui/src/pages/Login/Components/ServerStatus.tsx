import { mainStore } from "@/store";
import { ConnectionError, ConnectionStatus } from "@/types/dobble";
import { useEffect, useMemo, useState } from "react";
import { useStore } from "zustand";

export default function Connecting() {
  const lobbyServerStatus = useStore(mainStore, (state) => state.views.lobbyServerStatus);
  const lobbyServerError = useStore(mainStore, (state) => state.views.lobbyServerError);

  const readableStatus = useMemo(() => {
    switch (lobbyServerStatus) {
      case ConnectionStatus.Connecting:
        return (
          <span>
            Connecting
            <Dots />
          </span>
        );
      case ConnectionStatus.Connected:
        return "Connected";
      case ConnectionStatus.Disconnected:
        return "Disconnected";
      default:
        return (
          <span>
            Retrying
            <Dots />
          </span>
        );
    }
  }, [lobbyServerStatus]);

  const readableError = useMemo(() => {
    if (!lobbyServerError) return null;

    switch (lobbyServerError) {
      case ConnectionError.NetworkError:
        return "Network Error. Please check your connection.";
      case ConnectionError.Timeout:
        return "Connection Timeout. Please try again.";
      case ConnectionError.ConnectionRefused:
        return "Connection Refused by the server.";
      case ConnectionError.HostNotFound:
        return "Host Not Found. Please check the server address.";
      default:
        return "Unknown Error";
    }
  }, [lobbyServerError]);

  return (
    <div className="flex flex-col items-center justify-center h-full gap-4">
      <h1 className="text-2xl font-bold">Connecting to Lobby Server...</h1>
      <button className="p-2 px-4 border-2 rounded">Retry</button>
    </div>
  );
}

function Dots() {
  const [value, setValue] = useState(0);

  useEffect(() => {
    const interval = setInterval(() => setValue((prev) => (prev + 1) % 4), 500);
    return () => clearInterval(interval);
  }, []);

  const dots = useMemo(() => {
    return ".".repeat(value);
  }, [value]);

  return <span>{dots}</span>;
}
