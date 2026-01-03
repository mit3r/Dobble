import  { mainStore } from "@/store";
import  { useStore } from "zustand/react";
import ServerStatus from ".";



export default function LobbyServStatus() {

  const connectionStatus = useStore(mainStore, (state) => state.browser.lobbyConnection);
  const connectionError = useStore(mainStore, (state) => state.browser.lobbyError);
  const connectToLobbyServer = useStore(mainStore, (state) => state.browser.connectToLobbyServer);

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