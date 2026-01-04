import  { mainStore } from "@/store";
import  { useStore } from "zustand/react";
import ServerStatus from ".";
import { ConnectionStatus } from "@/types/dobble";

export default function LobbyServStatus() {
  const connectionStatus = useStore(mainStore, (state) => state.browser.lobbyConnection);
  const connectionError = useStore(mainStore, (state) => state.browser.lobbyError);
  const connectToLobbyServer = useStore(mainStore, (state) => state.browser.connectToLobbyServer);
  if (connectionStatus === ConnectionStatus.Connected) return null;

  return (
    <div className="w-full flex justify-center">
      <span>Lobby server: </span>
      <ServerStatus
        connectionStatus={connectionStatus}
        connectionError={connectionError}
        retryCallback={connectToLobbyServer}
      />
    </div>
  );
}