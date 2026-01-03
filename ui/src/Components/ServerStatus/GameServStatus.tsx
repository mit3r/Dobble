import  { mainStore } from "@/store";
import  { useStore } from "zustand/react";
import ServerStatus from ".";



export default function GameServStatus() {

  const connectionStatus = useStore(mainStore, (state) => state.game.gameServerConnectionStatus);
  const connectionError = useStore(mainStore, (state) => state.game.gameServerConnectionError);

  return (
    <div className="w-full flex justify-center">
      <span>Game server: </span>
        <ServerStatus
          connectionStatus={connectionStatus}
          connectionError={connectionError}
        />
      </div>
  );

}