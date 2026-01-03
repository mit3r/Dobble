import { mainStore } from "@/store";
import { ConnectionError, ConnectionStatus } from "@/types/dobble";
import { useMemo } from "react";
import { useStore } from "zustand/react";
import Dots from "@/Components/Dots";

export default function LobbyServerConnectionStatus() {
  const connectionStatus = useStore(mainStore, (state) => state.browser.lobbyConnection);
  const connectionError = useStore(mainStore, (state) => state.browser.lobbyError);
  window.bridges?.main.callMsg(
    `LobbyServerConnectionStatus rendered: ${connectionStatus}, ${connectionError}`
  ); // DEBUG

  const connectionStatusElement = useMemo(() => {
    switch (connectionStatus) {
      case ConnectionStatus.Connecting:
        return <Dots text="Connecting to server" />;
      case ConnectionStatus.HostLookup:
        return <Dots text="Looking up server host" />;
      case ConnectionStatus.Retrying:
        return <Dots text="Reconnecting to server" />;
      default:
        return null;
    }
  }, [connectionStatus]);

  const connectionErrorElement = useMemo(() => {
    if (connectionError === null || connectionError === ConnectionError.None) return null;

    switch (connectionError) {
      case ConnectionError.NetworkError:
        return "Network Error. Please check your connection.";
      case ConnectionError.Timeout:
        return "Connection Timeout. Please try again.";
      case ConnectionError.ConnectionRefused:
        return "Connection Refused by the server.";
      case ConnectionError.HostNotFound:
        return "Host Not Found. Please check the server address.";
      default:
        return "Disconnected from server.";
    }
  }, [connectionError]);

  const element = useMemo(() => {
    if (connectionStatusElement) {
      return <div className="text-blue-500">{connectionStatusElement}</div>;
    }

    if (!connectionErrorElement) return null
    
    return (
      <div className="text-red-500 flex gap-4">
        {connectionErrorElement} <button className="p-2 border-red-500 px-4">Retry</button>
      </div>
    );

    }, [connectionStatus, connectionError]);
  

  return <div className="flex flex-col items-center justify-center px-4 w-full">{element}</div>;
}
