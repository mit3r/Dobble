import { mainStore } from "@/store";
import { ConnectionError, ConnectionStatus } from "@/types/dobble";
import { ConnectionStatusSpan } from "@/Components/Statuses";
import { useMemo } from "react";
import { useStore } from "zustand/react";
import { ConnectionErrorSpan } from "../Statuses";

export default function LobbyServerStatus() {
  const connectionStatus = useStore(mainStore, (state) => state.browser.lobbyConnection);
  const connectionError = useStore(mainStore, (state) => state.browser.lobbyError);
  const connectToLobbyServer = useStore(mainStore, (state) => state.browser.connectToLobbyServer);

  window.bridges?.main.callMsg(
    "LobbyServerConnectionStatus - connectionStatus: " +
      connectionStatus +
      ", connectionError: " +
      connectionError
  );

  const connectionStatusElement = useMemo(() => {
    if (connectionStatus === ConnectionStatus.Connected) return null;
    return <ConnectionStatusSpan status={connectionStatus} />;
  }, [connectionStatus]);

  const connectionErrorElement = useMemo(() => {
    if (connectionError === null || connectionError === ConnectionError.None) return null;
    return <ConnectionErrorSpan error={connectionError} />;
  }, [connectionError]);

  if (connectionErrorElement)
    return (
      <div className="text-red-500 flex gap-4 items-baseline w-full justify-center">
        <span>{connectionErrorElement}</span>
        <button className="p-2 border-red-500 px-4" onClick={connectToLobbyServer}>
          Retry
        </button>
      </div>
    );

  if (connectionStatusElement)
    return (
      <div className="text-blue-500 flex justify-center w-full">
        {connectionStatusElement}
        {connectionStatus === ConnectionStatus.Disconnected && (
          <button className="p-2 border-blue-500 px-4 ml-4" onClick={connectToLobbyServer}>
            Reconnect
          </button>
        )}
      </div>
    );

  return <></>;
}
