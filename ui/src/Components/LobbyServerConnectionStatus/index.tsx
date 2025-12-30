import { mainStore } from "@/store";
import { ConnectionError, ConnectionStatus } from "@/types/dobble";
import { useMemo } from "react";
import { useStore } from "zustand/react";
import Dots from "@/Components/Dots";

export default function LobbyServerConnectionStatus() {
  const connectionStatus = useStore(mainStore, (state) => state.browser.lobbyConnection);
  const connectionError = useStore(mainStore, (state) => state.browser.lobbyError);

  const element = useMemo(() => {
    if (connectionStatus === ConnectionStatus.Connected) return null;

    if (connectionStatus === ConnectionStatus.Connecting)
      return (
        <p>
          Connecting to server
          <Dots />
        </p>
      );

    if (connectionStatus === ConnectionStatus.Retrying)
      return (
        <p>
          Reconnecting to server
          <Dots />
        </p>
      );

    let errorMessage = "Disconnected from server.";
    if (connectionError) {
      switch (connectionError) {
        case ConnectionError.NetworkError:
          errorMessage = "Network Error. Please check your connection.";
          break;
        case ConnectionError.Timeout:
          errorMessage = "Connection Timeout. Please try again.";
          break;
        case ConnectionError.ConnectionRefused:
          errorMessage = "Connection Refused by the server.";
          break;
        case ConnectionError.HostNotFound:
          errorMessage = "Host Not Found. Please check the server address.";
          break;
        default:
          errorMessage = "Unknown Error.";
      }
    }

    return (
      <div className="text-red-500 flex gap-4">
        {errorMessage} <button className="p-2 border-red-500 px-4">Retry</button>
      </div>
    );
  }, [connectionStatus, connectionError]);

  return <div className="flex flex-col items-center justify-center px-4 w-full">{element}</div>;
}
