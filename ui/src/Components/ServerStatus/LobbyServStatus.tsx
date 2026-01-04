import { mainStore } from "@/store";
import { ConnectionStatus } from "@/types/dobble";
import { useStore } from "zustand/react";
import ServerStatus from ".";

export default function LobbyServStatus() {
  const connectionStatus = useStore(mainStore, (state) => state.browser.lobbyConnection);
  const connectionError = useStore(mainStore, (state) => state.browser.lobbyError);
  const connectToLobbyServer = useStore(mainStore, (state) => state.browser.connectToLobbyServer);
  const clearLobbyAddress = useStore(mainStore, (state) => state.browser.clearLobbyAddress);
  if (connectionStatus === ConnectionStatus.Connected) return null;

  return (
    <div className="w-full flex flex-col gap-2">
      <div className="flex items-center gap-2 text-sm text-dobble-text-muted">
        <span>Lobby Server Status</span>
      </div>
      <ServerStatus
        connectionStatus={connectionStatus}
        connectionError={connectionError}
        retryCallback={connectToLobbyServer}
        editCallback={clearLobbyAddress}
      />
    </div>
  );
}
